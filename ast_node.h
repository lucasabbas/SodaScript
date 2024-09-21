#ifndef ASTNODE_H
#define ASTNODE_H

#include "ref_counted.h"
#include <string>
#include <vector>

// Base class for all AST nodes
class AstNode {
public:
  virtual ~AstNode() {}
};

#endif // ASTNODE_H
