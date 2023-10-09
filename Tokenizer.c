/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 8, 2023

Purpose of Assignment:
- Take a text file as input
- Tokenize the given input
- Parse the tokenized input to determine if it is grammatically valid 
*/

#include "Givens.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INVALID 14

// Verify if a character is a letter, digit, or underscore
int isAlphanumeric(char c) {
    return isalnum(c) || c == '_';
}

// Tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
    char c;
    int lexemeIndex;
    struct lexics token_tracker;

    *numLex = 0;

    while ((c = fgetc(inf)) != EOF) {
        lexemeIndex = 0;

        // Skip whitespace, along with not worrying about comments
        while (isspace(c) || c == '/') {
            if (c == '/') {
                if (fgetc(inf) == '/') {
                    // Skip until the end of the line
                    while ((c = fgetc(inf)) != '\n' && c != EOF);
                } else {
                    ungetc('/', inf);
                    break;
                }
            }
            c = fgetc(inf);
        }

        if (c == EOF) {
            break;
        }

        // Look for single character tokens from the input
        if (c == '(') {
            token_tracker.token = LEFT_PARENTHESIS;
            strcpy(token_tracker.lexeme, "(");
        } else if (c == ')') {
            token_tracker.token = RIGHT_PARENTHESIS;
            strcpy(token_tracker.lexeme, ")");
        } else if (c == '{') {
            token_tracker.token = LEFT_BRACKET;
            strcpy(token_tracker.lexeme, "{");
        } else if (c == '}') {
            token_tracker.token = RIGHT_BRACKET;
            strcpy(token_tracker.lexeme, "}");
        } else if (c == '=') {
            // Is the next character also '='?
            if (fgetc(inf) == '=') {
                token_tracker.token = BINOP;
                strcpy(token_tracker.lexeme, "==");
            } else {
                token_tracker.token = EQUAL;
                strcpy(token_tracker.lexeme, "=");
                // Push back the last character, which is not part of the lexeme.
                ungetc(fgetc(inf), inf);
            }
        } else if (c == ',') {
            token_tracker.token = COMMA;
            strcpy(token_tracker.lexeme, ",");
        } else if (c == ';') {
            token_tracker.token = EOL;
            strcpy(token_tracker.lexeme, ";");
        } else if (isspace(c)) {
            // Handle whitespace (no specific token, skip it).
            continue;
        } else {
            // Check for operators
            lexemeIndex = 0;
            if (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(inf) == '=')) {
                token_tracker.lexeme[lexemeIndex++] = c;
                if (c == '!' && lexemeIndex == 1) {
                    // This is the "!=" operator.
                    token_tracker.lexeme[lexemeIndex++] = '=';
                }
                token_tracker.lexeme[lexemeIndex] = '\0';
                token_tracker.token = BINOP;
            } else if (isdigit(c)) {
                // Check for numbers
                while (isdigit(c)) {
                    token_tracker.lexeme[lexemeIndex++] = c;
                    c = fgetc(inf);
                }
                token_tracker.lexeme[lexemeIndex] = '\0';
                ungetc(c, inf);
                token_tracker.token = NUMBER;
            } else if (isalpha(c)) {
                // Check with the input for keywords or identifiers
                while (isAlphanumeric(c)) {
                    token_tracker.lexeme[lexemeIndex++] = c;
                    c = fgetc(inf);
                }
                token_tracker.lexeme[lexemeIndex] = '\0';
                ungetc(c, inf);
                if (strcmp(token_tracker.lexeme, "while") == 0) {
                    token_tracker.token = WHILE_KEYWORD;
                } else if (strcmp(token_tracker.lexeme, "return") == 0) {
                    token_tracker.token = RETURN_KEYWORD;
                } else if (strcmp(token_tracker.lexeme, "int") == 0 || strcmp(token_tracker.lexeme, "void") == 0) {
                    token_tracker.token = VARTYPE;
                } else {
                    token_tracker.token = IDENTIFIER;
                }
            } else {
                // If none of the above cases match, it's an invalid character
                token_tracker.token = INVALID;
                sprintf(token_tracker.lexeme, "Invalid character: %c", c);
            }
        }

        // Store the token in the array
        if (*numLex < LEXEME_MAX) {
            aLex[*numLex] = token_tracker;
            (*numLex)++; // Increment the number of lexemes
        } else {
            printf("Maximum number of lexemes reached.\n");
            return 0;
        }
    }
    return 1;
}