/*
Author: Bryan Lizama Montecino
Course: CMSC 403
File For: Assignment 4
Last Edited: October 8, 2023
*/

#include "Givens.h"

_Bool validNumber(char* aLexeme){
	regex_t numberRegex;
	regcomp(&numberRegex, "^[0-9][0-9]*$", REG_EXTENDED);
	return !regexec(&numberRegex, aLexeme, 0, 0, 0);	
}

_Bool validIdentifier(char * aLexeme){
	regex_t identifierRegex;
	regcomp(&identifierRegex, "^[a-zA-Z][a-zA-Z0-9]*$", REG_EXTENDED);
	return !regexec(&identifierRegex, aLexeme, 0, 0, 0);
}