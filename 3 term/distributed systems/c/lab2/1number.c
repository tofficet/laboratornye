#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("=== Атрибуты текущего процесса ===\n\n");
    
    printf("1. Идентификатор текущего процесса (PID): %d\n", getpid());

    printf("2. Идентификатор родительского процесса (PPID): %d\n", getppid());
    
    printf("3. Идентификатор группы процесса: %d\n", getpgrp());
    
    printf("4. Реальный идентификатор владельца (RUID): %d\n", getuid());
    
    printf("5. Реальный идентификатор группы владельца (RGID): %d\n", getgid());
    
    printf("6. Эффективный идентификатор владельца (EUID): %d\n", geteuid());
    
    printf("7. Эффективный идентификатор группы владельца (EGID): %d\n", getegid());
    
    printf("Идентификатор сессии процесса: %d\n", getsid(0));
    
    return 0;
}