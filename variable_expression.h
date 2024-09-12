#ifndef VARIABLE_EXPRESSION_H
#define VARIABLE_EXPRESSION_H

#include "expression.h"
#include <string>

class VariableExpression : public Expression {
public:
  std::string Name;

  VariableExpression(const std::string &name) : Name(name) {}
};

#endif // VARIABLE_EXPRESSION_H