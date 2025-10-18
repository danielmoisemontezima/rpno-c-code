#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 25
#define X_MIN 0.0
#define X_MAX (4 * M_PI)
#define Y_MIN -1.0
#define Y_MAX 1.0

typedef enum {
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFT_PAREN,
    RIGHT_PAREN
} TokenType;

typedef struct Token {
    TokenType type;
    double value;
    char op[16];
} Token;

typedef struct Node {
    Token token;
    struct Node *next;
} Node;

typedef struct Stack {
    Node *top;
} Stack;

// Custom character classification functions
int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

Stack* create_stack() {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    if (s == NULL) return NULL;
    s->top = NULL;
    return s;
}

void push(Stack *s, Token token) {
    if (s == NULL) return;
    
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) return;
    
    new_node->token = token;
    new_node->next = s->top;
    s->top = new_node;
}

Token pop(Stack *s) {
    Token empty = {0};
    if (s == NULL || s->top == NULL) {
        return empty;
    }
    
    Node *temp = s->top;
    Token token = temp->token;
    s->top = temp->next;
    free(temp);
    return token;
}

Token peek(Stack *s) {
    Token empty = {0};
    if (s == NULL || s->top == NULL) {
        return empty;
    }
    return s->top->token;
}

int is_empty(Stack *s) {
    return s == NULL || s->top == NULL;
}

void free_stack(Stack *s) {
    if (s == NULL) return;
    
    while (!is_empty(s)) {
        pop(s);
    }
    free(s);
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int is_function(const char *str) {
    const char *funcs[] = {"sin", "cos", "tan", "sqrt", "log", "ln"};
    for (size_t i = 0; i < sizeof(funcs)/sizeof(funcs[0]); i++) {
        if (strcmp(str, funcs[i]) == 0) return 1;
    }
    return 0;
}

int get_priority(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

Token create_token(TokenType type, double value, const char* op) {
    Token token;
    token.type = type;
    token.value = value;
    if (op != NULL) {
        strncpy(token.op, op, sizeof(token.op) - 1);
        token.op[sizeof(token.op) - 1] = '\0';
    } else {
        token.op[0] = '\0';
    }
    return token;
}

int tokenize(const char *expr, Token **tokens, int *count) {
    if (expr == NULL || tokens == NULL || count == NULL) return 0;
    
    Stack *paren_stack = create_stack();
    if (paren_stack == NULL) return 0;
    
    *count = 0;
    int capacity = 10;
    *tokens = (Token*)malloc(capacity * sizeof(Token));
    if (*tokens == NULL) {
        free_stack(paren_stack);
        return 0;
    }
    
    const char *p = expr;
    while (*p) {
        if (is_space(*p)) {
            p++;
            continue;
        }
        
        if (*count >= capacity) {
            capacity *= 2;
            Token *temp = realloc(*tokens, capacity * sizeof(Token));
            if (temp == NULL) {
                free(*tokens);
                free_stack(paren_stack);
                return 0;
            }
            *tokens = temp;
        }

        if (is_digit(*p) || *p == '.') {
            char *end;
            double value = strtod(p, &end);
            if (end == p) {
                free(*tokens);
                free_stack(paren_stack);
                return 0;
            }
            (*tokens)[*count] = create_token(NUMBER, value, NULL);
            p = end;
            (*count)++;
            continue;
        }

        if (is_alpha(*p)) {
            char buf[16] = {0};
            int len = 0;
            while (is_alpha(*p) && len < 15) {
                buf[len++] = *p++;
            }
            
            if (strcmp(buf, "x") == 0) {
                (*tokens)[*count] = create_token(VARIABLE, 0, NULL);
            } else if (is_function(buf)) {
                (*tokens)[*count] = create_token(FUNCTION, 0, buf);
            } else {
                free(*tokens);
                free_stack(paren_stack);
                return 0;
            }
            (*count)++;
            continue;
        }

        if (is_operator(*p)) {
            char op_str[2] = {*p, '\0'};
            (*tokens)[*count] = create_token(OPERATOR, 0, op_str);
            (*count)++;
            p++;
            continue;
        }

        if (*p == '(') {
            push(paren_stack, create_token(LEFT_PAREN, 0, NULL));
            (*tokens)[*count] = create_token(LEFT_PAREN, 0, NULL);
            (*count)++;
            p++;
            continue;
        }

        if (*p == ')') {
            if (is_empty(paren_stack)) {
                free(*tokens);
                free_stack(paren_stack);
                return 0;
            }
            pop(paren_stack);
            (*tokens)[*count] = create_token(RIGHT_PAREN, 0, NULL);
            (*count)++;
            p++;
            continue;
        }

        free(*tokens);
        free_stack(paren_stack);
        return 0;
    }

    if (!is_empty(paren_stack)) {
        free(*tokens);
        free_stack(paren_stack);
        return 0;
    }
    
    free_stack(paren_stack);
    return 1;
}

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

void plot_graph(Token *rpn, int rpn_count) {
    char field[HEIGHT][WIDTH];
    
    // Initialize field with dots
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = '.';
        }
    }

    // Calculate and plot the graph
    for (int j = 0; j < WIDTH; j++) {
        double x = X_MIN + j * (X_MAX - X_MIN) / (WIDTH - 1);
        double y = evaluate_rpn(rpn, rpn_count, x);
        
        if (y == y && y >= Y_MIN && y <= Y_MAX) { // y == y проверяет, что не NaN
            // Convert y coordinate to screen position
            // With Y axis pointing downward
            int screen_y = (int)round((y - Y_MIN) * (HEIGHT - 1) / (Y_MAX - Y_MIN));
            
            if (screen_y >= 0 && screen_y < HEIGHT) {
                field[screen_y][j] = '*';
            }
        }
    }

    // Output the field
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(field[i][j]);
        }
        putchar('\n');
    }
}

int main() {
    char expression[256];
    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        printf("n/a\n");
        return 1;
    }
    
    // Remove newline character
    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }
    
    // Check for empty expression
    if (len == 0 || expression[0] == '\0') {
        printf("n/a\n");
        return 1;
    }

    Token *tokens = NULL;
    int count = 0;
    if (!tokenize(expression, &tokens, &count)) {
        printf("n/a\n");
        return 1;
    }

    Token *rpn = NULL;
    int rpn_count = 0;
    if (!to_rpn(tokens, count, &rpn, &rpn_count)) {
        free(tokens);
        printf("n/a\n");
        return 1;
    }

    plot_graph(rpn, rpn_count);

    free(tokens);
    free(rpn);
    return 0;
}