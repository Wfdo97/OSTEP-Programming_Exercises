#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    
    

    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    
    size_t arrcap = 16;
    size_t count = 0;
    char** string = malloc(arrcap * sizeof(char*));
    if(string == NULL) {
        printf("malloc didn't work properly");
        exit(1);
    }

    for(int i = 1; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");
        if(fp == NULL) {
            printf("file cannot be open\n");
            exit(1);
        }

        while((nread = getline(&line, &len, fp1)) != -1) {
            
            if(count >= arrcap) {
                arrcap *= 2;
                char** temp = realloc(string, arrcap * sizeof(char*));
                if(temp == NULL) {
                    printf("malloc didn't work properly");
                    exit(1);
                }
                string = temp;
            }

            char* copy = malloc(nread + 1);
            if(copy == NULL) {
                printf("malloc didn't work properly");
                exit(1);
            }

            memcpy(copy, string, nread + 1);
            string[count++] = copy;
        }
        fclose(fp);
    }

    
    char temp = string[0];
    uint32_t count = 1;
    for(int i = 1; i < sizeof(string); i++) {

        if(string[i] == temp) {
            count++;
        }
        else {
            fwrite(&count, sizeof(count), 1, stdout);
            fwrite(&temp, sizeof(temp), 1, stdout);
            temp = line[i];
            count = 1;
        }
    }
    fwrite(&count, sizeof(count), 1, stdout);
    fwrite(&temp, sizeof(temp), 1, stdout);

    free(string);
    // fclose(fp1);
    return 0;
}