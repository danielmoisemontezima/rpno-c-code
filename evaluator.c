#include "evaluator.h"
#include "rpn.h"
#include "utils.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

// Evaluation logic implementation
double evaluate_rpn(Token *rpn, int count, double x) {
    if (rpn == NULL || count <= 0) return NAN;

    Stack *stack = create_stack();
    if (stack == NULL) return NAN;

    for (int i = 0; i < count; i++) {
        Token t = rpn[i];

        if (t.type == NUMBER) {
            push(stack, t);
        } else if (t.type == VARIABLE) {
            push(stack, create_token(NUMBER, x, NULL));
        } else if (t.type == OPERATOR) {
            if (is_empty(stack)) {
                free_stack(stack);
                return NAN;
            }
            double b = pop(stack).value;

            if (is_empty(stack)) {
                free_stack(stack);
                return NAN;
            }
            double a = pop(stack).value;

            double result;
            switch (t.op[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': 
                    if (b == 0) {
                        free_stack(stack);
                        return NAN;
                    }
                    result = a / b; 
                    break;
                case '^': result = pow(a, b); break;
                default: 
                    free_stack(stack);
                    return NAN;
            }
            push(stack, create_token(NUMBER, result, NULL));
        } else if (t.type == FUNCTION) {
            if (is_empty(stack)) {
                free_stack(stack);
                return NAN;
            }
            double a = pop(stack).value;
            double result;

            if (strcmp(t.op, "sin") == 0) result = sin(a);
            else if (strcmp(t.op, "cos") == 0) result = cos(a);
            else if (strcmp(t.op, "tan") == 0) result = tan(a);
            else if (strcmp(t.op, "sqrt") == 0) {
                if (a < 0) {
                    free_stack(stack);
                    return NAN;
                }
                result = sqrt(a);
            }
            else if (strcmp(t.op, "log") == 0) {
                if (a <= 0) {
                    free_stack(stack);
                    return NAN;
                }
                result = log10(a);
            }
            else if (strcmp(t.op, "ln") == 0) {
                if (a <= 0) {
                    free_stack(stack);
                    return NAN;
                }
                result = log(a);
            }
            else {
                free_stack(stack);
                return NAN;
            }
            push(stack, create_token(NUMBER, result, NULL));
        }
    }

    if (is_empty(stack)) {
        free_stack(stack);
        return NAN;
    }

    double result = pop(stack).value;
    free_stack(stack);
    return result;
}