#ifndef CALL_EXPRESSION_H
#define CALL_EXPRESSION_H

#include "expression.h"
#include "type_reference.h"
#include <string>
#include <vector>


class CallExpression : public Expression {
public:
  std::string FunctionName;
  std::vector<TypeReference *> GenericTypes;
  std::vector<Expression *> Arguments;

  CallExpression(const std::string &functionName,
                 const std::vector<TypeReference *> &genericTypes,
                 const std::vector<Expression *> &arguments)
      : FunctionName(functionName), GenericTypes(genericTypes),
        Arguments(arguments) {
    for (auto &type : GenericTypes) {
      if (type)
        type->addRef();
    }
    for (auto &arg : Arguments) {
      if (arg)
        arg->addRef();
    }
  }

  ~CallExpression() {
    for (auto &type : GenericTypes) {
      if (type)
        type->release();
    }
    for (auto &arg : Arguments) {
      if (arg)
        arg->release();
    }
  }
};

#endif // CALLEXPRESSION_H