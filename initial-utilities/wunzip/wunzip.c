#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    if(argc < 2) {
        printf("%s", "wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for(int i = 1; i < argc; i++) {
        FILE* fp = fopen(argv[i], "rb");
        if(fp == NULL) {
            printf("%s", "cannot open this file\n");
            exit(1);
        }

        uint32_t cntLen;
        char c;

        while(fread(&cntLen, sizeof(cntLen), 1, fp) == 1 && fread(&c, sizeof(c), 1, fp) == 1) {
            for(uint32_t j = 0; j < cntLen; j++) {
                putchar(c);
            }
        }
        fclose(fp);
    }
    
    return 0;
}
