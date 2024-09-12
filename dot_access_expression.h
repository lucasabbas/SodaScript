#ifndef DOT_ACCESS_EXPRESSION_H
#define DOT_ACCESS_EXPRESSION_H

#include "expression.h"

class DotAccessExpression : public Expression {
public:
  Expression *Left;
  Expression *Right;

  DotAccessExpression(Expression *left, Expression *right)
      : Left(left), Right(right) {
    if (Left)
      Left->addRef();
    if (Right)
      Right->addRef();
  }

  ~DotAccessExpression() {
    if (Left)
      Left->release();
    if (Right)
      Right->release();
  }
};

#endif