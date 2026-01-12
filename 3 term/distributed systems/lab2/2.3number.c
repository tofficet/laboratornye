#include <stdio.h>
#include <unistd.h>

int main(){
    if (fork() || fork()) fork();
    printf("forked! %d", fork());
    return 0;
}