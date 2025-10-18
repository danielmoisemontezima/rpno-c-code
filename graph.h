#ifndef GRAPH_H
#define GRAPH_H

#include "evaluator.h"

// Define constants for graph dimensions and range
#define WIDTH 80
#define HEIGHT 25
#define X_MIN 0.0
#define X_MAX (4 * M_PI)
#define Y_MIN -1.0
#define Y_MAX 1.0

// Function prototypes
void plot_graph(Token *rpn, int rpn_count);

#endif