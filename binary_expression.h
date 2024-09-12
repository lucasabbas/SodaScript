#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "expression.h"
#include "ref_counted.h"

class BinaryExpression : public Expression {
public:
  Expression *Left;
  std::string Operator;
  Expression *Right;

  BinaryExpression(Expression *left, const std::string &op, Expression *right)
      : Left(left), Operator(op), Right(right) {
    if (Left)
      Left->addRef();
    if (Right)
      Right->addRef();
  }

  ~BinaryExpression() {
    if (Left)
      Left->release();
    if (Right)
      Right->release();
  }
};

#endif // BINARYEXPRESSION_H