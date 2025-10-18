#include "rpn.h"
#include "tokenizer.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// RPN conversion logic implementation

// Function to convert an expression to Reverse Polish Notation (RPN)
int to_rpn(Token *tokens, int count, Token **rpn, int *rpn_count) {
    if (tokens == NULL || count <= 0 || rpn == NULL || rpn_count == NULL) return 0;

    Stack *op_stack = create_stack();
    if (op_stack == NULL) return 0;

    *rpn = (Token*)malloc(count * sizeof(Token));
    if (*rpn == NULL) {
        free_stack(op_stack);
        return 0;
    }
    *rpn_count = 0;

    for (int i = 0; i < count; i++) {
        Token t = tokens[i];
        switch (t.type) {
            case NUMBER:
            case VARIABLE:
                (*rpn)[(*rpn_count)++] = t;
                break;

            case FUNCTION:
                push(op_stack, t);
                break;

            case OPERATOR:
                while (!is_empty(op_stack) && 
                       peek(op_stack).type != LEFT_PAREN &&
                       get_priority(peek(op_stack).op[0]) >= get_priority(t.op[0])) {
                    (*rpn)[(*rpn_count)++] = pop(op_stack);
                }
                push(op_stack, t);
                break;

            case LEFT_PAREN:
                push(op_stack, t);
                break;

            case RIGHT_PAREN:
                while (!is_empty(op_stack) && peek(op_stack).type != LEFT_PAREN) {
                    (*rpn)[(*rpn_count)++] = pop(op_stack);
                }
                if (is_empty(op_stack)) {
                    free(*rpn);
                    free_stack(op_stack);
                    return 0;
                }
                pop(op_stack); // Remove left parenthesis
                if (!is_empty(op_stack) && peek(op_stack).type == FUNCTION) {
                    (*rpn)[(*rpn_count)++] = pop(op_stack);
                }
                break;

            default:
                break;
        }
    }

    while (!is_empty(op_stack)) {
        if (peek(op_stack).type == LEFT_PAREN) {
            free(*rpn);
            free_stack(op_stack);
            return 0;
        }
        (*rpn)[(*rpn_count)++] = pop(op_stack);
    }

    free_stack(op_stack);
    return 1;
}

// Function to get the priority of operators
int get_priority(char operator) {
    switch (operator) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// Add other necessary functions and their implementations here