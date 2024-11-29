#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

#include <signal.h>
int* child_pids = NULL; // Massive for saving PID`s of child processes 
int pnum = 0; // amount of processes

// Process signal SIGALRM
void alarm_handler(int sig) {
    printf("Timeout reached! Killing child processes...\n");
    for (int i = 0; i < pnum; i++) {
        if (child_pids[i] != -1) {
            kill(child_pids[i], SIGKILL); // killing child process
        }
    }
}



int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;
  int timeout = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (seed <= 0){
              printf("Seed must be a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0){
              printf("array_size must be positive\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0){
              printf("Pnum must be positive\n");
              return 1;
            }

            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg);
            if (timeout <= 0) {
              printf("Time out must be positive");
              return 1;
            }
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  child_pids = malloc(sizeof(int) * pnum);
  for (int i = 0; i < pnum; i++){
    child_pids[i] = -1; // Init Pids as -1 (inactive)
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int pipefd[2];
  if (!with_files) {
      pipe(pipefd);
  }
  signal(SIGALRM, alarm_handler);

  if (timeout > 0){
    alarm(timeout); //timer start
  }
  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        int part_size = array_size / pnum;
        int start = i * part_size;
        int end = (i == pnum - 1) ? array_size : start + part_size;
        
        struct MinMax local_min_max = GetMinMax(array, start, end);
        int local_min = local_min_max.min;
        int local_max = local_min_max.max;
        
        // parallel somehow

        if (with_files) {
          // use files here
          char file_name[256];
          sprintf(file_name, "min_max_%d.txt", i);
          FILE *file = fopen(file_name, "w");

          if (file == NULL){
            printf("Error file is empty\n");
            return 1;
          }
          fprintf(file, "%d %d\n", local_min, local_max);
          fclose(file);
        } else {
          // use pipe here
          close(pipefd[0]);
          write(pipefd[1], &local_min, sizeof(int));
          write(pipefd[2], &local_max, sizeof(int));
          close(pipefd[1]);
        }
        return 0;
      } else {
          child_pids[i] = child_pid; //save child process`s pid
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    pid_t child_pid = waitpid(-1, NULL, WNOHANG);
    if (child_pid > 0){
      active_child_processes -= 1;
    }
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
      char file_name[256];
      sprintf(file_name, "min_max_%d.txt", i);
      FILE *file = fopen(file_name, "r");
      if (file == NULL){
        printf("Error file is empty\n");
        return 1;
      }
      fscanf(file, "%d %d", &min, &max);
      fclose(file);
      
    } else {
      // read from pipes
      close(pipefd[1]);
      read(pipefd[0], &min, sizeof(int));
      read(pipefd[0], &max, sizeof(int));


    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  free(child_pids);
  
  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
