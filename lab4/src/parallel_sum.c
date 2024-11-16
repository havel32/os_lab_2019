#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// Функция для подсчёта времени
double GetElapsedTime(struct timeval start, struct timeval end) {
  return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

struct SumArgs {
  int *array;
  int begin;
  int end;
};

// Функция для подсчёта суммы в заданном диапазоне
int Sum(const struct SumArgs *args) {
  int sum = 0;
  for (int i = args->begin; i < args->end; i++) {
    sum += args->array[i];
  }
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

// Генерация массива
void GenerateArray(int *array, size_t size, unsigned int seed) {
  srand(seed);
  for (size_t i = 0; i < size; i++) {
    array[i] = rand() % 100; // Числа от 0 до 99
  }
}


// Парсинг аргументов командной строки
int ParseArg(const char *arg_name, const char *arg_value, uint32_t *result) {
  char *end;
  long value = strtol(arg_value, &end, 10);
  if (*end != '\0' || value <= 0) {
    printf("Invalid value for %s: %s\n", arg_name, arg_value);
    return -1;
  }
  *result = (uint32_t)value;
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 7) {
    printf("Usage: %s --threads_num <num> --seed <num> --array_size <num>\n", argv[0]);
    return 1;
  }

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t threads[threads_num];

  // Парсинг аргументов
  for (int i = 1; i < argc; i += 2) {
    if (strcmp(argv[i], "--threads_num") == 0) {
      if (ParseArg("threads_num", argv[i + 1], &threads_num) != 0) return 1;
    } else if (strcmp(argv[i], "--seed") == 0) {
      if (ParseArg("seed", argv[i + 1], &seed) != 0) return 1;
    } else if (strcmp(argv[i], "--array_size") == 0) {
      if (ParseArg("array_size", argv[i + 1], &array_size) != 0) return 1;
    } else {
      printf("Unknown argument: %s\n", argv[i]);
      return 1;
    }
  }

  // Проверка аргументов
  if (threads_num > array_size) {
    printf("Error: threads_num cannot be greater than array_size.\n");
    return 1;
  }

  //ЗАПОЛНЯЕМ МАССИВ И ПРОВЕРЯЕМ НА ОШИБКИ
  int *array = malloc(sizeof(int) * array_size);
    if (!array) {
    printf("Error: memory allocation failed.\n");
    return 1;
  }
  GenerateArray(array, array_size, seed);

  struct SumArgs args[threads_num];


  // Определяем диапазоны для потоков
  int chunk_size = array_size / threads_num;
  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = i * chunk_size;
    args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * chunk_size;
  }

  // Засекаем время выполнения
  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL);  // Засекаем время начала


  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  // Засекаем время окончания
  gettimeofday(&end_time, NULL);  // Засекаем время конца
  double elapsed_time = GetElapsedTime(start_time, end_time);
  // Вывод результатов
  printf("Total sum: %d\n", total_sum);
  printf("Elapsed time: %.6f seconds\n", elapsed_time);

  free(array);
  return 0;
}
