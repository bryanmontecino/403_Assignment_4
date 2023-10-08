#include "Tokenizer.h"
#include "Givens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define INVALID 13

// Function to check if a character is a valid alphanumeric character
int isAlphanumeric(char c) {
    return isalnum(c) || c == '_';
}

// Function to tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf) {
    char c;
    char lexeme[LEXEME_MAX]; // Use LEXEME_MAX from Givens.h
    int lexemeIndex = 0;

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
        aLex[*numLex].token = INVALID;
        strcpy(aLex[*numLex].lexeme, "EOF");
        (*numLex)++;
        return 0;
    }

    // Check for single-character tokens
    if (c == '(') {
        aLex[*numLex].token = LEFT_PARENTHESIS;
        strcpy(aLex[*numLex].lexeme, "(");
        (*numLex)++;
        return 1;
    } else if (c == ')') {
        aLex[*numLex].token = RIGHT_PARENTHESIS;
        strcpy(aLex[*numLex].lexeme, ")");
        (*numLex)++;
        return 1;
    } else if (c == '{') {
        aLex[*numLex].token = LEFT_BRACKET;
        strcpy(aLex[*numLex].lexeme, "{");
        (*numLex)++;
        return 1;
    } else if (c == '}') {
        aLex[*numLex].token = RIGHT_BRACKET;
        strcpy(aLex[*numLex].lexeme, "}");
        (*numLex)++;
        return 1;
    } else if (c == '=') {
        aLex[*numLex].token = EQUAL;
        strcpy(aLex[*numLex].lexeme, "=");
        (*numLex)++;
        return 1;
    } else if (c == ',') {
        aLex[*numLex].token = COMMA;
        strcpy(aLex[*numLex].lexeme, ",");
        (*numLex)++;
        return 1;
    } else if (c == ';') {
        aLex[*numLex].token = EOL;
        strcpy(aLex[*numLex].lexeme, ";");
        (*numLex)++;
        return 1;
    }

    // Check for operators
    if (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(inf) == '=')) {
        lexeme[lexemeIndex++] = c;
        if (c == '!' && lexemeIndex == 1) {
            // != operator
            lexeme[lexemeIndex++] = '=';
        }
        lexeme[lexemeIndex] = '\0';
        aLex[*numLex].token = BINOP;
        strcpy(aLex[*numLex].lexeme, lexeme);
        (*numLex)++;
        return 1;
    }

    // Check for numbers using validNumber function
    if (isdigit(c) || (c == '-' && isdigit(fgetc(inf)))) {
        lexeme[lexemeIndex++] = c;
        while (isdigit(c) || c == '-') {
            c = fgetc(inf);
            lexeme[lexemeIndex++] = c;
        }
        lexeme[lexemeIndex - 1] = '\0'; // Remove the last character
        ungetc(c, inf); // Push back the last non-digit character
        if (validNumber(lexeme)) {
            aLex[*numLex].token = NUMBER;
            strcpy(aLex[*numLex].lexeme, lexeme);
            (*numLex)++;
            return 1;
        } else {
            // Invalid number
            aLex[*numLex].token = INVALID;
            strcpy(aLex[*numLex].lexeme, lexeme);
            (*numLex)++;
            return 1;
        }
    }

    // Check for keywords or identifiers using validIdentifier function
    if (isalpha(c)) {
        while (isAlphanumeric(c)) {
            lexeme[lexemeIndex++] = c;
            c = fgetc(inf);
        }
        lexeme[lexemeIndex] = '\0';
        ungetc(c, inf); // Push back the last non-alphanumeric character
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
            return 1;
        } else {
            // Invalid identifier
            aLex[*numLex].token = INVALID;
            strcpy(aLex[*numLex].lexeme, lexeme);
            (*numLex)++;
            return 1;
        }
    }

    // If none of the above cases match, it's an invalid character
    aLex[*numLex].token = INVALID;
    sprintf(aLex[*numLex].lexeme, "Invalid character: %c", c);
    (*numLex)++;
    return 1;
}

const char* tokenEnumToString(enum token aToken){
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

    struct lexics allLexics[LEXEME_MAX]; // Use LEXEME_MAX from Givens.h
    int numberOfLexics = 0;

    printf("Did tokenize: %d\n", tokenizer(allLexics, &numberOfLexics, file));
    printf("Number of lexemes: %d\n", numberOfLexics);
    printf("Lexemes: ");
    for (int i = 0; i < numberOfLexics; i++) {
        printf("%s  ", allLexics[i].lexeme);
    }
    printf("\nTokens: ");
    for (int i = 0; i < numberOfLexics; i++) {
        printf("%s  ", tokenEnumToString(allLexics[i].token));
    }

    fclose(file);

    return 0;
}