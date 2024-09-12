#ifndef CLASS_DECLARATION_H
#define CLASS_DECLARATION_H

#include "ast_node.h"
#include "type_reference.h"
#include <string>
#include <vector>


class ClassDeclaration : public AstNode {
public:
  std::string Name;
  bool IsStatic;
  TypeReference *BaseClass;
  std::vector<TypeReference *> GenericTypes;
  std::vector<AstNode *> Members;

  ClassDeclaration(const std::string &name, bool isStatic,
                   TypeReference *baseClass,
                   const std::vector<TypeReference *> &genericTypes,
                   const std::vector<AstNode *> &members)
      : Name(name), IsStatic(isStatic), BaseClass(baseClass),
        GenericTypes(genericTypes), Members(members) {
    if (BaseClass)
      BaseClass->addRef();
    for (auto &type : GenericTypes) {
      if (type)
        type->addRef();
    }
    for (auto &member : Members) {
      if (member)
        member->addRef();
    }
  }

  ~ClassDeclaration() {
    if (BaseClass)
      BaseClass->release();
    for (auto &type : GenericTypes) {
      if (type)
        type->release();
    }
    for (auto &member : Members) {
      if (member)
        member->release();
    }
  }
};

#endif