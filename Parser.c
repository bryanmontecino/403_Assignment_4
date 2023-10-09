/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 7, 2023

Purpose of Assignment:
- Take a text file as input
- Tokenize the given input
- Parse the tokenized input to determine if it is grammatically valid 
*/

#include "Givens.h"
#include "Parser.h"
#include <stdio.h>

// Function to check if a lexeme matches the current token
_Bool match(enum token expected, struct lexics *someLexics, int *index, int numberOfLexics) {
    if (*index < numberOfLexics && someLexics[*index].token == expected) {
        (*index)++;
        return TRUE;
    }
    return FALSE;
}

// Grammar rules
_Bool function(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool header(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool arg_decl(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool body(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool statement_list(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool statement(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool while_loop(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool return_stmt(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool assignment(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool expression(struct lexics *someLexics, int *index, int numberOfLexics);
_Bool term(struct lexics *someLexics, int *index, int numberOfLexics);

_Bool parser(struct lexics *someLexics, int numberOfLexics) {
    int index = 0;
    _Bool success = function(someLexics, &index, numberOfLexics);

    // Ensure all tokens are consumed for a valid input
    if (success && index == numberOfLexics) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// Implementing the grammar rules

// Function rule
_Bool function(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (header(someLexics, index, numberOfLexics) && body(someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Header rule
_Bool header(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(VARTYPE, someLexics, index, numberOfLexics) &&
        match(IDENTIFIER, someLexics, index, numberOfLexics) &&
        match(LEFT_PARENTHESIS, someLexics, index, numberOfLexics)) {
        if (arg_decl(someLexics, index, numberOfLexics)) {
            return match(RIGHT_PARENTHESIS, someLexics, index, numberOfLexics);
        } else {
            return match(RIGHT_PARENTHESIS, someLexics, index, numberOfLexics);
        }
    }
    return FALSE;
}

// Argument declaration rule
_Bool arg_decl(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(VARTYPE, someLexics, index, numberOfLexics) &&
        match(IDENTIFIER, someLexics, index, numberOfLexics)) {
        while (match(COMMA, someLexics, index, numberOfLexics)) {
            if (!match(VARTYPE, someLexics, index, numberOfLexics) ||
                !match(IDENTIFIER, someLexics, index, numberOfLexics)) {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

// Body rule
_Bool body(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(LEFT_BRACKET, someLexics, index, numberOfLexics)) {
        if (statement_list(someLexics, index, numberOfLexics)) {
            return match(RIGHT_BRACKET, someLexics, index, numberOfLexics);
        } else {
            return match(RIGHT_BRACKET, someLexics, index, numberOfLexics);
        }
    }
    return FALSE;
}

// Statement list rule
_Bool statement_list(struct lexics *someLexics, int *index, int numberOfLexics) {
    while (statement(someLexics, index, numberOfLexics)) {
    }
    return TRUE;
}

// Statement rule
_Bool statement(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (while_loop(someLexics, index, numberOfLexics) ||
        return_stmt(someLexics, index, numberOfLexics) ||
        assignment(someLexics, index, numberOfLexics) ||
        body(someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// While loop rule
_Bool while_loop(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(WHILE_KEYWORD, someLexics, index, numberOfLexics) &&
        match(LEFT_PARENTHESIS, someLexics, index, numberOfLexics) &&
        expression(someLexics, index, numberOfLexics) &&
        match(RIGHT_PARENTHESIS, someLexics, index, numberOfLexics) &&
        statement(someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Return statement rule
_Bool return_stmt(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(RETURN_KEYWORD, someLexics, index, numberOfLexics) &&
        expression(someLexics, index, numberOfLexics) &&
        match(EOL, someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Assignment rule
_Bool assignment(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(IDENTIFIER, someLexics, index, numberOfLexics) &&
        match(EQUAL, someLexics, index, numberOfLexics) &&
        expression(someLexics, index, numberOfLexics) &&
        match(EOL, someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Expression rule
_Bool expression(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (term(someLexics, index, numberOfLexics)) {
        while (match(BINOP, someLexics, index, numberOfLexics)) {
            if (!term(someLexics, index, numberOfLexics)) {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

// Term rule
_Bool term(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (match(IDENTIFIER, someLexics, index, numberOfLexics) || 
        match(NUMBER, someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}