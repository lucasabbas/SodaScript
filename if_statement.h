#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <memory>
#include <vector>


class IfStatement : public AstNode {
public:
  std::shared_ptr<Expression> Condition;
  std::vector<std::shared_ptr<AstNode>> ThenBody;
  std::vector<std::shared_ptr<AstNode>> ElseBody;

  IfStatement(std::shared_ptr<Expression> condition,
              const std::vector<std::shared_ptr<AstNode>> &thenBody,
              const std::vector<std::shared_ptr<AstNode>> &elseBody)
      : Condition(condition), ThenBody(thenBody), ElseBody(elseBody) {}

  ~IfStatement() = default; // No need for manual memory management
};

#endif // IF_STATEMENT_H