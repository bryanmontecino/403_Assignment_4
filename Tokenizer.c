#include "Tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Define the maximum size for a lexeme
#define LEXEME_MAX 256

// Helper function to check if a character is a binary operator
_Bool isBinOp(char c) {
    return (c == '+' || c == '*' || c == '!' || c == '=' || c == '%');
}

// Helper function to read and append characters to a buffer until a condition is met
static int readUntilCondition(FILE *inf, char *buffer, int (*condition)(int)) {
    int bufferIndex = 0;
    int ch;

    while ((ch = fgetc(inf)) != EOF && condition(ch)) {
        buffer[bufferIndex++] = ch;
    }
    buffer[bufferIndex] = '\0';

    return bufferIndex;
}

// Helper function to tokenize numbers
static _Bool tokenizeNumber(struct lexics *aLex, int *lexIndex, FILE *inf) {
    char buffer[LEXEME_MAX];
    buffer[0] = '\0';

    int bufferIndex = readUntilCondition(inf, buffer, isdigit);

    // Check if it's a valid number
    if (validNumber(buffer)) {
        aLex[*lexIndex].token = NUMBER;
        strcpy(aLex[*lexIndex].lexeme, buffer);
        (*lexIndex)++;
        return TRUE;
    }

    return FALSE; // Invalid number
}

// Helper function to tokenize identifiers
static _Bool tokenizeIdentifier(struct lexics *aLex, int *lexIndex, FILE *inf) {
    char buffer[LEXEME_MAX];
    buffer[0] = '\0';

    int bufferIndex = readUntilCondition(inf, buffer, isalnum);

    // Check if it's a valid identifier
    if (validIdentifier(buffer)) {
        aLex[*lexIndex].token = IDENTIFIER;
        strcpy(aLex[*lexIndex].lexeme, buffer);
        (*lexIndex)++;
        return TRUE;
    }

    return FALSE; // Invalid identifier
}

// Implement the tokenizer function
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
    int lexIndex = 0;
    int ch;
    enum token currentToken = ERROR_INVALID_TOKEN;

    // Read characters from the file
    while ((ch = fgetc(inf)) != EOF) {
        // Handle whitespace characters
        if (isspace(ch)) {
            continue;
        }
        
        // Handle numeric lexemes
        if (isdigit(ch)) {
            if (!tokenizeNumber(aLex, &lexIndex, inf)) {
                return FALSE; // Invalid number
            }
        }
        // Handle identifiers
        else if (isalpha(ch)) {
            if (!tokenizeIdentifier(aLex, &lexIndex, inf)) {
                return FALSE; // Invalid identifier
            }
        }
        // Handle binary operators and other characters
        else if (isBinOp(ch)) {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = BINOP;
            lexIndex++;
        } else if (ch == '(') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = LEFT_PARENTHESIS;
            lexIndex++;
        } else if (ch == ')') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = RIGHT_PARENTHESIS;
            lexIndex++;
        } else if (ch == '{') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = LEFT_BRACKET;
            lexIndex++;
        } else if (ch == '}') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = RIGHT_BRACKET;
            lexIndex++;
        } else if (ch == ',') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = COMMA;
            lexIndex++;
        } else if (ch == ';') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = EOL;
            lexIndex++;
        } else if (ch == '=') {
            aLex[lexIndex].lexeme[0] = ch;
            aLex[lexIndex].lexeme[1] = '\0';
            aLex[lexIndex].token = EQUAL;
            lexIndex++;
        } else {
            return FALSE; // Invalid character
        }
    }

    // Update the number of lexics
    *numLex = lexIndex;

    // Return TRUE if tokenization is successful
    return TRUE;
}