#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    for(int i = 1; i < argc; i++) {
        // printf("%s\n", argv[i]);

        FILE* fp = fopen(argv[i], "r");
        if(fp == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        char* line = NULL;
        size_t len = 0;
        while(getline(&line, &len, fp) != -1) {
            printf("%s", line);
        }
        free(line);
        fclose(fp);
    }
    
    exit(0);
}