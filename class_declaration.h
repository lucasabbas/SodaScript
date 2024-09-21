#ifndef CLASS_DECLARATION_H
#define CLASS_DECLARATION_H

#include "ast_node.h"
#include "type_reference.h"
#include <string>
#include <vector>

#include <memory>

class ClassDeclaration : public AstNode {
public:
  std::string Name;
  bool IsStatic;
  std::shared_ptr<TypeReference> BaseClass;
  std::vector<std::shared_ptr<TypeReference>> GenericTypes;
  std::vector<std::shared_ptr<AstNode>> Members;

  ClassDeclaration(
      const std::string &name, bool isStatic,
      std::shared_ptr<TypeReference> baseClass,
      const std::vector<std::shared_ptr<TypeReference>> &genericTypes,
      const std::vector<std::shared_ptr<AstNode>> &members)
      : Name(name), IsStatic(isStatic), BaseClass(baseClass),
        GenericTypes(genericTypes), Members(members) {}

  ~ClassDeclaration() = default; // No need for manual memory management
};

#endif