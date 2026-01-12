#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dirent.h"
#include "sys/stat.h"
#include "limits.h"

#define MAX_FILES 10000

typedef struct {
    ino_t inode;
    int printed;
} FileInfo;

FileInfo files[MAX_FILES];
int file_count = 0;

void process_directory(char *path, int current_depth, int recmin, int recmax);
int is_file_printed(ino_t inode);
void mark_file_printed(ino_t inode);

int main(int argc, char *argv[])
{
    int recmin, recmax, i;
    
    if (argc < 4)
    {
        printf("Usage: %s recmin recmax dir1 [dir2 ...]\n", argv[0]);
        return 1;
    }
    
    recmin = atoi(argv[1]);
    recmax = atoi(argv[2]);
    
    if (recmin < 0 || recmax < 0 || recmin > recmax)
    {
        fprintf(stderr, "Invalid recmin and(or) recmax values\n");
        return 1;
    }
    
    for (i = 0; i < MAX_FILES; i++) {
        files[i].printed = 0;
    }
    
    for (i = 3; i < argc; i++)
    {
        process_directory(argv[i], 0, recmin, recmax);
    }
    
    return 0;
}

int is_file_printed(ino_t inode) {
    for (int i = 0; i < file_count; i++) {
        if (files[i].inode == inode && files[i].printed) {
            return 1;
        }
    }
    return 0;
}

void mark_file_printed(ino_t inode) {
    for (int i = 0; i < file_count; i++) {
        if (files[i].inode == inode) {
            files[i].printed = 1;
            return;
        }
    }
    
    if (file_count < MAX_FILES) {
        files[file_count].inode = inode;
        files[file_count].printed = 1;
        file_count++;
    }
}

void process_directory(char *path, int current_depth, int recmin, int recmax)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char full_path[PATH_MAX];
    
    if (current_depth > recmax)
    {
        return;
    }
    
    dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "Cannot open directory: %s\n", path);
        return;
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (lstat(full_path, &statbuf) == -1)
        {
            continue;
        }

        if (S_ISREG(statbuf.st_mode))
        {
            if (current_depth >= recmin && current_depth <= recmax && 
                !is_file_printed(statbuf.st_ino))
            {
                char *dot_pos = strrchr(entry->d_name, '.');
                char filename[NAME_MAX];
                char extension[NAME_MAX] = "";
                
                if (dot_pos != NULL && dot_pos != entry->d_name) 
                {
                    size_t name_len = dot_pos - entry->d_name;
                    strncpy(filename, entry->d_name, name_len);
                    filename[name_len] = '\0';
                    
                    strncpy(extension, dot_pos + 1, sizeof(extension) - 1);
                    extension[sizeof(extension) - 1] = '\0';
                }
                else
                {
                    strncpy(filename, entry->d_name, sizeof(filename) - 1);
                    filename[sizeof(filename) - 1] = '\0';
                }
                
                printf("%-20s %-15s %-10lu\n", 
                       filename, 
                       extension, 
                       (unsigned long)statbuf.st_ino);
                
                mark_file_printed(statbuf.st_ino);
            }
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            if (current_depth < recmax)
            {
                process_directory(full_path, current_depth + 1, recmin, recmax);
            }
        }
    }
    
    closedir(dir);
}