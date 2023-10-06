#include "Tokenizer.h"
#include "Givens.h"
#include <stdio.h>
#include <string.h>

// Helper function to add a token to the lexics array
static void addToken(struct lexics *aLex, int *numLex, enum token token, const char *lexeme) {
    aLex[*numLex].token = token;
    strcpy(aLex[*numLex].lexeme, lexeme);
    (*numLex)++;
}

_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
    char currentChar;
    int lexemeIndex = 0;
    int tokenIndex = 0;
    _Bool inLexeme = FALSE;
    char lexeme[LEXEME_MAX];

    // Define character lexemes
    const char* characterLexemes = "();{}";

    while ((currentChar = fgetc(inf)) != EOF) {
        if (strchr(characterLexemes, currentChar) != NULL) {
            // If it's a character lexeme, treat it as a separator
            if (inLexeme) {
                lexeme[lexemeIndex] = '\0'; // Null-terminate the lexeme
                inLexeme = FALSE;
                // Tokenize the lexeme and add it to aLex
                if (validNumber(lexeme)) {
                    addToken(aLex, &tokenIndex, NUMBER, lexeme);
                } else if (validIdentifier(lexeme)) {
                    addToken(aLex, &tokenIndex, IDENTIFIER, lexeme);
                } else {
                    // Handle other cases as needed
                }
            }
            // Handle the character lexeme
            char charLexeme[2] = {currentChar, '\0'};
            addToken(aLex, &tokenIndex, BINOP, charLexeme);
        } else if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n') {
            // If it's a space, tab, or newline, treat it as a separator
            if (inLexeme) {
                lexeme[lexemeIndex] = '\0'; // Null-terminate the lexeme
                inLexeme = FALSE;
                // Tokenize the lexeme and add it to aLex
                if (validNumber(lexeme)) {
                    addToken(aLex, &tokenIndex, NUMBER, lexeme);
                } else if (validIdentifier(lexeme)) {
                    addToken(aLex, &tokenIndex, IDENTIFIER, lexeme);
                } else {
                    // Handle other cases as needed
                }
            }
        } else {
            // Handle non-space characters
            lexeme[lexemeIndex++] = currentChar;
            inLexeme = TRUE;
        }
    }

    // Check if there is a lexeme remaining
    if (inLexeme) {
        lexeme[lexemeIndex] = '\0'; // Null-terminate the lexeme
        // Tokenize the lexeme and add it to aLex
        if (validNumber(lexeme)) {
            addToken(aLex, &tokenIndex, NUMBER, lexeme);
        } else if (validIdentifier(lexeme)) {
            addToken(aLex, &tokenIndex, IDENTIFIER, lexeme);
        } else {
            // Handle other cases as needed
        }
    }

    *numLex = tokenIndex; // Update the number of tokens
    return TRUE; // Return TRUE upon successful tokenization
}