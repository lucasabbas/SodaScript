#include "utils.h"
#include <cctype>

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>');
}

bool isWhitespace(char c) {
    return std::isspace(c);
}
