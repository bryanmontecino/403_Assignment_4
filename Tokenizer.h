/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 7, 2023
*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h>

// Function to tokenize the input from a FILE
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

#endif