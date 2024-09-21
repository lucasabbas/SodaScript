#ifndef CONSTRUCTOR_CALL_EXPRESSION_H
#define CONSTRUCTOR_CALL_EXPRESSION_H

#include "call_expression.h"
#include "expression.h"
#include "type_reference.h"
#include <memory>
#include <string>
#include <vector>

class ConstructorCallExpression : public CallExpression {
public:
  ConstructorCallExpression(
      const std::string &className,
      const std::vector<std::shared_ptr<TypeReference>> &genericTypes,
      const std::vector<std::shared_ptr<Expression>> &arguments)
      : CallExpression(className, genericTypes, arguments) {}

  ~ConstructorCallExpression() =
      default; // No need for manual memory management
};

#endif