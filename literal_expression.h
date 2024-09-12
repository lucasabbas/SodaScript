#ifndef LITERAL_EXPRESSION_H
#define LITERAL_EXPRESSION_H

#include "expression.h"
#include <string>

class LiteralExpression : public Expression {
public:
  std::string Value;

  LiteralExpression(const std::string &value) : Value(value) {}
};

#endif // LITERAL_EXPRESSION_H