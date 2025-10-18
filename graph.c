#include "graph.h"
#include "evaluator.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

// Graph plotting logic implementation
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

        if (y == y && y >= Y_MIN && y <= Y_MAX) { // Check for NaN and range
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

