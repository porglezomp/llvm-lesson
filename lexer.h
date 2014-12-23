#ifndef _LEXER_H
#define _LEXER_H

#include <string>

extern std::string identifier_string;
extern double num_value;

enum Token {
    tok_eof = -1,

    // Commands
    tok_def = -2, tok_extern = -3,

    // Primary
    tok_identifier = -4, tok_number = -5,
};

int get_token();

#endif