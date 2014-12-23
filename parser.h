#ifndef _PARSER_H
#define _PARSER_H

#include "ast.h"
#include <map>
class ExprAST;

extern int current_token;
extern std::map<char, int> binop_precendence;

int get_next_token();
int get_token_precedence();
ExprAST *Error(const char*);
PrototypeAST *ErrorP(const char*);
FunctionAST *ErrorF(const char*);

ExprAST *parse_number_expr();
ExprAST *parse_paren_expr();
ExprAST *parse_identifier_expr();
ExprAST *parse_primary();
ExprAST *parse_expression();
ExprAST *parse_binop_rhs(int, ExprAST*);
PrototypeAST *parse_prototype();
FunctionAST *parse_definition();
PrototypeAST *parse_extern();
FunctionAST *parse_top_level_expression();

#endif