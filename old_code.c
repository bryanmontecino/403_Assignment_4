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
    NUMBER,
    INVALID
} TokenType;

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
Token getNextToken(FILE *file) {
    Token token;
    char c = fgetc(file);
    char lexeme[100];
    int lexemeIndex = 0;

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
        token.type = INVALID;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    // Check for single-character tokens
    if (c == '(') {
        token.type = LEFT_PARENTHESIS;
        strcpy(token.lexeme, "(");
        return token;
    } else if (c == ')') {
        token.type = RIGHT_PARENTHESIS;
        strcpy(token.lexeme, ")");
        return token;
    } else if (c == '{') {
        token.type = LEFT_BRACKET;
        strcpy(token.lexeme, "{");
        return token;
    } else if (c == '}') {
        token.type = RIGHT_BRACKET;
        strcpy(token.lexeme, "}");
        return token;
    } else if (c == '=') {
        token.type = EQUAL;
        strcpy(token.lexeme, "=");
        return token;
    } else if (c == ',') {
        token.type = COMMA;
        strcpy(token.lexeme, ",");
        return token;
    } else if (c == ';') {
        token.type = EOL;
        strcpy(token.lexeme, ";");
        return token;
    }

    // Check for operators
    if (c == '+' || c == '*' || c == '%' || (c == '!' && fgetc(file) == '=')) {
        lexeme[lexemeIndex++] = c;
        if (c == '!' && lexemeIndex == 1) {
            // != operator
            lexeme[lexemeIndex++] = '=';
        }
        lexeme[lexemeIndex] = '\0';
        token.type = BINOP;
        strcpy(token.lexeme, lexeme);
        return token;
    }

    // Check for numbers
    if (isdigit(c)) {
        while (isdigit(c)) {
            lexeme[lexemeIndex++] = c;
            c = fgetc(file);
        }
        lexeme[lexemeIndex] = '\0';
        ungetc(c, file); // Push back the last non-digit character
        token.type = NUMBER;
        strcpy(token.lexeme, lexeme);
        return token;
    }

    // Check for keywords or identifiers
    if (isalpha(c)) {
        while (isAlphanumeric(c)) {
            lexeme[lexemeIndex++] = c;
            c = fgetc(file);
        }
        lexeme[lexemeIndex] = '\0';
        ungetc(c, file); // Push back the last non-alphanumeric character
        if (strcmp(lexeme, "while") == 0) {
            token.type = WHILE_KEYWORD;
            strcpy(token.lexeme, "while");
        } else if (strcmp(lexeme, "return") == 0) {
            token.type = RETURN_KEYWORD;
            strcpy(token.lexeme, "return");
        } else if (strcmp(lexeme, "int") == 0 || strcmp(lexeme, "void") == 0) {
            token.type = VARTYPE;
            strcpy(token.lexeme, lexeme);
        } else {
            token.type = IDENTIFIER;
            strcpy(token.lexeme, lexeme);
        }
        return token;
    }

    // If none of the above cases match, it's an invalid character
    token.type = INVALID;
    sprintf(token.lexeme, "Invalid character: %c", c);
    return token;
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

    Token token;
    printf("Tokens:\n");
    do {
        token = getNextToken(file);
        if (token.type != INVALID) {
            printf("%d: %s\n", token.type, token.lexeme);
        }
    } while (token.type != INVALID);

    fclose(file);

    return 0;
}