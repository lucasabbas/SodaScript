#include "package.h"
#include "parser.h"
#include "tokenizer.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // Read from file or string input
  std::ifstream file("./Example01.soda");
  std::string input((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));

  // Initialize the tokenizer
  Tokenizer tokenizer;

  // Tokenize the input
  std::vector<Token> tokens = tokenizer.tokenize(input);

  // Print out the tokens
  for (size_t i = 0; i < tokens.size(); ++i) {
    std::cout << "Token: " << tokens[i].value << " Type: " << tokens[i].type
              << " Line:" << tokens[i].line << " Column:" << tokens[i].column
              << " ID: " << i << std::endl;
  }

  // Initialize the parser
  Parser parser(tokens);

  // Parse the input
  Package package = parser.parse();

  // Print out the package
  std::cout << "Package: " << package.Name << std::endl;

  return 0;
}
