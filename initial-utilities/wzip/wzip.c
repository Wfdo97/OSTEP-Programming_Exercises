#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    if(argc == 1) {
        printf("%s", "wzip: file1 [file2 ...]\n");
        exit(1);
    }
    
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    
    char* comb = NULL;
    size_t resCap = 0;
    size_t resLen = 0;

    if(argc > 2) {
        for(int i = 1; i < argc; i++) {
            FILE* fp = fopen(argv[i], "r");
            if(fp == NULL) {
                printf("file cannot be open\n");
                exit(1);
            }

            while((nread = getline(&line, &len, fp)) != -1) {

                if(nread > 0 && line[nread - 1] == '\n') line[--nread] = '\0';

                if(resLen + nread + 1 > resCap) {
                    resCap = (resLen + nread + 1) * 2;
                    char* tmp = realloc(comb, resCap);

                    if(tmp == NULL) {
                        printf("malloc didn't work properly\n");
                        free(comb);
                        free(line);
                        exit(1);
                    }
                    comb = tmp;
                }
                memcpy(comb + resLen, line, nread);
                resLen += nread;
                comb[resLen] = '\n';
            }
            fclose(fp);
        }

        char temp = comb[0];
        uint32_t count = 1;
        for(int i = 1; i < resLen; i++) {
            
            if(comb[i] == temp) {
                count++;
            }
            else {
                fwrite(&count, sizeof(count), 1, stdout);
                fwrite(&temp, sizeof(temp), 1, stdout);
                temp = comb[i];
                count = 1;
            }
        }
        fwrite(&count, sizeof(count), 1, stdout);
        fwrite(&temp, sizeof(temp), 1, stdout);

        free(comb);
    }
    else if(argc == 2) {
        FILE* fp = fopen(argv[1], "r");
        if(fp == NULL) {
            printf("file cannot be open\n");
            exit(1);
        }

        char temp;
        uint32_t count = 1;
        int first = 1;

        while((nread = getline(&line, &len, fp)) != -1) {
            
            for(ssize_t i = 0; i < nread; i++) {
                char c = line[i];

                if(first) {
                    temp = c;
                    count = 1;
                    first = 0;
                }
                else if(c == temp) {
                    count++;
                }
                else {
                    fwrite(&count, sizeof(count), 1, stdout);
                    fwrite(&temp, sizeof(temp), 1, stdout);
                    temp = c;
                    count = 1;
                }
            }
        }
        if(!first) {
            fwrite(&count, sizeof(count), 1, stdout);
            fwrite(&temp, sizeof(temp), 1, stdout);
        }
        fclose(fp);
    }
    
    free(line);
    return 0;
}