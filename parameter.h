#ifndef PARAMETER_H
#define PARAMETER_H

#include "ast_node.h"
#include "type_reference.h"
#include <string>
#include <vector>

class Parameter : public AstNode {
public:
  std::string Name;
  TypeReference *Type;

  Parameter(std::string &name, TypeReference *type) : Name(name), Type(type) {
    if (Type)
      Type->addRef();
  }
  ~Parameter() {
    if (Type)
      Type->release();
  }
};

#endif