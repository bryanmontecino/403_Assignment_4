#include <stdio.h>
#include "Tokenizer.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *inf = fopen(argv[1], "r");
    if (inf == NULL) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }

    struct lexics aLex[100];  // Adjust the array size as needed
    int numLex;

    if (tokenizer(aLex, &numLex, inf)) {
        for (int i = 0; i < numLex; i++) {
            printf("Token: %d, Lexeme: %s\n", aLex[i].token, aLex[i].lexeme);
        }
    } else {
        fprintf(stderr, "Tokenization failed.\n");
    }

    fclose(inf);
    return 0;
}