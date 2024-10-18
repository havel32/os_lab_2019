#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork(); // Сreating new process

    if (pid == 0) {
        // child process
        printf("Starting sequential_min_max inside child process.\n");

        // args for sequential_min_max
        char *args[] = {"./sequential_min_max", "3", "100", NULL};

        // execvp replaceing this process to sequential_min_max
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            return 1;
        }
    } else if (pid > 0) {
        // Parent process
        int status;
        wait(&status); // waiting end of child process
        if (WIFEXITED(status)) {
            printf("programm executed with code: %d\n", WEXITSTATUS(status));
        }
    } else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
