#ifndef FUNCTION_DECLARATION_H
#define FUNCTION_DECLARATION_H

#include "ast_node.h"
#include "parameter.h"
#include "type_reference.h"
#include <memory>
#include <string>
#include <vector>


class FunctionDeclaration : public AstNode {
public:
  std::string Name;
  std::vector<std::shared_ptr<Parameter>> Parameters;
  std::shared_ptr<TypeReference> ReturnType;
  std::vector<std::shared_ptr<AstNode>> Body;
  std::vector<std::shared_ptr<TypeReference>> GenericTypes;

  FunctionDeclaration(
      const std::string &name,
      const std::vector<std::shared_ptr<Parameter>> &parameters,
      std::shared_ptr<TypeReference> returnType,
      const std::vector<std::shared_ptr<AstNode>> &body,
      const std::vector<std::shared_ptr<TypeReference>> &genericTypes)
      : Name(name), Parameters(parameters), ReturnType(returnType), Body(body),
        GenericTypes(genericTypes) {}

  ~FunctionDeclaration() = default; // No need for manual memory management
};

#endif // FUNCTION_DECLARATION_H
