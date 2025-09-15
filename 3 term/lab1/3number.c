#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("%s", argv[0]);
        return 1;
    }

    char *filename1 = argv[1];
    char *filename2 = argv[2];
    char buffer[1024];
    size_t bytes_read;

    FILE *f1 = fopen(filename1, "rb");   
    if (!f1) {
        return 1;
    }

    FILE *f2 = fopen(filename2, "wb"); 
    if (!f2) {
        fclose(f1);
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), f1)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, f2);
        if (bytes_written != bytes_read) {
            fclose(f1);
            fclose(f2);
            return 1;
        }
    }

    fclose(f1);
    fclose(f2);

    return 0;
}