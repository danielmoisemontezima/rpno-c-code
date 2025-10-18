#ifndef TOKENIZER_H
#define TOKENIZER_H

// Define TokenType enum
typedef enum {
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFT_PAREN,
    RIGHT_PAREN
} TokenType;

// Define Token struct
typedef struct {
    TokenType type;
    double value;
    char op[16];
} Token;

// Function prototypes
int tokenize(const char *expr, Token **tokens, int *count);
Token create_token(TokenType type, double value, const char* op);

#endif