#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include "token.h"

// Tokenizer class declaration
class Tokenizer {
public:
    // Tokenize an input string and return a vector of tokens
    std::vector<Token> tokenize(const std::string &input);

private:
    // Helper functions to identify token types
    SodaScriptToken getKeywordToken(const std::string &word);
    SodaScriptToken getOperatorToken(char c);
    bool isOperator(char c);
};

#endif // TOKENIZER_H
