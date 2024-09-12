#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "ast_node.h"
#include "expression.h"

class ReturnStatement : public AstNode {
public:
  Expression *ReturnValue;

  ReturnStatement(Expression *returnValue) : ReturnValue(returnValue) {
    if (ReturnValue)
      ReturnValue->addRef();
  }

  ~ReturnStatement() {
    if (ReturnValue)
      ReturnValue->release();
  }
};

#endif // RETURN_STATEMENT_H