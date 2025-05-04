#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    if(argc == 1) {
        printf("%s", "wgrep: searchterm [file ...]\n");
        exit(1);
    }

    char* line = NULL;
    size_t len = 0;

    if(argc == 2) {
        while(getline(&line, &len, stdin) != -1) {
            if(strstr(line, argv[1]) != NULL) {
                printf("%s", line);
            }
        }
    }
    else {
        for(int i = 2; i < argc; i++) {
            // printf("%s\n", argv[i]);
            FILE* fp = fopen(argv[i], "r");
            if(fp == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }
    
            while(getline(&line, &len, fp) != -1) {
                if(strstr(line, argv[1]) != NULL) {
                    printf("%s", line);
                }
            }
        }
    }
    
    return 0;
}