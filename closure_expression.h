#ifndef CLOSURE_EXPRESSION_H
#define CLOSURE_EXPRESSION_H

#include "ast_node.h"
#include "expression.h"
#include "parameter.h"
#include "type_reference.h"
#include <memory>
#include <string>
#include <vector>


class ClosureExpression : public Expression {
public:
  std::vector<std::shared_ptr<Parameter>> Parameters;
  std::shared_ptr<TypeReference> ReturnType;
  std::vector<std::shared_ptr<AstNode>> Body;
  std::vector<std::shared_ptr<TypeReference>> GenericTypes;

  ClosureExpression(
      const std::vector<std::shared_ptr<Parameter>> &parameters,
      std::shared_ptr<TypeReference> returnType,
      const std::vector<std::shared_ptr<AstNode>> &body,
      const std::vector<std::shared_ptr<TypeReference>> &genericTypes)
      : Parameters(parameters), ReturnType(returnType), Body(body),
        GenericTypes(genericTypes) {}

  ~ClosureExpression() = default; // No need for manual memory management
};

#endif