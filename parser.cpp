#include "lexer.h"
#include "parser.h"

/// The token buffer
int current_token;
/// Advance the token buffer
int get_next_token() {
    return current_token = get_token();
}

/// Binary operator precedence table
std::map<char, int> binop_precendence;

int get_token_precedence() {
    if (!isascii(current_token)) return -1;

    // Make sure it's a declared binary operator
    int token_precedence = binop_precendence[current_token];
    if (token_precedence <= 0) return -1;
    return token_precedence;
}

/// These are little helper functions for error handling
/// All return a null pointer of the specified type
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { Error(Str); return 0; }

/// numberexpr ::= number
ExprAST *parse_number_expr() {
    ExprAST *result = new NumberExprAST(num_value);
    get_next_token(); // consume the number
    return result;
}

/// parenexpr ::= '(' expression ')'
ExprAST *parse_paren_expr() {
    get_next_token(); // eat '('
    ExprAST *v = parse_expression();
    if (!v) return 0;

    if (current_token != ')') return Error("expected ')'");
    get_next_token(); // eat ')'
    return v;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
ExprAST *parse_identifier_expr() {
    std::string id_name = identifier_string;

    get_next_token(); // eat the identifier

    // A simple variable reference
    if (current_token != '(') return new VariableExprAST(id_name);

    // A function call
    get_next_token(); // eat '('
    std::vector<ExprAST*> args;
    if (current_token != ')') {
        while (true) {
            ExprAST *arg = parse_expression();
            if (!arg) return 0;
            args.push_back(arg);

            if (current_token == ')') break;

            if (current_token != ',') {
                return Error("Expected ')' or ',' in argument list");
            }
            get_next_token(); // eat ','
        }
    }

    get_next_token(); // eat the ')'

    return new CallExprAST(id_name, args);
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
ExprAST *parse_primary() {
    switch(current_token) {
    default: return Error("unknown token when expecting an expression");
    case tok_identifier: return parse_identifier_expr();
    case tok_number:     return parse_number_expr();
    case '(':            return parse_paren_expr();
    }
}

/// expression
///   ::= primary binoprhs
///
ExprAST *parse_expression() {
    ExprAST *lhs = parse_primary();
    if (!lhs) return 0;

    return parse_binop_rhs(0, lhs);
}

/// binoprhs
///   ::= ('+' primary)*
ExprAST *parse_binop_rhs(int precedence, ExprAST *lhs) {
    // If this is a binary operator find it's precedence
    while (true) {
        int token_precedence = get_token_precedence();

        // If this is a binary operator that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        // Invalid operators will have precedence -1, and will always return
        if (token_precedence < precedence) return lhs;

        // Okay, we know this is a binary operator
        int binary_operator = current_token;
        get_next_token(); // eat operator

        // Parse the primary expression after the binary operator
        ExprAST *rhs = parse_primary();
        if (!rhs) return 0;

        // If the operator binds less tightly with rhs than the operator after RHS, let
        // the pending operator take RHS as its LHS
        int next_precedence = get_token_precedence();
        if (token_precedence < next_precedence) {
            rhs = parse_binop_rhs(token_precedence + 1, rhs);
            if (rhs == 0) return 0;
        }

        // Merge the LHS/RHS
        lhs = new BinaryExprAST(binary_operator, lhs, rhs);
    } // To top of while loop
}

/// prototype
///   ::= id '(' id* ')'
PrototypeAST *parse_prototype() {
    if (current_token != tok_identifier) {
        return ErrorP("Expected function name in prototype");
    }

    std::string function_name = identifier_string;
    get_next_token(); // eat function name

    if (current_token != '(') {
        return ErrorP("Expected '(' in prototype");
    }

    // Read the list of argument names
    std::vector<std::string> arg_names;
    while (get_next_token() == tok_identifier) {
        arg_names.push_back(identifier_string);
    }
    if (current_token != ')') {
        return ErrorP("Expected ')' in prototype");
    }

    // Success
    get_next_token(); // eat ')'

    return new PrototypeAST(function_name, arg_names);
}

/// definition ::= 'def' prototype expression
FunctionAST *parse_definition() {
    get_next_token(); // eat def
    PrototypeAST *protoype = parse_prototype();
    if (protoype == 0) return 0;

    if (ExprAST *E = parse_expression()) {
        return new FunctionAST(protoype, E);
    }
    return 0;
}

/// external ::= 'extern' prototype
PrototypeAST *parse_extern() {
  get_next_token();  // eat extern
  return parse_prototype();
}

/// toplevelexpr ::= expression
FunctionAST *parse_top_level_expression() {
  if (ExprAST *E = parse_expression()) {
    // Make an anonymous prototype
    PrototypeAST *prototype = new PrototypeAST("", std::vector<std::string>());
    return new FunctionAST(prototype, E);
  }
  return 0;
}