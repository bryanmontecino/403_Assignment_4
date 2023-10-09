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

#ifndef ANALYZER_H
#define ANALYZER_H

#include "Tokenizer.h"
#include "Parser.h"

const char* tokenEnumToString(enum token aToken);

#endif