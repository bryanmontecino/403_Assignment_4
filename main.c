#include <stdio.h>
#include <stdlib.h>
#include "Tokenizer.h"
#include "Givens.h"

int main() {
    // Initialize file pointer to NULL
    FILE *infile = NULL;

    // Define a character array to store the name of the file to read and write
    char filename[MY_CHAR_MAX];

    // Prompt the user to input a filename and continue to prompt until they enter a correct one
    while (infile == NULL) {
        printf("Enter filename: ");
        scanf("%s", filename);

        // When given a filename, use fopen to create a new file pointer.
        // If fopen cannot find the file, it returns null
        infile = fopen(filename, "r");
        if (infile == NULL) {
            printf("ERROR: File %s cannot be opened\n", filename);
        }
    }

    // Define an array to store lexics and initialize the number of lexics
    struct lexics aLex[MY_CHAR_MAX];
    int numLex = 0;

    // Tokenize the input file
    if (tokenizer(aLex, &numLex, infile)) {
        // Print the tokens and lexemes
        printf("Tokens:\n");
        for (int i = 0; i < numLex; i++) {
            printf("%d: %s\n", aLex[i].token, aLex[i].lexeme);
        }
    } else {
        printf("Tokenization failed.\n");
    }

    // Close the input file
    fclose(infile);

    return 0;
}

