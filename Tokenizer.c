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

// Function to check if a character is a valid alphanumeric character
int isAlphanumeric(char c) {
    return isalnum(c) || c == '_';
}

// Function to tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
    char c;
    int lexemeIndex;
    struct lexics token_tracker;

    *numLex = 0; // Initialize the number of lexics to 0

    while ((c = fgetc(inf)) != EOF) {
        lexemeIndex = 0;

        // Skip whitespace and comments
        while (isspace(c) || c == '/') {
            if (c == '/') {
                if (fgetc(inf) == '/') {
                    // Single-line comment, skip until EOL
                    while ((c = fgetc(inf)) != '\n' && c != EOF);
                } else {
                    // Not a comment, push back the character
                    ungetc('/', inf);
                    break;
                }
            }
            c = fgetc(inf);
        }

        if (c == EOF) {
            break;
        }

        // Check for single-character tokens
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
            // Check if the next character is also '='
            if (fgetc(inf) == '=') {
                token_tracker.token = BINOP;
                strcpy(token_tracker.lexeme, "==");
            } else {
                token_tracker.token = EQUAL;
                strcpy(token_tracker.lexeme, "=");
                // Push back the last character, which is not part of the lexeme
                ungetc(fgetc(inf), inf);
            }
        } else if (c == ',') {
            token_tracker.token = COMMA;
            strcpy(token_tracker.lexeme, ",");
        } else if (c == ';') {
            token_tracker.token = EOL;
            strcpy(token_tracker.lexeme, ";");
        } else if (isspace(c)) { // Handle whitespace
            token_tracker.token = WHITESPACE;
            token_tracker.lexeme[0] = c;
            token_tracker.lexeme[1] = '\0';
        } else {
            // Check for operators
            lexemeIndex = 0;
            if (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(inf) == '=')) {
                token_tracker.lexeme[lexemeIndex++] = c;
                if (c == '!' && lexemeIndex == 1) {
                    // != operator
                    token_tracker.lexeme[lexemeIndex++] = '=';
                }
                token_tracker.lexeme[lexemeIndex] = '\0';
                token_tracker.token = BINOP;
            }
            // Check for numbers
            else if (isdigit(c)) {
                while (isdigit(c)) {
                    token_tracker.lexeme[lexemeIndex++] = c;
                    c = fgetc(inf);
                }
                token_tracker.lexeme[lexemeIndex] = '\0';
                ungetc(c, inf); // Push back the last non-digit character
                token_tracker.token = NUMBER;
            }
            // Check for keywords or identifiers
            else if (isalpha(c)) {
                while (isAlphanumeric(c)) {
                    token_tracker.lexeme[lexemeIndex++] = c;
                    c = fgetc(inf);
                }
                token_tracker.lexeme[lexemeIndex] = '\0';
                ungetc(c, inf); // Push back the last non-alphanumeric character
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
        if (*numLex < 1000) {
            aLex[*numLex] = token_tracker; // Copy the token_tracker into the array
            (*numLex)++; // Increment the number of lexemes
        } else {
            printf("Maximum number of lexemes reached.\n");
            return 0;
        }
    }
    return 1;
}