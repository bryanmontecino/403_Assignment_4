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

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h>

// Function to check if a character is a valid alphanumeric character
int isAlphanumeric(char c);

// Function to tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

#endif