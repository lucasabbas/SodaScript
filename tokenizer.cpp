#include "tokenizer.h"
#include "token.h"
#include "utils.h"
#include <cctype>

bool Tokenizer::isOperator(char c) {
  // Define the logic to check if a character is an operator or punctuation
  return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
          c == '<' || c == '>' || c == '(' || c == ')' || c == '{' ||
          c == '}' || c == '[' || c == ']' || c == ':' || c == ';' ||
          c == '%' || c == '$' || c == '!' || c == '?' || c == ',' ||
          c == '.' || c == '~'); // Add more punctuation as needed
}

std::vector<Token> Tokenizer::tokenize(const std::string &input) {
  std::vector<Token> tokens;
  std::string currentWord;
  int line = 1;   // Start from the first line
  int column = 1; // Start from the first column

  for (size_t i = 0; i < input.length(); ++i) {
    char currentChar = input[i];

    // Handle newlines and update line and column
    if (currentChar == '\n') {
      line++;
      column = 1; // Reset column for the new line
      continue;
    }

    // Handle whitespace and update column
    if (isWhitespace(currentChar)) {
      column++;
      continue;
    }

    // Handle string literals
    if (currentChar == '"') {
      std::string literal;
      int literalStartColumn = column; // Store the start column for the literal

      // Move to the next character and start capturing the string literal
      currentChar = input[++i];
      column++; // Increment column since we consumed a character

      while (currentChar != '"' && i < input.length()) {
        if (currentChar == '\\' && i + 1 < input.length() &&
            input[i + 1] == '"') {
          literal += currentChar;
          currentChar = input[++i]; // Move past the escaped quote
        }
        literal += currentChar;
        currentChar = input[++i];
        column++; // Update column for each character
      }

      // Create a string literal token
      tokens.push_back(Token(StringLiteral, literal, line, literalStartColumn));
      continue;
    }

    // Handle operators and punctuation
    if (isOperator(currentChar)) {
      tokens.push_back(Token(getOperatorToken(currentChar),
                             std::string(1, currentChar), line, column));
      column++; // Increment column
      continue;
    }

    // Handle letters (likely a keyword or identifier)
    if (isalpha(currentChar)) {
      currentWord.clear();
      int wordStartColumn = column; // Store the start column for the word
      while (isalpha(currentChar)) {
        currentWord += currentChar;
        currentChar = input[++i];
        column++;
      }
      --i; // Step back to avoid skipping the next character

      tokens.push_back(Token(getKeywordToken(currentWord), currentWord, line,
                             wordStartColumn));
      continue;
    }

    // Handle numbers (literals)
    if (isdigit(currentChar)) {
      currentWord.clear();
      int numberStartColumn = column; // Store the start column for the number
      while (isdigit(currentChar)) {
        currentWord += currentChar;
        currentChar = input[++i];
        column++;
      }
      --i; // Step back to avoid skipping the next character

      tokens.push_back(
          Token(IntegerLiteral, currentWord, line, numberStartColumn));
    }

    column++; // Increment column for each character
  }

  // Add an EndOfFile token to mark the end of the file
  tokens.push_back(Token(EndOfFile, "", line, column));

  return tokens;
}

SodaScriptToken Tokenizer::getKeywordToken(const std::string &word) {
  if (word == "package")
    return PackageKeyword;
  if (word == "import")
    return ImportKeyword;
  if (word == "private")
    return PrivateKeyword;
  if (word == "public")
    return PublicKeyword;
  if (word == "class")
    return ClassKeyword;
  if (word == "extends")
    return ExtendsKeyword;
  if (word == "function")
    return FunctionKeyword;
  if (word == "constructor")
    return ConstructorKeyword;
  if (word == "static")
    return StaticKeyword;
  if (word == "new")
    return NewKeyword;
  if (word == "return")
    return ReturnKeyword;
  if (word == "if")
    return IfKeyword;
  if (word == "else")
    return ElseKeyword;
  if (word == "while")
    return WhileKeyword;
  if (word == "for")
    return ForKeyword;
  if (word == "foreach")
    return ForeachKeyword;
  if (word == "in")
    return InKeyword;
  if (word == "continue")
    return ContinueKeyword;
  if (word == "break")
    return BreakKeyword;
  if (word == "is")
    return IsKeyword;
  if (word == "var")
    return VarKeyword;
  // Add other keywords...
  return Identifier;
}

SodaScriptToken Tokenizer::getOperatorToken(char c) {
  switch (c) {
  case '+':
    return Plus;
  case '-':
    return Minus;
  case '*':
    return Multiply;
  case '/':
    return Divide;
  case '=':
    return Assign;
  case '<':
    return LessThan;
  case '>':
    return GreaterThan;
  case ':':
    return Colon;
  case ';':
    return Semicolon;
  case '(':
    return LParen;
  case ')':
    return RParen;
  case '[':
    return LBracket;
  case ']':
    return RBracket;
  case '{':
    return LBrace;
  case '}':
    return RBrace;
  case '.':
    return Dot;
  case ',':
    return Comma;
  case '%':
    return Modulo;
  case '!':
    return Not;
  case '~':
    return Tilde;
  case '$':
    return DollarSign;
  case '?':
    return QuestionMark;
  // Add any other punctuation you need here
  default:
    return Identifier; // Fallback case
  }
}
