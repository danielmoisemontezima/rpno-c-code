#ifndef UTILS_H
#define UTILS_H

#include "tokenizer.h"

// Define Stack struct
typedef struct Stack {
    struct Node *top;
} Stack;

// Function prototypes
Stack* create_stack();
void push(Stack* stack, Token token);
Token pop(Stack* stack);
Token peek(Stack* stack);
int is_empty(Stack* stack);
void free_stack(Stack* stack);

#endif