#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t pid;
    char write_buffer[] = "Hello from parent process!";
    char read_buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");

        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        
        close(pipe_fd[1]);
        
        bytes_read = read(pipe_fd[0], read_buffer, BUFFER_SIZE - 1);
        if (bytes_read == -1) {
            perror("read");
            close(pipe_fd[0]);
            exit(EXIT_FAILURE);
        }
        
        read_buffer[bytes_read] = '\0';

        printf("Дочерний процесс получил: %s\n", read_buffer);
        
        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
        
    } else {

        close(pipe_fd[0]);
        

        ssize_t bytes_written = write(pipe_fd[1], write_buffer, strlen(write_buffer));
        if (bytes_written == -1) {
            perror("write");
            close(pipe_fd[1]);
            exit(EXIT_FAILURE);
        }
        
        printf("Родительский процесс отправил: %s\n", write_buffer);
        
        close(pipe_fd[1]);
        
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
        }
    }
    
    return EXIT_SUCCESS;
}