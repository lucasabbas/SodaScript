#ifndef TYPE_REFERENCE_H
#define TYPE_REFERENCE_H

#include "ast_node.h"
#include <string>
#include <vector>

class TypeReference : public AstNode {
public:
  std::string Name;
  std::vector<TypeReference *> GenericTypes;

  TypeReference(const std::string &name,
                const std::vector<TypeReference *> &genericTypes)
      : Name(name), GenericTypes(genericTypes) {
    for (auto &type : GenericTypes) {
      if (type)
        type->addRef();
    }
  }

  ~TypeReference() {
    for (auto &type : GenericTypes) {
      if (type)
        type->release();
    }
  }
};

#endif