/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 8, 2023
*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h>

// Function to check if a character is a valid alphanumeric character
int isAlphanumeric(char c);

// Function to tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

// Function to convert a token type to a string
const char* tokenEnumToString(enum token aToken);

#endif