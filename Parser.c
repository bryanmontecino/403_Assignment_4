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
#include "Parser.h"
#include <stdio.h>

// Function to check if token at an index matches expected token
// If match, increments the index -> returns TRUE
// Otherwise, it returns FALSE
_Bool lexeme_crosscheck(enum token expected, struct lexics *someLexics, int *index, int numberOfLexics) {
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
    return function(someLexics, &index, numberOfLexics) && index == numberOfLexics;
}

// Implementation of grammar rules

// Checks if a valid function is defined by ensuring header and body of the function are correctly defined
_Bool function(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (header(someLexics, index, numberOfLexics) && body(someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Header rule: checks the syntax of a function header
_Bool header(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(VARTYPE, someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(IDENTIFIER, someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(LEFT_PARENTHESIS, someLexics, index, numberOfLexics)) {
        if (arg_decl(someLexics, index, numberOfLexics)) {
            return lexeme_crosscheck(RIGHT_PARENTHESIS, someLexics, index, numberOfLexics);
        } else {
            return lexeme_crosscheck(RIGHT_PARENTHESIS, someLexics, index, numberOfLexics);
        }
    }
    return FALSE;
}

// Argument declaration rule: handles the argument declaration part of a function header
_Bool arg_decl(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(VARTYPE, someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(IDENTIFIER, someLexics, index, numberOfLexics)) {
        while (lexeme_crosscheck(COMMA, someLexics, index, numberOfLexics)) {
            if (!lexeme_crosscheck(VARTYPE, someLexics, index, numberOfLexics) ||
                !lexeme_crosscheck(IDENTIFIER, someLexics, index, numberOfLexics)) {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

// Body rule: verifies the syntax of the function body
_Bool body(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(LEFT_BRACKET, someLexics, index, numberOfLexics)) {
        if (statement_list(someLexics, index, numberOfLexics)) {
            return lexeme_crosscheck(RIGHT_BRACKET, someLexics, index, numberOfLexics);
        } else {
            return lexeme_crosscheck(RIGHT_BRACKET, someLexics, index, numberOfLexics);
        }
    }
    return FALSE;
}

// Statement list rule: handles a list of statements within a function body
_Bool statement_list(struct lexics *someLexics, int *index, int numberOfLexics) {
    while (statement(someLexics, index, numberOfLexics)) {
    }
    return TRUE;
}

// Statement rule: checks for various types of statements within a function body
_Bool statement(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (while_loop(someLexics, index, numberOfLexics) ||
        return_stmt(someLexics, index, numberOfLexics) ||
        assignment(someLexics, index, numberOfLexics) ||
        body(someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// While loop rule: checks for the presence of WHILE keyword, ( expression ) and a statement within the loop
_Bool while_loop(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(WHILE_KEYWORD, someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(LEFT_PARENTHESIS, someLexics, index, numberOfLexics) &&
        expression(someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(RIGHT_PARENTHESIS, someLexics, index, numberOfLexics) &&
        statement(someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Return statement rule: checks the syntax of a return statement
_Bool return_stmt(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(RETURN_KEYWORD, someLexics, index, numberOfLexics) &&
        expression(someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(EOL, someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Assignment rule: checks for an identifier, EQUAL, an expression and EOL
_Bool assignment(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(IDENTIFIER, someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(EQUAL, someLexics, index, numberOfLexics) &&
        expression(someLexics, index, numberOfLexics) &&
        lexeme_crosscheck(EOL, someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}

// Expression rule: defines the syntax of an expression, consisting of one or more BINOP
_Bool expression(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (term(someLexics, index, numberOfLexics)) {
        while (lexeme_crosscheck(BINOP, someLexics, index, numberOfLexics)) {
            if (!term(someLexics, index, numberOfLexics)) {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

// Term rule: checks if a term is either IDENTIFIER or NUMBER 
_Bool term(struct lexics *someLexics, int *index, int numberOfLexics) {
    if (lexeme_crosscheck(IDENTIFIER, someLexics, index, numberOfLexics) || 
        lexeme_crosscheck(NUMBER, someLexics, index, numberOfLexics)) {
        return TRUE;
    }
    return FALSE;
}