#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "ast_node.h"
#include "expression.h"
#include <vector>

class IfStatement : public AstNode {
public:
  Expression *Condition;
  std::vector<AstNode *> ThenBody;
  std::vector<AstNode *> ElseBody;

  IfStatement(Expression *condition, std::vector<AstNode *> &thenBody,
              std::vector<AstNode *> &elseBody)
      : Condition(condition), ThenBody(thenBody), ElseBody(elseBody) {
    if (Condition)
      Condition->addRef();
    for (auto &i : ThenBody) {
      if (i)
        i->addRef();
    }
    for (auto &i : ElseBody) {
      if (i)
        i->addRef();
    }
  }

  ~IfStatement() {
    if (Condition)
      Condition->release();
    for (auto &i : ThenBody) {
      if (i)
        i->release();
    }
    for (auto &i : ElseBody) {
      if (i)
        i->release();
    }
  }
};

#endif // IF_STATEMENT_H