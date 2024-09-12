#ifndef VARIABLE_DECLARATION_H
#define VARIABLE_DECLARATION_H

#include "ast_node.h"
#include "expression.h"
#include "type_reference.h"

class VariableDeclaration : public AstNode {
public:
  std::string Name;
  TypeReference *Type;
  Expression *Value;

  VariableDeclaration(std::string &name, TypeReference *type, Expression *value)
      : Name(name), Type(type), Value(value) {
    if (Type)
      Type->addRef();
    if (Value)
      Value->addRef();
  }
  ~VariableDeclaration() {
    if (Type)
      Type->release();
    if (Value)
      Value->release();
  }
};
#endif // VARIABLE_DECLARATION_H