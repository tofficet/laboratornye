#include <stdio.h>

int main(int argc, char* argv[]) {

    unsigned char b;
    size_t bytes_read;
    int iteration = 0;
    unsigned char buffer[4];
    unsigned char bytes_to_write[11] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    if (argc != 2) {
    return -1;
    }
    
    FILE *file = fopen(argv[1], "wb");
    if (file == NULL) {
        return -2;
    }
    
    fwrite(bytes_to_write, sizeof(unsigned char), 11, file);
    fclose(file);
    
    file = fopen(argv[1], "rb");
    if (file == NULL) {
        return -2;
    }
    
    while (!feof(file)) {
        bytes_read = fread(&b, sizeof(unsigned char), 1, file);
        if (bytes_read != 1) {
            break;
        }
        printf("Byte = %d\n", b);
        printf("  Current position: %ld\n", ftell(file));
        printf("  End-of-file indicator: %d\n", feof(file));
        printf("  Error indicator: %d\n", ferror(file));
    }
    
    fclose(file);
    
    file = fopen(argv[1], "rb");
    if (file == NULL) {
        return -2;
    }
    
    if (fseek(file, sizeof(unsigned char) * 3, SEEK_SET) != 0) {
        fclose(file);
        return -4;
    }

    bytes_read = fread(buffer, sizeof(unsigned char), 4, file);

    fclose(file);
    
    return 0;
}

