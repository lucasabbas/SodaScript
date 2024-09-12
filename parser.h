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
#include <string>
#include <vector>

class Parser {
public:
  Parser(const std::string &input) {
    Tokenizer tokenizer;
    tokens = tokenizer.tokenize(input);
    position = 0;
  }

  Parser(std::vector<Token> _tokens) : tokens(_tokens), position(0) {}

  ~Parser() {}

  Package parse() {
    std::string name = "";
    std::vector<AstNode *> members;
    std::vector<PackageImportStatement *> packageImports;
    std::vector<MemberImportStatement *> memberImports;

    while (!isAtEnd()) {
      if (isImport(peek())) {
        packageImports.push_back(parseImport(tokens, position));
      } else if (isIdentifier(peek())) {
        memberImports.push_back(parseMemberImport(tokens, position));
      } else if (isPackage(peek())) {
        consume();
        if (isIdentifier(peek())) {
          name = consume().value;
        }

        if (isBlock(peek())) {
          consume();
          while (!isAtEnd() && !match(RBrace)) {
            members.push_back(parseDeclaration(tokens, position));
          }
        } else if (isSemicolon(peek())) {
          // Ensure we handle semicolons properly
          consume();
          continue; // No expression expected here, so continue to the next
                    // token
        } else {
          error("Expected block or semicolon after package declaration -> "
                "Value:" +
                peek().value + " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
        }
      } else {
        members.push_back(parseDeclaration(tokens, position));
      }
    }

    return Package(name, members, packageImports, memberImports);
  }

  AstNode *parseDeclaration(std::vector<Token> _tokens, size_t _position = 0) {
    if (isClass(peek())) {
      return new ClassDeclaration(parseClass(_tokens, _position));
    } else if (isFunction(peek())) {
      return new FunctionDeclaration(parseFunction(_tokens, _position));
    } else if (isVariable(peek())) {
      return new VariableDeclaration(parseVariable(_tokens, _position));
    } else if (isReturn(peek())) {
      return new ReturnStatement(parseReturn(_tokens, _position));
    } else if (isIf(peek())) {
      return new IfStatement(parseIf(_tokens, _position));
    } else if (isFor(peek())) {
      return new ForStatement(parseFor(_tokens, _position));
    } else if (isWhile(peek())) {
      return new WhileStatement(parseWhile(_tokens, _position));
    } else {
      Expression *e = new Expression(parseDotAccess(_tokens, _position));
      if (isSemicolon(peek())) {
        // Ensure we handle semicolons properly
        consume();
      } else {
        error("Expected semicolon after expression -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
      return e;
    }
  }

  PackageImportStatement *parseImport(std::vector<Token>, size_t position = 0) {
    std::string path = "";
    if (isImport(peek())) {
      consume();
      if (isIdentifier(peek())) {
        path = consume().value;
        if (isSemicolon(peek())) {
          // Ensure we handle semicolons properly
          consume();
        } else {
          error("Expected semicolon after import statement -> Value:" +
                peek().value + " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
        }
      }
    }
    return new PackageImportStatement(path);
  }

  MemberImportStatement *parseMemberImport(std::vector<Token>,
                                           size_t position = 0) {
    std::string packageName = "";
    std::string memberName = "";
    if (isIdentifier(peek())) {
      packageName = consume().value;
      if (isDotAccessExpression(peek())) {
        consume();
        if (isIdentifier(peek())) {
          memberName = consume().value;
          if (isSemicolon(peek())) {
            // Ensure we handle semicolons properly
            consume();
          } else {
            error("Expected semicolon after member import statement -> Value:" +
                  peek().value + " TokenType:" + std::to_string(peek().type) +
                  " Line:" + std::to_string(peek().line) +
                  " Column:" + std::to_string(peek().column));
          }
        }
      }
    }
    return new MemberImportStatement(packageName, memberName);
  }

  ClassDeclaration parseClass(std::vector<Token>, size_t position = 0) {
    std::string name = "";
    std::string parent = "";
    std::vector<AstNode> members;
    bool isStatic = false;

    if (isStaticKeyword(peek())) {
      consume();
      isStatic = true;
    }

    if (isClass(peek())) {
      consume();
      if (isIdentifier(peek())) {
        name = consume().value;
      }

      if (isExtendsKeyword(peek())) {
        consume();
        if (isIdentifier(peek())) {
          parent = consume().value;
        }
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          if (isFunction(peek())) {
            members.push_back(parseFunction(tokens, position));
          } else if (isVariable(peek())) {
            members.push_back(parseVariable(tokens, position));
          } else {
            error(
                "Expected method or field declaration in class body -> Value:" +
                peek().value + " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
          }
        }
      } else {
        error("Expected block after class declaration -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected class declaration -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    std::vector<AstNode *> membersPtr;
    for (size_t i = 0; i < members.size(); ++i) {
      membersPtr.push_back(&members[i]);
    }

    return ClassDeclaration(
        name, isStatic,
        new TypeReference(parent, std::vector<TypeReference *>()),
        std::vector<TypeReference *>(), membersPtr);
  }

  FunctionDeclaration parseFunction(std::vector<Token>, size_t position = 0) {
    std::string name = "";
    std::vector<Parameter> parameters;
    TypeReference returnType =
        TypeReference("", std::vector<TypeReference *>());
    std::vector<AstNode *> body;

    if (isFunction(peek())) {
      consume();
      if (isIdentifier(peek())) {
        name = consume().value;
      }

      if (isParameterList(peek())) {
        consume();
        while (!isAtEnd() && !match(RParen)) {
          parameters.push_back(parseParameter(tokens, position));
          if (!match(Comma)) {
            break;
          }
        }
      }

      if (isArrow(peek())) {
        consume();
        if (isTypeReference(peek())) {
          returnType = parseTypeReference(tokens, position);
        }
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          body.push_back(parseDeclaration(tokens, position));
        }
      } else {
        error("Expected block after function declaration -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected function declaration -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    TypeReference *returnTypePtr = &returnType;

    std::vector<Parameter *> parametersPtr;
    for (size_t i = 0; i < parameters.size(); ++i) {
      parametersPtr.push_back(&parameters[i]);
    }

    return FunctionDeclaration(name, parametersPtr, returnTypePtr, body,
                               std::vector<TypeReference *>());
  }

  VariableDeclaration parseVariable(std::vector<Token>, size_t position = 0) {
    std::string name = "";
    TypeReference type = TypeReference("", std::vector<TypeReference *>());
    Expression value = Expression();

    if (isVariable(peek())) {
      consume();
      if (isIdentifier(peek())) {
        name = consume().value;
      }

      if (isColon(peek())) {
        consume();
        if (isTypeReference(peek())) {
          type = parseTypeReference(tokens, position);
        }
      }

      if (isAssign(peek())) {
        consume();
        value = parseExpression(tokens, position);
      }

      if (isSemicolon(peek())) {
        consume();
      } else {
        error("Expected semicolon after variable declaration -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected variable declaration -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    TypeReference *typePtr = &type;

    Expression *valuePtr = &value;

    return VariableDeclaration(name, typePtr, valuePtr);
  }

  ReturnStatement parseReturn(std::vector<Token>, size_t position = 0) {
    Expression value = Expression();

    if (isReturn(peek())) {
      consume();
      if (isExpression(peek())) {
        value = parseExpression(tokens, position);
      }
    } else {
      error("Expected return statement -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    Expression *valuePtr = &value;

    return ReturnStatement(valuePtr);
  }

  IfStatement parseIf(std::vector<Token>, size_t position = 0) {
    Expression condition = Expression();
    std::vector<AstNode *> thenBody;
    std::vector<AstNode *> elseBody;

    if (isIf(peek())) {
      consume();
      if (isExpression(peek())) {
        condition = parseExpression(tokens, position);
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          thenBody.push_back(parseDeclaration(tokens, position));
        }
      } else {
        error("Expected block after if condition -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isElseKeyword(peek())) {
        consume();
        if (isBlock(peek())) {
          consume();
          while (!isAtEnd() && !match(RBrace)) {
            elseBody.push_back(parseDeclaration(tokens, position));
          }
        } else {
          error("Expected block after else keyword -> Value:" + peek().value +
                " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
        }
      }
    } else {
      error("Expected if statement -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    Expression *conditionPtr = &condition;

    return IfStatement(conditionPtr, thenBody, elseBody);
  }

  ForStatement parseFor(std::vector<Token>, size_t position = 0) {
    AstNode *initializer;
    Expression *condition;
    AstNode *increment;
    std::vector<AstNode *> body;

    if (isFor(peek())) {
      consume();
      if (isExpression(peek())) {
        initializer = new Expression(parseExpression(tokens, position));
      } else {
        error("Expected expression after for keyword -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isSemicolon(peek())) {
        consume();
        if (isExpression(peek())) {
          condition = new Expression(parseExpression(tokens, position));
        } else {
          error("Expected expression after semicolon -> Value:" + peek().value +
                " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
        }
      } else {
        error("Expected semicolon after initializer -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isSemicolon(peek())) {
        consume();
        if (isExpression(peek())) {
          increment = new Expression(parseExpression(tokens, position));
        } else {
          error("Expected expression after semicolon -> Value:" + peek().value +
                " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
        }
      } else {
        error("Expected semicolon after condition -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          body.push_back(parseDeclaration(tokens, position));
        }
      } else {
        error("Expected block after for statement -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected for statement -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    return ForStatement(initializer, condition, increment, body);
  }

  WhileStatement parseWhile(std::vector<Token>, size_t position = 0) {
    Expression *condition;
    std::vector<AstNode *> body;

    if (isWhile(peek())) {
      consume();
      if (isExpression(peek())) {
        condition = new Expression(parseExpression(tokens, position));
      } else {
        error("Expected expression after while keyword -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          body.push_back(parseDeclaration(tokens, position));
        }
      } else {
        error("Expected block after while condition -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected while statement -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    return WhileStatement(condition, body);
  }

  Expression parseExpression(std::vector<Token>, size_t position = 0) {
    if (isBinaryExpression(peek())) {
      return Expression(parseBinary(tokens, position));
    } else if (isCallExpression(peek())) {
      return Expression(parseCall(tokens, position));
    } else if (isLiteralExpression(peek())) {
      return Expression(parseLiteral(tokens, position));
    } else if (isVariableExpression(peek())) {
      return Expression(parseVariableExpression(tokens, position));
    } else if (isClosureExpression(peek())) {
      return Expression(parseClosure(tokens, position));
    } else if (isLambdaExpression(peek())) {
      return Expression(parseLambda(tokens, position));
    } else if (isConstructorCallExpression(peek())) {
      return Expression(parseConstructorCall(tokens, position));
    } else {
      error("Expected expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
      return Expression();
    }
  }

  BinaryExpression parseBinary(std::vector<Token>, size_t position = 0) {
    Expression left = Expression();
    std::string op = "";
    Expression right = Expression();

    if (isBinaryOperator(peek())) {
      if (isExpression(peek())) {
        left = parseExpression(tokens, position);
      } else {
        error("Expected expression before binary operator -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isBinaryOperator(peek())) {
        op = consume().value;
      } else {
        error("Expected binary operator -> Value:" + peek().value +
              " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }

      if (isExpression(peek())) {
        right = parseExpression(tokens, position);
      } else {
        error("Expected expression after binary operator -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected binary expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    Expression *leftPtr = &left;
    Expression *rightPtr = &right;

    return BinaryExpression(leftPtr, op, rightPtr);
  }

  CallExpression parseCall(std::vector<Token>, size_t position = 0) {
    std::string callee = "";
    std::vector<Expression> arguments;

    if (isCallExpression(peek())) {
      consume();
      if (isIdentifier(peek())) {
        callee = consume().value;
      }

      if (isParameterList(peek())) {
        consume();
        while (!isAtEnd() && !match(RParen)) {
          arguments.push_back(parseExpression(tokens, position));
          if (!match(Comma)) {
            break;
          }
        }
        if (peek().type == RParen) {
          consume();
        } else {
          error("Expected closing parenthesis after call expression -> Value:" +
                peek().value + " TokenType:" + std::to_string(peek().type) +
                " Line:" + std::to_string(peek().line) +
                " Column:" + std::to_string(peek().column));
        }
      }
    } else {
      error("Expected call expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    std::vector<Expression *> argumentsPtr;
    for (size_t i = 0; i < arguments.size(); ++i) {
      argumentsPtr.push_back(&arguments[i]);
    }

    return CallExpression(callee, std::vector<TypeReference *>(), argumentsPtr);
  }

  Expression parseDotAccess(std::vector<Token> tokens, size_t position = 0) {
    // Parse the left-hand side (e.g., `Sys`)
    Expression left = parseExpression(tokens, position);

    // Check if there is a dot ('.') for a dot access
    while (peek().type == Dot) {
      consume(); // Consume the '.'

      // Parse the right-hand side (e.g., `print`)
      Expression right = parseExpression(tokens, position);

      // Wrap both sides in a DotAccessExpression
      left = DotAccessExpression(&left, &right);
    }

    return left;
  }

  LiteralExpression parseLiteral(std::vector<Token>, size_t position = 0) {
    Token token = peek();
    if (isLiteralExpression(token)) {
      consume();
      return LiteralExpression(token.value);
    } else {
      error("Expected literal expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }
    return LiteralExpression("");
  }

  VariableExpression parseVariableExpression(std::vector<Token>,
                                             size_t position = 0) {
    std::string name = "";
    if (isVariableExpression(peek())) {
      name = consume().value;
    } else {
      error("Expected variable expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }
    return VariableExpression(name);
  }

  ClosureExpression parseClosure(std::vector<Token>, size_t position = 0) {
    std::vector<Parameter> parameters;
    TypeReference *returnType;
    std::vector<AstNode *> body;

    if (isClosureExpression(peek())) {
      consume();
      if (isParameterList(peek())) {
        consume();
        while (!isAtEnd() && !match(RParen)) {
          parameters[parameters.size()] = parseParameter(tokens, position);
          if (!match(Comma)) {
            break;
          }
        }
      }

      if (isArrow(peek())) {
        consume();
        if (isTypeReference(peek())) {
          returnType = new TypeReference(parseTypeReference(tokens, position));
        }
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          body.push_back(parseDeclaration(tokens, position));
        }
      } else {
        error("Expected block after closure declaration -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected closure expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    std::vector<Parameter *> parametersPtr;
    for (size_t i = 0; i < parameters.size(); ++i) {
      parametersPtr.push_back(&parameters[i]);
    }

    std::vector<TypeReference *> genericTypes = std::vector<TypeReference *>();

    return ClosureExpression(parametersPtr, returnType, body, genericTypes);
  }

  LambdaExpression parseLambda(std::vector<Token>, size_t position = 0) {
    std::vector<Parameter> parameters;
    AstNode *body;
    TypeReference *returnType;
    std::vector<TypeReference *> genericTypes;

    if (isLambdaExpression(peek())) {
      consume();
      if (isParameterList(peek())) {
        consume();
        while (!isAtEnd() && !match(RParen)) {
          parameters.push_back(parseParameter(tokens, position));
          if (!match(Comma)) {
            break;
          }
        }
      }

      if (isArrow(peek())) {
        consume();
        if (isTypeReference(peek())) {
          returnType = new TypeReference(parseTypeReference(tokens, position));
        }
      }

      if (isBlock(peek())) {
        consume();
        while (!isAtEnd() && !match(RBrace)) {
          body = parseDeclaration(tokens, position);
        }
      } else {
        error("Expected block after lambda declaration -> Value:" +
              peek().value + " TokenType:" + std::to_string(peek().type) +
              " Line:" + std::to_string(peek().line) +
              " Column:" + std::to_string(peek().column));
      }
    } else {
      error("Expected lambda expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    std::vector<Parameter *> parametersPtr;
    for (size_t i = 0; i < parameters.size(); ++i) {
      parametersPtr.push_back(&parameters[i]);
    }

    return LambdaExpression(parametersPtr, body, returnType, genericTypes);
  }

  ConstructorCallExpression parseConstructorCall(std::vector<Token>,
                                                 size_t position = 0) {
    std::string name;
    std::vector<TypeReference *> genericTypes;
    std::vector<Expression> arguments;

    if (isConstructorCallExpression(peek())) {
      consume();
      if (isIdentifier(peek())) {
        name = consume().value;
      }

      if (isParameterList(peek())) {
        consume();
        while (!isAtEnd() && !match(RParen)) {
          arguments.push_back(parseExpression(tokens, position));
          if (!match(Comma)) {
            break;
          }
        }
      }
    } else {
      error("Expected constructor call expression -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    std::vector<Expression *> argumentsPtr;
    for (size_t i = 0; i < arguments.size(); ++i) {
      argumentsPtr.push_back(&arguments[i]);
    }

    return ConstructorCallExpression(name, genericTypes, argumentsPtr);
  }

  TypeReference parseTypeReference(std::vector<Token>, size_t position = 0) {
    std::string name = "";
    std::vector<TypeReference> genericTypes;

    if (isTypeReference(peek())) {
      if (isIdentifier(peek())) {
        name = consume().value;
      }

      if (isTypeReferenceList(peek())) {
        consume();
        while (!isAtEnd() && !match(RBracket)) {
          genericTypes.push_back(parseTypeReference(tokens, position));
        }
      }
    } else {
      error("Expected type reference -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    std::vector<TypeReference *> genericTypesPtr;
    for (size_t i = 0; i < genericTypes.size(); ++i) {
      genericTypesPtr.push_back(&genericTypes[i]);
    }

    return TypeReference(name, genericTypesPtr);
  }

  Parameter parseParameter(std::vector<Token>, size_t position = 0) {
    std::string name = "";
    TypeReference type = TypeReference("", std::vector<TypeReference *>());

    if (isParameter(peek())) {
      if (isIdentifier(peek())) {
        name = consume().value;
      }

      if (isColon(peek())) {
        consume();
        if (isTypeReference(peek())) {
          type = parseTypeReference(tokens, position);
        }
      }
    } else {
      error("Expected parameter -> Value:" + peek().value +
            " TokenType:" + std::to_string(peek().type) +
            " Line:" + std::to_string(peek().line) +
            " Column:" + std::to_string(peek().column));
    }

    TypeReference *typePtr = &type;

    return Parameter(name, typePtr);
  }

private:
  std::vector<Token> tokens;
  size_t position;

  Token peek() { return tokens[position]; }

  Token consume() { return tokens[position++]; }

  bool match(SodaScriptToken type) {
    if (peek().type == type) {
      consume();
      return true;
    }
    return false;
  }

  bool isAtEnd() { return position >= tokens.size(); }

  void error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
    exit(1);
  }

  void synchronize() {
    while (!isAtEnd() && peek().type != Semicolon) {
      consume();
    }
  }

  bool isBinaryOperator(Token token) {
    switch (token.type) {
    case Plus:
    case Minus:
    case Multiply:
    case Divide:
    case Modulo:
    case Equals:
    case NotEquals:
    case LessThan:
    case GreaterThan:
    case LessThanOrEqual:
    case GreaterThanOrEqual:
    case Arrow:
      return true;
    default:
      return false;
    }
  }

  bool isUnaryOperator(Token token) {
    switch (token.type) {
    case Plus:
    case Minus:
    case Not:
      return true;
    default:
      return false;
    }
  }

  bool isLiteral(Token token) {
    switch (token.type) {
    case StringLiteral:
    case IntegerLiteral:
    case FloatLiteral:
    case LongLiteral:
    case DoubleLiteral:
    case BooleanLiteral:
      return true;
    default:
      return false;
    }
  }

  bool isType(Token token) {
    switch (token.type) {
    case StringType:
    case IntType:
    case FloatType:
    case LongType:
    case DoubleType:
    case NumberType:
    case AnyType:
    case ArrayType:
    case DictionaryType:
    case BoolType:
    case VoidType:
      return true;
    default:
      return false;
    }
  }

  bool isKeyword(Token token) {
    switch (token.type) {
    case PackageKeyword:
    case ImportKeyword:
    case PublicKeyword:
    case PrivateKeyword:
    case VarKeyword:
    case FuncKeyword:
    case FunctionKeyword:
    case ReturnKeyword:
    case IfKeyword:
    case ElseKeyword:
    case ForKeyword:
    case ForeachKeyword:
    case InKeyword:
    case ContinueKeyword:
    case BreakKeyword:
    case IsKeyword:
    case ClassKeyword:
    case ExtendsKeyword:
    case ConstructorKeyword:
    case StaticKeyword:
    case NewKeyword:
      return true;
    default:
      return false;
    }
  }

  bool isPunctuation(Token token) {
    switch (token.type) {
    case Plus:
    case Minus:
    case Multiply:
    case Divide:
    case Modulo:
    case Assign:
    case AssignRef:
    case Equals:
    case NotEquals:
    case LessThan:
    case GreaterThan:
    case LessThanOrEqual:
    case GreaterThanOrEqual:
    case Arrow:
    case Not:
    case Dot:
    case Comma:
    case Colon:
    case Semicolon:
    case Tilde:
    case LParen:
    case RParen:
    case LBracket:
    case RBracket:
    case LBrace:
    case RBrace:
    case DollarSign:
    case QuestionMark:
      return true;
    default:
      return false;
    }
  }

  bool isIdentifier(Token token) { return token.type == Identifier; }

  bool isOperator(Token token) {
    return isBinaryOperator(token) || isUnaryOperator(token);
  }

  bool isExpression(Token token) {
    return isLiteral(token) || isIdentifier(token) || isOperator(token);
  }

  bool isStatement(Token token) {
    return isKeyword(token) || isExpression(token);
  }

  bool isDeclaration(Token token) {
    return isKeyword(token) && token.type != ReturnKeyword;
  }

  bool isTypeReference(Token token) {
    return isType(token) || isIdentifier(token);
  }

  bool isParameter(Token token) {
    return isTypeReference(token) || isIdentifier(token);
  }

  bool isPackage(Token token) { return token.type == PackageKeyword; }

  bool isImport(Token token) { return token.type == ImportKeyword; }

  bool isMemberImport(Token token) { return isIdentifier(token); }

  bool isClass(Token token) { return token.type == ClassKeyword; }

  bool isFunction(Token token) { return token.type == FunctionKeyword; }

  bool isVariable(Token token) { return token.type == VarKeyword; }

  bool isReturn(Token token) { return token.type == ReturnKeyword; }

  bool isIf(Token token) { return token.type == IfKeyword; }

  bool isFor(Token token) { return token.type == ForKeyword; }

  bool isWhile(Token token) { return token.type == WhileKeyword; }

  bool isExpressionStatement(Token token) {
    return isExpression(token) && !isPunctuation(token);
  }

  bool isBinaryExpression(Token token) { return isBinaryOperator(token); }

  bool isCallExpression(Token token) {
    for (size_t i = position; i < tokens.size(); i++) {
      if (tokens[i].line == peek().line) {
        if (tokens[i].type == NewKeyword) {
          break;
        } else if (tokens[i].type == Semicolon) {
          break;
        } else if (tokens[i].type == LParen) {
          return true;
        }
      }
    }
    return false;
  }

  bool isDotAccessExpression(Token token) {
    for (size_t i = position; i < tokens.size(); i++) {
      if (tokens[i].line == peek().line) {
        if (tokens[i].type == Semicolon) {
          break;
        } else if (tokens[i].type == Dot) {
          return true;
        }
      }
    }
    return false;
  }

  bool isLiteralExpression(Token token) { return isLiteral(token); }

  bool isVariableExpression(Token token) { return isIdentifier(token); }

  bool isClosureExpression(Token token) { return token.type == LBrace; }

  bool isLambdaExpression(Token token) { return token.type == Arrow; }

  bool isConstructorCallExpression(Token token) {
    return token.type == NewKeyword;
  }

  bool isTypeReferenceExpression(Token token) { return isTypeReference(token); }

  bool isParameterList(Token token) { return token.type == LParen; }

  bool isTypeReferenceList(Token token) { return token.type == LBracket; }

  bool isBlock(Token token) { return token.type == LBrace; }

  bool isStatementList(Token token) { return isStatement(token); }

  bool isExpressionList(Token token) { return isExpression(token); }

  bool isSemicolon(Token token) { return token.type == Semicolon; }

  bool isExtendsKeyword(Token token) { return token.type == ExtendsKeyword; }

  bool isStaticKeyword(Token token) { return token.type == StaticKeyword; }

  bool isArrow(Token token) { return token.type == Arrow; }

  bool isColon(Token token) { return token.type == Colon; }

  bool isAssign(Token token) { return token.type == Assign; }

  bool isElseKeyword(Token token) { return token.type == ElseKeyword; }

  bool isForeachKeyword(Token token) { return token.type == ForeachKeyword; }
};

#endif // PARSER_H