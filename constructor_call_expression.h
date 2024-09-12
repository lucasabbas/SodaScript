#ifndef CONSTRUCTOR_CALL_EXPRESSION_H
#define CONSTRUCTOR_CALL_EXPRESSION_H

#include "call_expression.h"
#include "expression.h"
#include "type_reference.h"
#include <string>
#include <vector>

class ConstructorCallExpression : public CallExpression {

public:
  ConstructorCallExpression(const std::string &className,
                            const std::vector<TypeReference *> &genericTypes,
                            const std::vector<Expression *> &arguments)
      : CallExpression(className, genericTypes, arguments) {}
};

#endif