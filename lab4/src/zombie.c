#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Создание дочернего процесса

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс
        printf("Child process started with PID: %d\n", getpid());
        exit(0);  // Завершаем дочерний процесс
    } else {
        // Родительский процесс
        printf("Parent process with PID: %d\n", getpid());
        printf("Child process created with PID: %d\n", pid);

        // Пауза, чтобы показать зомби-процесс
        sleep(5);  
        system("ps aux | grep Z");

        // Удаление записи зомби-процесса из таблицы процессов
        int status;
        wait(&status);
        printf("Zombie process cleaned up.\n");
        system("ps aux | grep Z");
    }

    return 0;
}
