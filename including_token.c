/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 8, 2023
*/

#include "including_token.h"
#include "Givens.h"
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

        /*
        // Check if the lexeme size exceeds MY_CHAR_MAX
        if (lexemeIndex >= MY_CHAR_MAX) {
            printf("Lexeme size exceeds MY_CHAR_MAX, cannot tokenize further.\n");
            return 0;
        }
        */

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
                while (lexemeIndex < LEXEME_MAX - 1 && (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(inf) == '='))) {
                    token_tracker.lexeme[lexemeIndex++] = c;
                    c = fgetc(inf);
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

const char* tokenEnumToString(enum token aToken) {
	if(aToken == LEFT_PARENTHESIS){
		return "LEFT_PARENTHESIS";
	}
	else if(aToken == RIGHT_PARENTHESIS){
		return "RIGHT_PARENTHESIS";
	}
	else if(aToken == LEFT_BRACKET){
		return "LEFT_BRACKET";
	}
	else if(aToken == RIGHT_BRACKET){
		return "RIGHT_BRACKET";
	}
	else if(aToken == WHILE_KEYWORD){
		return "WHILE_KEYWORD";
	}
	else if(aToken == RETURN_KEYWORD){
		return "RETURN_KEYWORD";
	}
	else if(aToken == EQUAL){
		return "EQUAL";
	}
	else if(aToken == COMMA){
		return "COMMA";
	}
	else if(aToken == EOL){
		return "EOL";
	}
	else if(aToken == VARTYPE){
		return "VARTYPE";
	}
	else if(aToken == IDENTIFIER){
		return "IDENTIFIER";
	}
	else if(aToken == BINOP){
		return "BINOP";
	}
	else if(aToken == WHITESPACE){
		return "WHITESPACE";
	}
	else if(aToken == NUMBER){
		return "NUMBER";
	}
	else{
		return "ERROR-INVALID_TOKEN";
	}
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct lexics allLexics[1000]; // Declare an array of struct lexics
    int numberOfLexics = 0;
    _Bool success = tokenizer(allLexics, &numberOfLexics, file); // Pass the array

    printf("Did tokenize: %d\n", success);
    printf("Number of lexemes: %d\n", numberOfLexics);
    printf("Lexemes: ");
    for (int i = 0; i < numberOfLexics; i++) {
        printf("%s  ", allLexics[i].lexeme); // Access members correctly
    }
    printf("\nTokens: ");
    for (int i = 0; i < numberOfLexics; i++) {
        printf("%s  ", tokenEnumToString(allLexics[i].token)); // Access members correctly
    }

    printf("\n");

    fclose(file);

    return 0;
}