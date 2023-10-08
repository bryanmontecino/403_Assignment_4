/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 7, 2023
*/

#include "Tokenizer.h"
#include "Givens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INVALID -1

// Structure to represent a token
typedef struct {
    enum token type; // Use the enum from Givens.h for token types
    char lexeme[LEXEME_MAX]; // Adjust the size as needed
} Token;

// Function to check if a character is a valid alphanumeric character
int isAlphanumeric(char c) {
    return isalnum(c) || c == '_';
}

// Function to tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *file) {
    char c;
    char lexeme[LEXEME_MAX];
    int lexemeIndex = 0;

    // Skip whitespace and comments
    while (1) {
        c = fgetc(file);

        // Skip whitespace
        while (isspace(c)) {
            c = fgetc(file);
        }

        // Handle comments
        if (c == '/') {
            c = fgetc(file);
            if (c == '/') {
                // Single-line comment, skip until EOL
                while ((c = fgetc(file)) != '\n' && c != EOF);
                continue;
            } else {
                // Not a comment, push back the character
                ungetc('/', file);
            }
        }
        break;
    }

    if (c == EOF) {
        aLex[*numLex].token = INVALID;
        strcpy(aLex[*numLex].lexeme, "EOF");
        (*numLex)++;
        return FALSE;
    }

    // Check for single-character tokens
    switch (c) {
        case '(':
            aLex[*numLex].token = LEFT_PARENTHESIS;
            strcpy(aLex[*numLex].lexeme, "(");
            (*numLex)++;
            return TRUE;
        case ')':
            aLex[*numLex].token = RIGHT_PARENTHESIS;
            strcpy(aLex[*numLex].lexeme, ")");
            (*numLex)++;
            return TRUE;
        case '{':
            aLex[*numLex].token = LEFT_BRACKET;
            strcpy(aLex[*numLex].lexeme, "{");
            (*numLex)++;
            return TRUE;
        case '}':
            aLex[*numLex].token = RIGHT_BRACKET;
            strcpy(aLex[*numLex].lexeme, "}");
            (*numLex)++;
            return TRUE;
        case '=':
            aLex[*numLex].token = EQUAL;
            strcpy(aLex[*numLex].lexeme, "=");
            (*numLex)++;
            return TRUE;
        case ',':
            aLex[*numLex].token = COMMA;
            strcpy(aLex[*numLex].lexeme, ",");
            (*numLex)++;
            return TRUE;
        case ';':
            aLex[*numLex].token = EOL;
            strcpy(aLex[*numLex].lexeme, ";");
            (*numLex)++;
            return TRUE;
    }

    // Check for operators
    if (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(file) == '=')) {
        lexeme[lexemeIndex++] = c;
        if (c == '!' && lexemeIndex == 1) {
            // != operator
            lexeme[lexemeIndex++] = '=';
        }
        lexeme[lexemeIndex] = '\0';
        aLex[*numLex].token = BINOP;
        strcpy(aLex[*numLex].lexeme, lexeme);
        (*numLex)++;
        return TRUE;
    }

    // Check for numbers
    if (isdigit(c)) {
        while (isdigit(c)) {
            lexeme[lexemeIndex++] = c;
            c = fgetc(file);
        }
        lexeme[lexemeIndex] = '\0';
        ungetc(c, file); // Push back the last non-digit character
        if (validNumber(lexeme)) {
            aLex[*numLex].token = NUMBER;
            strcpy(aLex[*numLex].lexeme, lexeme);
            (*numLex)++;
            return TRUE;
        } else {
            aLex[*numLex].token = INVALID;
            sprintf(aLex[*numLex].lexeme, "Invalid number: %s", lexeme);
            (*numLex)++;
            return FALSE;
        }
    }

    // Check for keywords or identifiers
    if (isalpha(c)) {
        while (isAlphanumeric(c)) {
            lexeme[lexemeIndex++] = c;
            c = fgetc(file);
        }
        lexeme[lexemeIndex] = '\0';
        ungetc(c, file); // Push back the last non-alphanumeric character
        if (validIdentifier(lexeme)) {
            if (strcmp(lexeme, "while") == 0) {
                aLex[*numLex].token = WHILE_KEYWORD;
                strcpy(aLex[*numLex].lexeme, "while");
            } else if (strcmp(lexeme, "return") == 0) {
                aLex[*numLex].token = RETURN_KEYWORD;
                strcpy(aLex[*numLex].lexeme, "return");
            } else if (strcmp(lexeme, "int") == 0 || strcmp(lexeme, "void") == 0) {
                aLex[*numLex].token = VARTYPE;
                strcpy(aLex[*numLex].lexeme, lexeme);
            } else {
                aLex[*numLex].token = IDENTIFIER;
                strcpy(aLex[*numLex].lexeme, lexeme);
            }
            (*numLex)++;
            return TRUE;
        } else {
            aLex[*numLex].token = INVALID;
            sprintf(aLex[*numLex].lexeme, "Invalid identifier: %s", lexeme);
            (*numLex)++;
            return FALSE;
        }
    }

    // If none of the above cases match, it's an invalid character
    aLex[*numLex].token = INVALID;
    sprintf(aLex[*numLex].lexeme, "Invalid character: %c", c);
    (*numLex)++;
    return FALSE;
}