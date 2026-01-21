#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 101
#define REQUIRED_COUNT 5

volatile sig_atomic_t keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
}

int main() {
    int fifo_fd;
    char buffer[BUFFER_SIZE];
    int lengths[101] = {0}; 
    int found_length = -1;
    
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            return EXIT_FAILURE;
        }
        printf("FIFO уже существует, используем существующий\n");
    } else {
        printf("FIFO создан: %s\n", FIFO_NAME);
    }
    
    printf("Сервер запущен. Ожидание подключения клиента...\n");
    
    
    if ((fifo_fd = open(FIFO_NAME, O_RDONLY)) == -1) {
        perror("open FIFO for reading");
        unlink(FIFO_NAME);
        return EXIT_FAILURE;
    }
    
    printf("Клиент подключен. Ожидание строк...\n");
    printf("Сервер завершит работу при получении %d строк одинаковой длины.\n", REQUIRED_COUNT);
    
    while (keep_running) {
        ssize_t bytes_read = read(fifo_fd, buffer, BUFFER_SIZE - 1);
        
        if (bytes_read == -1) {
            if (errno == EINTR) {
                continue;  
            }
            perror("read from FIFO");
            break;
        } else if (bytes_read == 0) {
            printf("Клиент отключился\n");
            break;
        } else {
            buffer[bytes_read] = '\0';
            
            if (buffer[strlen(buffer) - 1] == '\n') {
                buffer[strlen(buffer) - 1] = '\0';
            }
            
            int len = strlen(buffer);
            
            if (len >= 1 && len <= 100) {
                lengths[len]++;
                printf("Получена строка (длина %d): %s\n", len, buffer);
                printf("Длина %d встретилась %d раз(а)\n", len, lengths[len]);
                
                if (lengths[len] >= REQUIRED_COUNT) {
                    found_length = len;
                    printf("Получено %d строк длины %d. Завершаем работу...\n", 
                           lengths[len], len);
                    break;
                }
            } else {
                printf("Получена строка некорректной длины %d\n", len);
            }
        }
    }
    
    if (close(fifo_fd) == -1) {
        perror("close FIFO");
    }
    
    if (unlink(FIFO_NAME) == -1) {
        perror("unlink FIFO");
    }
    
    if (found_length != -1) {
        printf("Сервер завершил работу: найдено %d строк длины %d\n", 
               lengths[found_length], found_length);
    } else {
        printf("Сервер завершил работу по сигналу\n");
    }
    
    return EXIT_SUCCESS;
}