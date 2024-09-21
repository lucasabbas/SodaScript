#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "expression.h"
#include "ref_counted.h"

#include <memory>

class BinaryExpression : public Expression {
public:
  std::shared_ptr<Expression> Left;
  std::string Operator;
  std::shared_ptr<Expression> Right;

  BinaryExpression(std::shared_ptr<Expression> left, const std::string &op,
                   std::shared_ptr<Expression> right)
      : Left(left), Operator(op), Right(right) {}

  ~BinaryExpression() = default; // No need for manual memory management
};

#endif // BINARYEXPRESSION_H