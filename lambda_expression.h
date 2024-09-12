#ifndef LAMBDA_EXPRESSION_H
#define LAMBDA_EXPRESSION_H

#include "ast_node.h"
#include "expression.h"
#include "parameter.h"
#include "type_reference.h"
#include <string>
#include <vector>

class LambdaExpression : public Expression {
public:
  std::vector<Parameter *> Parameters;
  AstNode *Body;
  TypeReference *ReturnType;
  std::vector<TypeReference *> GenericTypes;

  LambdaExpression(std::vector<Parameter *> &parameters, AstNode *body,
                   TypeReference *returnType,
                   std::vector<TypeReference *> &genericTypes)
      : Parameters(parameters), Body(body), ReturnType(returnType),
        GenericTypes(genericTypes) {
    for (auto &i : Parameters) {
      if (i)
        i->addRef();
    }
    if (Body)
      Body->addRef();
    if (ReturnType)
      ReturnType->addRef();
    for (auto &i : GenericTypes) {
      if (i)
        i->addRef();
    }
  }

  ~LambdaExpression() {
    for (auto &i : Parameters) {
      if (i)
        i->release();
    }
    if (Body)
      Body->release();
    if (ReturnType)
      ReturnType->release();
    for (auto &i : GenericTypes) {
      if (i)
        i->release();
    }
  }
};

#endif // LAMBDA_EXPRESSION_H