#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <vector>

class ForStatement : public AstNode {
public:
  AstNode *Initializer;
  Expression *Condition;
  AstNode *Increment;
  std::vector<AstNode *> Body;

  ForStatement(AstNode *initializer, Expression *condition, AstNode *increment,
               std::vector<AstNode *> &body)
      : Initializer(initializer), Condition(condition), Increment(increment),
        Body(body) {
    if (Initializer)
      Initializer->addRef();
    if (Condition)
      Condition->addRef();
    if (Increment)
      Increment->addRef();
    for (auto &i : Body) {
      if (i)
        i->addRef();
    }
  }

  ~ForStatement() {
    if (Initializer)
      Initializer->release();
    if (Condition)
      Condition->release();
    if (Increment)
      Increment->release();
    for (auto &i : Body) {
      if (i)
        i->release();
    }
  }
};
#endif // FOR_STATEMENT_H