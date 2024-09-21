#ifndef PARAMETER_H
#define PARAMETER_H

#include "ast_node.h"
#include "type_reference.h"
#include <memory>
#include <string>
#include <vector>


class Parameter : public AstNode {
public:
  std::string Name;
  std::shared_ptr<TypeReference> Type;

  Parameter(const std::string &name, std::shared_ptr<TypeReference> type)
      : Name(name), Type(type) {}

  ~Parameter() = default; // No need for manual memory management
};

#endif