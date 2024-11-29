#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

// Структура для передачи аргументов в поток
struct ThreadArgs {
    int start;
    int end;
    int mod;
    unsigned long long *result;
    pthread_mutex_t *mutex;
};

// Функция для вычисления факториала части диапазона
void *factorial_part(void *args) {
    struct ThreadArgs *targs = (struct ThreadArgs *)args;
    unsigned long long partial_result = 1;
    
    // Вычисляем произведение для диапазона
    for (int i = targs->start; i <= targs->end; i++) {
        partial_result = (partial_result * i) % targs->mod;
    }

    // Синхронизируем доступ к общей переменной
    pthread_mutex_lock(targs->mutex);
    *targs->result = (*targs->result * partial_result) % targs->mod;
    pthread_mutex_unlock(targs->mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    int k = 0;     // Число, факториал которого нужно вычислить
    int pnum = 1;  // Количество потоков
    int mod = 1;   // Модуль

    // Парсинг аргументов
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0) {
            k = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--pnum") == 0) {
            pnum = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--mod") == 0) {
            mod = atoi(argv[++i]);
        }
    }

    if (k <= 0 || pnum <= 0 || mod <= 0) {
        printf("Invalid arguments.\n");
        return 1;
    }

    // Разделим диапазон чисел на pnum частей
    int range_size = k / pnum;
    pthread_t threads[pnum];
    unsigned long long result = 1;  // Для хранения итогового результата
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // Создаём потоки
    for (int i = 0; i < pnum; i++) {
        struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
        args->start = i * range_size + 1;
        args->end = (i == pnum - 1) ? k : (i + 1) * range_size;
        args->mod = mod;
        args->result = &result;
        args->mutex = &mutex;

        if (pthread_create(&threads[i], NULL, factorial_part, (void *)args) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Ожидаем завершения всех потоков
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    // Выводим результат
    printf("Factorial of %d modulo %d is: %llu\n", k, mod, result);

    return 0;
}
