#ifndef RPN_H
#define RPN_H

#include "tokenizer.h"

// RPN-related declarations
int to_rpn(Token *tokens, int count, Token **rpn, int *rpn_count);
int get_priority(char operator);

#endif