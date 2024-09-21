#ifndef VARIABLE_DECLARATION_H
#define VARIABLE_DECLARATION_H

#include "ast_node.h"
#include "expression.h"
#include "type_reference.h"
#include <memory>

class VariableDeclaration : public AstNode {
public:
  std::string Name;
  std::shared_ptr<TypeReference> Type;
  std::shared_ptr<Expression> Value;

  VariableDeclaration(const std::string &name,
                      std::shared_ptr<TypeReference> type,
                      std::shared_ptr<Expression> value)
      : Name(name), Type(type), Value(value) {}

  ~VariableDeclaration() = default; // No need for manual memory management
};

#endif // VARIABLE_DECLARATION_H