#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h>

// Function to tokenize the input from a FILE
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

#endif