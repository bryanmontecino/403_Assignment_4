#include "Tokenizer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to tokenize the input from a FILE
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
    // Initialize variables to keep track of the lexeme and its length
    char lexeme[LEXEME_MAX];
    int lexemeLength = 0;

    // Initialize variables to keep track of the current character and the token
    char currentChar;
    enum token currentToken;

    // Initialize the number of tokens to zero
    *numLex = 0;

    // Flag to indicate if we are inside a function declaration
    _Bool insideFunctionDeclaration = FALSE;

    // Loop through characters in the input file
    while ((currentChar = fgetc(inf)) != EOF) {
        // Check if the character is whitespace
        if (isspace(currentChar)) {
            // If the lexeme is not empty, tokenize it
            if (lexemeLength > 0) {
                lexeme[lexemeLength] = '\0'; // Null-terminate the lexeme
                // Determine the token for the lexeme
                if (validNumber(lexeme)) {
                    currentToken = NUMBER;
                } else if (validIdentifier(lexeme)) {
                    currentToken = IDENTIFIER;
                } else {
                    // Handle error for an invalid lexeme
                    printf("ERROR: Invalid lexeme '%s'\n", lexeme);
                    return FALSE;
                }
                // Create a lexics struct for the token and lexeme
                aLex[*numLex].token = currentToken;
                strcpy(aLex[*numLex].lexeme, lexeme);
                (*numLex)++; // Increment the number of tokens
                lexemeLength = 0; // Reset the lexeme
            }
        } else {
            // Add the character to the current lexeme
            lexeme[lexemeLength++] = currentChar;

            // Check if we are inside a function declaration
            if (currentChar == '(' && lexemeLength == 1) {
                insideFunctionDeclaration = TRUE;
            } else if (currentChar == ')' && insideFunctionDeclaration) {
                insideFunctionDeclaration = FALSE;
            }

            // If we are inside a function declaration, treat everything as part of it
            if (insideFunctionDeclaration) {
                currentToken = IDENTIFIER; // Function name or parameter name
            }
        }
    }

    // Tokenize the last lexeme if it's not empty
    if (lexemeLength > 0) {
        lexeme[lexemeLength] = '\0';
        if (validNumber(lexeme)) {
            currentToken = NUMBER;
        } else if (validIdentifier(lexeme)) {
            currentToken = IDENTIFIER;
        } else {
            // Handle error for an invalid lexeme
            printf("ERROR: Invalid lexeme '%s'\n", lexeme);
            return FALSE;
        }
        aLex[*numLex].token = currentToken;
        strcpy(aLex[*numLex].lexeme, lexeme);
        (*numLex)++;
    }

    // Tokenization successful
    return TRUE;
}