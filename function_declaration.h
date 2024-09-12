#ifndef FUNCTION_DECLARATION_H
#define FUNCTION_DECLARATION_H

#include "ast_node.h"
#include "parameter.h"
#include "type_reference.h"
#include <string>
#include <vector>

class FunctionDeclaration : public AstNode {
public:
  std::string Name;
  std::vector<Parameter *> Parameters;
  TypeReference *ReturnType;
  std::vector<AstNode *> Body;
  std::vector<TypeReference *> GenericTypes;

  FunctionDeclaration(const std::string &name,
                      const std::vector<Parameter *> &parameters,
                      TypeReference *returnType,
                      const std::vector<AstNode *> &body,
                      const std::vector<TypeReference *> &genericTypes)
      : Name(name), Parameters(parameters), ReturnType(returnType), Body(body),
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

  ~FunctionDeclaration() {
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

#endif // FUNCTION_DECLARATION_H
