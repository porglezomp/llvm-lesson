#include "lexer.h"
#include "helper.h"

#include <string>
#include <cstdlib>

std::string identifier_string;
double num_value;

int get_token() {
    static int last_char = ' ';

    // Skip whitespace
    while (isspace(last_char)) {
        last_char = getchar();
    }

    if (isalpha(last_char)) { // Identifier [a-zA-Z][a-zA-Z0-9]*
        identifier_string = last_char;
        while (isalnum((last_char = getchar()))) {
            identifier_string += last_char;
        }

        if (identifier_string == "def") return tok_def;
        if (identifier_string == "extern") return tok_extern;
        return tok_identifier;
    }

    if (isdigit(last_char) || last_char == '.') {
        std::string num_string;
        bool integral = true;
        do {
            num_string += last_char;
            last_char = getchar();

            // If this is not the first decimal point in the number, log a (bad) error message
            if (last_char == '.') {
                if (integral) integral = false;
                else fail("Extra decimal in numeric value starting with '" + num_string + "'");
            }
        } while (isdigit(last_char) || last_char == '.');
        num_value = strtod(num_string.c_str(), 0);
        return tok_number;
    }

    if (last_char == '#') {
        // Comment until end of line
        do last_char = getchar();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');
        if (last_char != EOF)
            return get_token();
    }

    // Check for end of file, don't eat EOF
    if (last_char == EOF) {
        return tok_eof;
    }

    // Return the character as its ASCII value
    int this_char = last_char;
    // Clear out the last character (otherwise we get repeating)
    last_char = getchar();
    return this_char;
}