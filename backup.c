#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define token types
typedef enum {
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    WHILE_KEYWORD,
    RETURN_KEYWORD,
    EQUAL,
    COMMA,
    EOL,
    VARTYPE,
    IDENTIFIER,
    BINOP,
    WHITESPACE,
    NUMBER
} TokenType;

#define INVALID 14

// Structure to represent a token
typedef struct {
    TokenType type;
    char lexeme[100]; // Adjust the size as needed
} Token;

// Function to check if a character is a valid alphanumeric character
int isAlphanumeric(char c) {
    return isalnum(c) || c == '_';
}

// Function to tokenize the input text
int tokenizer(Token allLexics[], int* numberOfLexics, FILE *file) {
    char c;
    int lexemeIndex;
    Token token;
    
    *numberOfLexics = 0; // Initialize the number of lexics to 0

    while ((c = fgetc(file)) != EOF) {
        lexemeIndex = 0;
        
        // Skip whitespace and comments
        while (isspace(c) || c == '/') {
            if (c == '/') {
                if (fgetc(file) == '/') {
                    // Single-line comment, skip until EOL
                    while ((c = fgetc(file)) != '\n' && c != EOF);
                } else {
                    // Not a comment, push back the character
                    ungetc('/', file);
                    break;
                }
            }
            c = fgetc(file);
        }

        if (c == EOF) {
            break;
        }

        // Check for single-character tokens
        if (c == '(') {
            token.type = LEFT_PARENTHESIS;
            strcpy(token.lexeme, "(");
        } else if (c == ')') {
            token.type = RIGHT_PARENTHESIS;
            strcpy(token.lexeme, ")");
        } else if (c == '{') {
            token.type = LEFT_BRACKET;
            strcpy(token.lexeme, "{");
        } else if (c == '}') {
            token.type = RIGHT_BRACKET;
            strcpy(token.lexeme, "}");
        } else if (c == '=') {
            token.type = EQUAL;
            strcpy(token.lexeme, "=");
        } else if (c == ',') {
            token.type = COMMA;
            strcpy(token.lexeme, ",");
        } else if (c == ';') {
            token.type = EOL;
            strcpy(token.lexeme, ";");
        } else if (isspace(c)) { // Handle whitespace
            token.type = WHITESPACE;
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
        } else {
            // Check for operators
            lexemeIndex = 0;
            if (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(file) == '=')) {
                token.lexeme[lexemeIndex++] = c;
                if (c == '!' && lexemeIndex == 1) {
                    // != operator
                    token.lexeme[lexemeIndex++] = '=';
                }
                token.lexeme[lexemeIndex] = '\0';
                token.type = BINOP;
            }
            // Check for numbers
            else if (isdigit(c)) {
                while (isdigit(c)) {
                    token.lexeme[lexemeIndex++] = c;
                    c = fgetc(file);
                }
                token.lexeme[lexemeIndex] = '\0';
                ungetc(c, file); // Push back the last non-digit character
                token.type = NUMBER;
            }
            // Check for keywords or identifiers
            else if (isalpha(c)) {
                while (isAlphanumeric(c)) {
                    token.lexeme[lexemeIndex++] = c;
                    c = fgetc(file);
                }
                token.lexeme[lexemeIndex] = '\0';
                ungetc(c, file); // Push back the last non-alphanumeric character
                if (strcmp(token.lexeme, "while") == 0) {
                    token.type = WHILE_KEYWORD;
                } else if (strcmp(token.lexeme, "return") == 0) {
                    token.type = RETURN_KEYWORD;
                } else if (strcmp(token.lexeme, "int") == 0 || strcmp(token.lexeme, "void") == 0) {
                    token.type = VARTYPE;
                } else {
                    token.type = IDENTIFIER;
                }
            } else {
                // If none of the above cases match, it's an invalid character
                token.type = INVALID;
                sprintf(token.lexeme, "Invalid character: %c", c);
            }
        }

        // Store the token in the array
        if (*numberOfLexics < 1000) { // Adjust the limit as needed
            allLexics[*numberOfLexics] = token;
            (*numberOfLexics)++;
        } else {
            printf("Too many tokens, cannot tokenize further.\n");
            return 0;
        }
    }

    return 1;
}

const char* tokenEnumToString(TokenType aToken) {
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
	else if(aToken == WHITESPACE){ // Added WHITESPACE case
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

    Token allLexics[1000]; // Adjust the array size as needed
    int numberOfLexics = 0;
    int success = tokenizer(allLexics, &numberOfLexics, file);

    printf("Did tokenize: %d\n", success);
    printf("Number of lexemes: %d\n", numberOfLexics);
    printf("Lexemes: ");
    for (int i = 0; i < numberOfLexics; i++) {
        printf("%s  ", allLexics[i].lexeme);
    }
    printf("\nTokens: ");
    for (int i = 0; i < numberOfLexics; i++) {
        printf("%s  ", tokenEnumToString(allLexics[i].type));
    }

    printf("\n");

    fclose(file);

    return 0;
}
