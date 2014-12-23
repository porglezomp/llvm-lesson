#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "helper.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"

static void handle_definition() {
  if (parse_definition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery
    get_next_token();
  }
}

static void handle_extern() {
  if (parse_extern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery
    get_next_token();
  }
}

static void handle_top_level_expression() {
  // Evaluate a top-level expression into an anonymous function
  if (parse_top_level_expression()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery
    get_next_token();
  }
}

/// top ::= definition | external | expression | ';'
static void mainloop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch(current_token) {
        case tok_eof:    return;
        case ';':        get_next_token(); break; // ignore top-level semicolons
        case tok_def:    handle_definition(); break;
        case tok_extern: handle_extern(); break;
        default:         handle_top_level_expression(); break;
        }
    }
}

//===------------------------------------------------------------------------------------------===//
// Main driver code
//===------------------------------------------------------------------------------------------===//

int main() {
    // Install standard binary operators.
    // 1 is lowest precedence.
    binop_precendence['<'] = 10;
    binop_precendence['>'] = 10;
    binop_precendence['+'] = 20;
    binop_precendence['-'] = 20;
    binop_precendence['*'] = 40; 
    binop_precendence['/'] = 40; // highest

    // Prime the first token.
    fprintf(stderr, "ready> ");
    get_next_token();

    // Run the main interpreter loop now
    mainloop();

    return 0;
}