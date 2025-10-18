#include "tokenizer.h"
#include "rpn.h"
#include "evaluator.h"
#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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