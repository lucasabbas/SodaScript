#ifndef DOT_ACCESS_EXPRESSION_H
#define DOT_ACCESS_EXPRESSION_H

#include "expression.h"
#include <memory>

class DotAccessExpression : public Expression {
public:
  std::shared_ptr<Expression> Left;
  std::shared_ptr<Expression> Right;

  DotAccessExpression(std::shared_ptr<Expression> left,
                      std::shared_ptr<Expression> right)
      : Left(left), Right(right) {}

  ~DotAccessExpression() = default; // No need for manual memory management
};

#endif