#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <memory>
#include <vector>


class ForStatement : public AstNode {
public:
  std::shared_ptr<AstNode> Initializer;
  std::shared_ptr<Expression> Condition;
  std::shared_ptr<AstNode> Increment;
  std::vector<std::shared_ptr<AstNode>> Body;

  ForStatement(std::shared_ptr<AstNode> initializer,
               std::shared_ptr<Expression> condition,
               std::shared_ptr<AstNode> increment,
               const std::vector<std::shared_ptr<AstNode>> &body)
      : Initializer(initializer), Condition(condition), Increment(increment),
        Body(body) {}

  ~ForStatement() = default; // No need for manual memory management
};

#endif // FOR_STATEMENT_H