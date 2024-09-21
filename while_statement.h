#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <memory>
#include <vector>

class WhileStatement : public AstNode {
public:
  std::shared_ptr<Expression> Condition;
  std::vector<std::shared_ptr<AstNode>> Body;

  WhileStatement(std::shared_ptr<Expression> condition,
                 const std::vector<std::shared_ptr<AstNode>> &body)
      : Condition(condition), Body(body) {}

  ~WhileStatement() = default; // No need for manual memory management
};

#endif // WHILE_STATEMENT_H
