#ifndef TYPE_REFERENCE_H
#define TYPE_REFERENCE_H

#include "ast_node.h"
#include <string>
#include <vector>

#include <memory>

class TypeReference : public AstNode {
public:
  std::string Name;
  std::vector<std::shared_ptr<TypeReference>> GenericTypes;

  TypeReference(const std::string &name,
                const std::vector<std::shared_ptr<TypeReference>> &genericTypes)
      : Name(name), GenericTypes(genericTypes) {}

  ~TypeReference() = default; // No need for manual memory management
};

#endif