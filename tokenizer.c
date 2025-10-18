#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tokenization logic implementation
int tokenize(const char *expr, Token **tokens, int *count) {
    if (expr == NULL || tokens == NULL || count == NULL) return 0;

    *count = 0;
    int capacity = 10;
    *tokens = (Token*)malloc(capacity * sizeof(Token));
    if (*tokens == NULL) return 0;

    const char *p = expr;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*count >= capacity) {
            capacity *= 2;
            Token *temp = realloc(*tokens, capacity * sizeof(Token));
            if (temp == NULL) {
                free(*tokens);
                return 0;
            }
            *tokens = temp;
        }

        if (isdigit(*p) || *p == '.') {
            char *end;
            double value = strtod(p, &end);
            (*tokens)[*count] = create_token(NUMBER, value, NULL);
            p = end;
            (*count)++;
            continue;
        }

        if (isalpha(*p)) {
            char buf[16] = {0};
            int len = 0;
            while (isalpha(*p) && len < 15) {
                buf[len++] = *p++;
            }

            if (strcmp(buf, "x") == 0) {
                (*tokens)[*count] = create_token(VARIABLE, 0, NULL);
            } else {
                (*tokens)[*count] = create_token(FUNCTION, 0, buf);
            }
            (*count)++;
            continue;
        }

        p++;
    }

    return 1;
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