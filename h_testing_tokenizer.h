#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h>

// Function to tokenize the input text
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf);

const char* tokenEnumToString(enum token aToken);

#endif // TOKENIZER_H