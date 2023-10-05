#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h> // Include for FILE type

// Function to tokenize the input and populate the lexics array
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

#endif