# Project Overview

## Modules

### Tokenizer
- **Files**: `tokenizer.c`, `tokenizer.h`
- **Purpose**: Handles tokenization of mathematical expressions.
- **Key Functions**:
  - `tokenize`: Converts an input string into tokens.
  - `create_token`: Creates a token of a specific type.

### RPN (Reverse Polish Notation)
- **Files**: `rpn.c`, `rpn.h`
- **Purpose**: Converts tokens into RPN using the Shunting-Yard algorithm.
- **Key Functions**:
  - `to_rpn`: Converts tokens to RPN.
  - `get_priority`: Determines operator precedence.

### Evaluator
- **Files**: `evaluator.c`, `evaluator.h`
- **Purpose**: Evaluates RPN expressions.
- **Key Functions**:
  - `evaluate_rpn`: Computes the result of an RPN expression.

### Graph
- **Files**: `graph.c`, `graph.h`
- **Purpose**: Plots a graph based on the evaluated RPN expression.
- **Key Functions**:
  - `plot_graph`: Generates a text-based graph.

### Utilities
- **Files**: `utils.c`, `utils.h`
- **Purpose**: Provides common helper functions.
- **Key Functions**:
  - `create_stack`, `push`, `pop`: Stack operations.

## How It Works
1. The user inputs a mathematical expression.
2. The `Tokenizer` module parses the input into tokens.
3. The `RPN` module converts the tokens into Reverse Polish Notation.
4. The `Evaluator` module evaluates the RPN expression for a range of `x` values.
5. The `Graph` module plots the results as a text-based graph.

## Usage
1. Compile the project using a C compiler.
2. Run the executable and input a mathematical expression.
3. View the plotted graph in the terminal.

## Example
Input:
```
sin(x)
```
Output:
```
... (graph output) ...
```