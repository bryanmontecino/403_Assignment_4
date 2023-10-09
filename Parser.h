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

#ifndef PARSER_H
#define PARSER_H

#include "Givens.h"

// Function to perform syntax analysis and parsing
_Bool parser(struct lexics *someLexics, int numberOfLexics);

#endif