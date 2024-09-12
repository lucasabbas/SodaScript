#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <vector>

class WhileStatement : public AstNode {
public:
  Expression *Condition;
  std::vector<AstNode *> Body;

  WhileStatement(Expression *condition, std::vector<AstNode *> &body)
      : Condition(condition), Body(body) {
    if (Condition)
      Condition->addRef();
    for (auto &i : Body) {
      if (i)
        i->addRef();
    }
  }

  ~WhileStatement() {
    if (Condition)
      Condition->release();
    for (auto &i : Body) {
      if (i)
        i->release();
    }
  }
};

#endif // WHILE_STATEMENT_H
