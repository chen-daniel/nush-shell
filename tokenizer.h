/* This file is from Spring 2019 Assignment 5 CS3650 */
/* Author: Daniel Chen */

#ifndef NUSH_TOKENIZER_H
#define NUSH_TOKENIZER_H

#include "svec.h"

svec *tokenize(const char *text);

svec *tokenizeParen(const char *text);

#endif
