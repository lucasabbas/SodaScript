#ifndef PARSER_H
#define PARSER_H

#include "ast_node.h"
#include "binary_expression.h"
#include "call_expression.h"
#include "class_declaration.h"
#include "closure_expression.h"
#include "constructor_call_expression.h"
#include "dot_access_expression.h"
#include "expression.h"
#include "for_statement.h"
#include "function_declaration.h"
#include "if_statement.h"
#include "lambda_expression.h"
#include "literal_expression.h"
#include "member_import_statement.h"
#include "package.h"
#include "package_import_statement.h"
#include "parameter.h"
#include "return_statement.h"
#include "token.h"
#include "tokenizer.h"
#include "type_reference.h"
#include "utils.h"
#include "variable_declaration.h"
#include "variable_expression.h"
#include "while_statement.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

class Parser {
public:
  Parser(const std::string &input) {
    Tokenizer tokenizer;
    tokens = tokenizer.tokenize(input);
    position = 0;
  }

  Parser(std::vector<Token> _tokens)
      : tokens(std::move(_tokens)), position(0) {}

  Package parse() {
    std::string name;
    std::vector<std::shared_ptr<AstNode>> members;
    std::vector<std::shared_ptr<PackageImportStatement>> packageImports;
    std::vector<std::shared_ptr<MemberImportStatement>> memberImports;

    while (!isAtEnd()) {
      Token current = peek();
      if (isImport(current)) {
        packageImports.push_back(parseImport());
      } else if (isMemberImport(current)) {
        memberImports.push_back(parseMemberImport());
      } else if (isPackage(current)) {
        consume();
        name = consumeIdentifier("Expected package name after 'package'");
        if (isBlock(peek())) {
          Token bodyStartToken = peek();
          Token bodyEndToken = GetMatchingBrace(bodyStartToken);
          members = parseDeclarationsUntil(bodyEndToken);
        } else if (isSemicolon(peek())) {
          consume(); // Consume the semicolon
          members = parseDeclarationsUntil(tokens[tokens.size()]);
        } else {
          error("Expected block after package declaration");
        }
      } else {
        members.push_back(parseDeclaration());
      }
    }

    return Package(name, members, packageImports, memberImports);
  }

private:
  std::vector<Token> tokens;
  size_t position;

  Token peek() const { return tokens[position]; }

  Token consume() {
    std::cout << "Consuming token: " << peek().value << " Type: " << peek().type
              << " Line: " << peek().line << " Column: " << peek().column
              << std::endl;
    return tokens[position++];
  }

  Token GetMatchingBrace(Token token) {
    if (token.type != LBrace) {
      return token; // Only process if it's an opening brace
    }

    std::stack<Token> braceStack;
    braceStack.push(token);

    // Start searching from the next token
    for (size_t i = getTokenIndex(token) + 1; i < tokens.size(); i++) {
      Token current = tokens[i];
      std::cout << "Checking token: " << current.value
                << " Type: " << current.type << " Line: " << current.line
                << " Column: " << current.column << std::endl;

      if (current.type == LBrace) {
        braceStack.push(current); // Push new opening brace onto stack
      } else if (current.type == RBrace) {
        braceStack.pop(); // Pop matching opening brace
        if (braceStack.empty()) {
          return current; // Found the matching brace
        }
      }
    }

    error("Unmatched opening brace"); // Error if no matching brace is found
    return token; // Fallback (this line is just for completeness)
  }

  size_t getTokenIndex(Token token) {
    for (size_t i = 0; i < tokens.size(); i++) {
      if (tokens[i].line == token.line && tokens[i].column == token.column &&
          tokens[i].type == token.type && tokens[i].value == token.value) {
        return i;
      }
    }
    return position;
  }

  bool match(SodaScriptToken type) {
    if (peek().type == type) {
      consume();
      return true;
    }
    return false;
  }

  bool isAtEnd() const { return position >= tokens.size(); }

  std::shared_ptr<AstNode> parseDeclaration() {
    Token current = peek();
    Token end = GetLineTermination();
    std::cout << "Parsing declaration, current token: " << current.value
              << " Type: " << current.type << " Line: " << current.line
              << " Column: " << current.column << std::endl;
    if (isClass(current)) {
      return parseClass();
    } else if (isFunction(current)) {
      return parseFunction();
    } else if (isVariable(current)) {
      return parseVariable();
    } else if (isReturn(current)) {
      return parseReturn();
    } else if (isIf(current)) {
      return parseIf();
    } else if (isFor(current)) {
      return parseFor();
    } else if (isWhile(current)) {
      return parseWhile();
    } else if (isSemicolon(current)) {
      consume();
      return nullptr;
    } else {
      return parseStandaloneExpression();
    }
  }

  std::shared_ptr<Expression> parseStandaloneExpression() {
    std::cout << "Parsing standalone expression, current token: "
              << peek().value << " Type: " << peek().type
              << " Line: " << peek().line << " Column: " << peek().column
              << std::endl;
    auto expr = parseExpression();
    if (isSemicolon(peek())) {
      consumeSemicolon();
      return expr;
    } else {
      error("Expected semicolon after expression");
    }
    return nullptr;
  }

  std::shared_ptr<PackageImportStatement> parseImport() {
    std::cout << "Parsing import statement, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'import'
    std::string path = consumeIdentifier("Expected identifier after 'import'");
    consumeSemicolon();
    return std::make_shared<PackageImportStatement>(path);
  }

  std::shared_ptr<MemberImportStatement> parseMemberImport() {
    std::cout << "Parsing member import statement, current token: "
              << peek().value << " Type: " << peek().type
              << " Line: " << peek().line << " Column: " << peek().column
              << std::endl;
    std::string packageName = consumeIdentifier("Expected package name");
    consume(); // Consume '.'
    std::string memberName =
        consumeIdentifier("Expected member name after '.'");
    consumeSemicolon();
    return std::make_shared<MemberImportStatement>(packageName, memberName);
  }

  std::shared_ptr<ClassDeclaration> parseClass() {
    std::cout << "Parsing class declaration, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'class'
    std::string name = consumeIdentifier("Expected class name");

    std::shared_ptr<TypeReference> baseClass =
        match(ExtendsKeyword) ? parseTypeReference() : nullptr;
    if (!match(LBrace)) {
      error("Expected '{' after class declaration");
    }
    Token bodyStartToken = peek();
    Token bodyEndToken = GetMatchingBrace(bodyStartToken);
    std::vector<std::shared_ptr<AstNode>> members =

        parseDeclarationsUntil(bodyEndToken);
    return std::make_shared<ClassDeclaration>(
        name, false, baseClass, std::vector<std::shared_ptr<TypeReference>>(),
        members);
  }

  std::shared_ptr<FunctionDeclaration> parseFunction() {
    std::cout << "Parsing function declaration, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'function'
    std::string name = consumeIdentifier("Expected function name");
    auto parameters = parseParameters();
    auto returnType = match(Arrow) ? parseTypeReference() : nullptr;
    if (!match(LBrace)) {
      error("Expected '{' after function declaration");
    }
    Token bodyStartToken = peek();
    Token bodyEndToken = GetMatchingBrace(bodyStartToken);
    auto body = parseDeclarationsUntil(bodyEndToken);
    return std::make_shared<FunctionDeclaration>(
        name, parameters, returnType, body,
        std::vector<std::shared_ptr<TypeReference>>());
  }

  std::shared_ptr<VariableDeclaration> parseVariable() {
    std::cout << "Parsing variable declaration, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'var'
    std::string name = consumeIdentifier("Expected variable name");
    std::shared_ptr<TypeReference> type = nullptr;
    if (match(Colon)) {
      type = parseTypeReference();
    }
    auto value = match(Assign) ? parseExpression() : nullptr;
    consumeSemicolon();
    return std::make_shared<VariableDeclaration>(name, type, value);
  }

  std::shared_ptr<ReturnStatement> parseReturn() {
    std::cout << "Parsing return statement, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'return'
    auto value = parseExpression();
    consumeSemicolon();
    return std::make_shared<ReturnStatement>(value);
  }

  std::shared_ptr<IfStatement> parseIf() {
    std::cout << "Parsing if statement, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'if'
    auto condition = parseExpression();
    if (!match(LBrace)) {
      error("Expected '{' after if condition");
    }
    Token bodyStartToken = peek();
    Token bodyEndToken = GetMatchingBrace(bodyStartToken);
    auto thenBody = parseDeclarationsUntil(bodyEndToken);
    std::vector<std::shared_ptr<AstNode>> elseBody;
    if (match(ElseKeyword)) {
      if (!match(LBrace)) {
        error("Expected '{' after else keyword");
      }
      Token elseBodyStartToken = peek();
      Token elseBodyEndToken = GetMatchingBrace(elseBodyStartToken);
      elseBody = parseDeclarationsUntil(elseBodyEndToken);
    }
    return std::make_shared<IfStatement>(condition, thenBody, elseBody);
  }

  std::shared_ptr<ForStatement> parseFor() {
    std::cout << "Parsing for statement, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'for'
    if (!match(LParen)) {
      error("Expected '(' after 'for' keyword");
    }
    auto initializer = parseExpression();
    consumeSemicolon();
    auto condition = parseExpression();
    consumeSemicolon();
    auto increment = parseExpression();
    if (!match(RParen)) {
      error("Expected ')' after for loop header");
    }
    consume(); // Consume ')'
    if (!match(LBrace)) {
      error("Expected '{' after for loop header");
    }
    Token bodyStartToken = peek();
    Token bodyEndToken = GetMatchingBrace(bodyStartToken);
    auto body = parseDeclarationsUntil(bodyEndToken);
    if (!match(RBrace)) {
      error("Expected '}' after for loop body");
    }
    return std::make_shared<ForStatement>(initializer, condition, increment,
                                          body);
  }

  std::shared_ptr<WhileStatement> parseWhile() {
    std::cout << "Parsing while statement, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    consume(); // Consume 'while'
    if (!match(LParen)) {
      error("Expected '(' after 'while'");
    }
    consume(); // Consume '('
    auto condition = parseExpression();
    if (!match(RParen)) {
      error("Expected ')' after condition");
    }
    consume(); // Consume ')'
    if (!match(LBrace)) {
      error("Expected '{' after condition");
    }
    Token bodyStartToken = peek();
    Token bodyEndToken = GetMatchingBrace(bodyStartToken);
    auto body = parseDeclarationsUntil(bodyEndToken);
    if (!match(RBrace)) {
      error("Expected '}' after body");
    }
    return std::make_shared<WhileStatement>(condition, body);
  }

  std::shared_ptr<Expression> parseDotAccess() {
    std::cout << "Parsing dot access expression, current token: "
              << peek().value << " Type: " << peek().type
              << " Line: " << peek().line << " Column: " << peek().column
              << std::endl;
    std::shared_ptr<Expression> left =
        std::make_shared<VariableExpression>(consume().value);

    if (!match(Dot)) {
      error("Expected '.' after variable");
    }

    std::string rightName = consumeIdentifier("Expected member name after '.'");
    std::shared_ptr<Expression> right =
        std::make_shared<VariableExpression>(rightName);

    if (match(LParen)) {
      std::cout << "Parsing dot access call expression, current token: "
                << peek().value << " Type: " << peek().type
                << " Line: " << peek().line << " Column: " << peek().column
                << std::endl;
      std::vector<std::shared_ptr<Expression>> arguments;
      while (!match(RParen)) {
        arguments.push_back(parseExpression());
        if (!match(Comma)) {
          break;
        }
      }
      right = std::make_shared<CallExpression>(
          rightName, std::vector<std::shared_ptr<TypeReference>>(), arguments);
    }

    return std::make_shared<DotAccessExpression>(left, right);
  }

  std::shared_ptr<Expression> parseExpression() {
    Token current = peek();
    std::cout << "Parsing expression, current token: " << current.value
              << " Type: " << current.type << " Line: " << current.line
              << " Column: " << current.column << std::endl;

    if (isBinaryOperator(current)) {
      return parseBinary();
    } else if (isCallExpression(current)) {
      return parseCall();
    } else if (isLiteralExpression(current)) {
      return std::make_shared<LiteralExpression>(consume().value);
    } else if (isVariableExpression(current)) {
      return std::make_shared<VariableExpression>(consume().value);
    } else if (isDotAccessExpression(current)) {
      return parseDotAccess();
    } else if (isSemicolon(current)) {
      consume();
      return nullptr;
    } else {
      error("Expected expression");
      return nullptr;
    }
  }

  std::shared_ptr<BinaryExpression> parseBinary() {
    std::cout << "Parsing binary expression, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    std::shared_ptr<Expression> left = parseExpression();
    std::string op = consume().value;
    std::shared_ptr<Expression> right = parseExpression();
    return std::make_shared<BinaryExpression>(left, op, right);
  }

  std::shared_ptr<CallExpression> parseCall() {
    std::cout << "Parsing call expression, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    std::string callee =
        consumeIdentifier("Expected function name in call expression");
    std::vector<std::shared_ptr<Expression>> arguments;
    consume(); // Consume '('
    while (!match(RParen)) {
      arguments.push_back(parseExpression());
      match(Comma);
    }

    std::vector<std::shared_ptr<TypeReference>> genericTypes;

    return std::make_shared<CallExpression>(callee, (genericTypes), arguments);
  }

  std::shared_ptr<TypeReference> parseTypeReference() {
    std::cout << "Parsing type reference, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    std::string name = consumeIdentifier("Expected type name");
    std::vector<std::shared_ptr<TypeReference>> genericTypes;
    if (match(LBracket)) {
      while (!match(RBracket)) {
        genericTypes.push_back(parseTypeReference());
      }
    }
    return std::make_shared<TypeReference>(name, genericTypes);
  }

  std::shared_ptr<Parameter> parseParameter() {
    std::cout << "Parsing parameter, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    std::string name = consumeIdentifier("Expected parameter name");
    consume(); // Consume ':'
    auto type = parseTypeReference();
    return std::make_shared<Parameter>(name, type);
  }

  std::vector<std::shared_ptr<Parameter>> parseParameters() {
    std::cout << "Parsing parameters, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    std::vector<std::shared_ptr<Parameter>> parameters;
    consume(); // Consume '('
    while (!match(RParen)) {
      parameters.push_back(parseParameter());
      match(Comma);
    }
    return parameters;
  }

  std::vector<std::shared_ptr<AstNode>> parseDeclarationsUntil(Token endToken) {
    std::cout << "Parsing declarations until Line:" << endToken.line
              << " Column:" << endToken.column << std::endl;
    std::vector<std::shared_ptr<AstNode>> declarations;
    Token current = peek();
    bool isEnd = false;
    while (!isEnd) {
      declarations.push_back(parseDeclaration());
      if (isSemicolon(peek())) {
        consume();
      }
      current = peek();
      if (current.line == endToken.line && current.column == endToken.column) {
        isEnd = true;
      }
    }
    return declarations;
  }

  std::string consumeIdentifier(const std::string &errorMessage) {
    std::cout << "Consuming identifier, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    if (isIdentifier(peek())) {
      return consume().value;
    } else {
      error(errorMessage);
      return "";
    }
  }

  void consumeSemicolon() {
    std::cout << "Consuming semicolon, current token: " << peek().value
              << " Type: " << peek().type << " Line: " << peek().line
              << " Column: " << peek().column << std::endl;
    if (!match(Semicolon)) {
      error("Expected semicolon");
    }
  }

  void error(const std::string &message) const {
    std::cerr << "Error: " << message << std::endl
              << "Current ->" << " Value:" << peek().value
              << " TokenType:" << std::to_string(peek().type)
              << " Line:" << std::to_string(peek().line)
              << " Column:" << std::to_string(peek().column)
              << " ID:" << std::to_string(position) << std::endl;

    if (position + 1 < tokens.size()) {
      std::cerr << "Next -> Value:" << tokens[position + 1].value
                << " TokenType:" << std::to_string(tokens[position + 1].type)
                << " Line:" << std::to_string(tokens[position + 1].line)
                << " Column:" << std::to_string(tokens[position + 1].column)
                << " ID:" << std::to_string(position + 1) << std::endl;
    }
    exit(1);
  }

  bool isBinaryOperator(Token token) const {
    for (size_t i = position; i < tokens.size(); i++) {
      std::cout << "Checking token: " << tokens[i].value
                << " Type: " << tokens[i].type << " Line: " << tokens[i].line
                << " Column: " << tokens[i].column << std::endl;
      if (tokens[i].line == peek().line) {
        if (tokens[i].type == Semicolon) {
          break;
        } else if (tokens[i].type == LParen) {
          break;
        } else if (tokens[i].type == RParen) {
          break;
        } else if (tokens[i].type == Plus) {
          return true;
        } else if (tokens[i].type == Minus) {
          return true;
        } else if (tokens[i].type == Multiply) {
          return true;
        } else if (tokens[i].type == Divide) {
          return true;
        }
      } else {
        std ::cout << "Line mismatch" << std::endl;
        break;
      }
    }
    std::cout << "Not a binary expression" << std::endl;
    return false;
  }

  bool isCallExpression(Token token) {
    for (size_t i = position; i < tokens.size(); i++) {
      std::cout << "Checking token: " << tokens[i].value
                << " Type: " << tokens[i].type << " Line: " << tokens[i].line
                << " Column: " << tokens[i].column << std::endl;
      if (tokens[i].line == peek().line) {
        if (tokens[i].type == NewKeyword) {
          break;
        } else if (tokens[i].type == Semicolon) {
          break;
        } else if (tokens[i].type == LParen) {
          return true;
        }
      } else {
        std ::cout << "Line mismatch" << std::endl;
        break;
      }
    }
    std::cout << "Not a call expression" << std::endl;
    return false;
  }

  Token GetLineTermination() {
    for (size_t i = position; i < tokens.size(); i++) {
      std::cout << "Checking token: " << tokens[i].value
                << " Type: " << tokens[i].type << " Line: " << tokens[i].line
                << " Column: " << tokens[i].column << std::endl;
      if (tokens[i].line == peek().line) {
        if (tokens[i].type == Semicolon) {
          return tokens[i];
        }
      } else {
        std::cout << "Line mismatch" << std::endl;
        break;
      }
    }
    return tokens[tokens.size()];
  }

  bool isDotAccessExpression(Token token) {
    for (size_t i = position; i < tokens.size(); i++) {
      std::cout << "Checking token: " << tokens[i].value
                << " Type: " << tokens[i].type << " Line: " << tokens[i].line
                << " Column: " << tokens[i].column << std::endl;
      if (tokens[i].line == peek().line) {
        if (tokens[i].type == Semicolon) {
          break;
        } else if (tokens[i].type == LParen) {
          break;
        } else if (tokens[i].type == RParen) {
          break;
        } else if (tokens[i].type == Dot) {
          return true;
        }
      } else {
        std ::cout << "Line mismatch" << std::endl;
        break;
      }
    }
    std::cout << "Not a dot access expression" << std::endl;
    return false;
  }

  bool isLiteralExpression(Token token) const {
    return token.type == StringLiteral || token.type == IntegerLiteral;
  }

  bool isVariableExpression(Token token) const {
    return token.type == Identifier;
  }

  bool isIdentifier(Token token) const { return token.type == Identifier; }

  bool isImport(Token token) const { return token.type == ImportKeyword; }

  bool isPackage(Token token) const { return token.type == PackageKeyword; }

  bool isMemberImport(Token token) const { return token.type == Identifier; }

  bool isClass(Token token) const { return token.type == ClassKeyword; }

  bool isFunction(Token token) const { return token.type == FunctionKeyword; }

  bool isVariable(Token token) const { return token.type == VarKeyword; }

  bool isReturn(Token token) const { return token.type == ReturnKeyword; }

  bool isIf(Token token) const { return token.type == IfKeyword; }

  bool isFor(Token token) const { return token.type == ForKeyword; }

  bool isWhile(Token token) const { return token.type == WhileKeyword; }

  bool isBlock(Token token) const { return token.type == LBrace; }

  bool isSemicolon(Token token) const { return token.type == Semicolon; }
};

#endif // PARSER_H
