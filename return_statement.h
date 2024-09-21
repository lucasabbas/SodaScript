#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <memory>

class ReturnStatement : public AstNode {
public:
  std::shared_ptr<Expression> ReturnValue;

  ReturnStatement(std::shared_ptr<Expression> returnValue)
      : ReturnValue(returnValue) {}

  ~ReturnStatement() = default; // No need for manual memory management
};

#endif // RETURN_STATEMENT_H