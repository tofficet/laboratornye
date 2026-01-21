#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#define FIFO_NAME "/tmp/my_fifo"
#define MIN_LENGTH 1
#define MAX_LENGTH 100
#define MAX_BUFFER 101

volatile sig_atomic_t keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
}

void generate_random_string(char *buffer, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charset_size = sizeof(charset) - 1;
    
    for (int i = 0; i < length; i++) {
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[length] = '\0';
}

int main() {
    int fifo_fd;
    char buffer[MAX_BUFFER];

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    
    srand(time(NULL) ^ getpid());
    
    if (access(FIFO_NAME, F_OK) == -1) {
        fprintf(stderr, "FIFO не существует. Запустите сначала сервер.\n");
        return EXIT_FAILURE;
    }
    
    printf("Клиент запущен. Подключение к серверу...\n");
    
    if ((fifo_fd = open(FIFO_NAME, O_WRONLY)) == -1) {
        perror("open FIFO for writing");
        return EXIT_FAILURE;
    }
    
    printf("Подключение к серверу установлено. Отправка строк...\n");
    
    while (keep_running) {
        int length = MIN_LENGTH + rand() % (MAX_LENGTH - MIN_LENGTH + 1);
        
        generate_random_string(buffer, length);
        
        int str_len = strlen(buffer);
        if (str_len < MAX_BUFFER - 1) {
            buffer[str_len] = '\n';
            buffer[str_len + 1] = '\0';
        }
        
        ssize_t bytes_written = write(fifo_fd, buffer, strlen(buffer));
        
        if (bytes_written == -1) {
            if (errno == EPIPE) {
                printf("Сервер закрыл соединение\n");
                break;
            } else if (errno == EINTR) {
                continue;
            }
            perror("write to FIFO");
            break;
        }
        
        printf("Отправлена строка (длина %d): %s", length, buffer);
        
    }
    
    if (close(fifo_fd) == -1) {
        perror("close FIFO");
    }
    
    printf("Клиент завершил работу\n");
    
    return EXIT_SUCCESS;
}