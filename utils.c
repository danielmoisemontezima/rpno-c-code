#include "utils.h"
#include <stdlib.h>

// Define Node struct
typedef struct Node {
    Token token;
    struct Node *next;
} Node;

// Utility functions implementation

// Function to create a stack of given capacity
Stack* create_stack() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack) stack->top = NULL;
    return stack;
}

// Stack is empty when top is NULL
int is_empty(Stack* stack) {
    return stack->top == NULL;
}

// Function to add an item to stack. It increases the top by 1
void push(Stack* stack, Token token) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node) {
        new_node->token = token;
        new_node->next = stack->top;
        stack->top = new_node;
    }
}

// Function to remove an item from stack. It decreases the top by 1
Token pop(Stack* stack) {
    Token empty = {0};
    if (is_empty(stack)) return empty;
    Node *temp = stack->top;
    Token token = temp->token;
    stack->top = temp->next;
    free(temp);
    return token;
}

// Function to peek the top item of the stack
Token peek(Stack* stack) {
    Token empty = {0};
    return is_empty(stack) ? empty : stack->top->token;
}

void free_stack(Stack* stack) {
    while (!is_empty(stack)) pop(stack);
    free(stack);
}