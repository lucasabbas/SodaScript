#ifndef CALL_EXPRESSION_H
#define CALL_EXPRESSION_H

#include "expression.h"
#include "type_reference.h"
#include <memory>
#include <string>
#include <vector>

class CallExpression : public Expression {
public:
  std::string FunctionName;
  std::vector<std::shared_ptr<TypeReference>> GenericTypes;
  std::vector<std::shared_ptr<Expression>> Arguments;

  CallExpression(
      const std::string &functionName,
      const std::vector<std::shared_ptr<TypeReference>> &genericTypes,
      const std::vector<std::shared_ptr<Expression>> &arguments)
      : FunctionName(functionName), GenericTypes(genericTypes),
        Arguments(arguments) {}

  ~CallExpression() = default; // No need for manual memory management
};

#endif // CALLEXPRESSION_H