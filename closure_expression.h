#ifndef CLOSURE_EXPRESSION_H
#define CLOSURE_EXPRESSION_H

#include "ast_node.h"
#include "expression.h"
#include "parameter.h"
#include "type_reference.h"
#include <string>
#include <vector>

class ClosureExpression : public Expression {
public:
  std::vector<Parameter *> Parameters;
  TypeReference *ReturnType;
  std::vector<AstNode *> Body;
  std::vector<TypeReference *> GenericTypes;

  ClosureExpression(std::vector<Parameter *> &parameters,
                    TypeReference *returnType, std::vector<AstNode *> &body,
                    std::vector<TypeReference *> &genericTypes)
      : Parameters(parameters), ReturnType(returnType), Body(body),
        GenericTypes(genericTypes) {
    for (auto &i : Parameters) {
      if (i)
        i->addRef();
    }
    if (ReturnType)
      ReturnType->addRef();
    for (auto &i : Body) {
      if (i)
        i->addRef();
    }
    for (auto &i : GenericTypes) {
      if (i)

        i->addRef();
    }
  }

  ~ClosureExpression() {
    for (auto &i : Parameters) {
      if (i)
        i->release();
    }
    if (ReturnType)
      ReturnType->release();
    for (auto &i : Body) {
      if (i)
        i->release();
    }
    for (auto &i : GenericTypes) {
      if (i)
        i->release();
    }
  }
};

#endif