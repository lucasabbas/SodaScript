#ifndef PACKAGE_IMPORT_STATEMENT_H
#define PACKAGE_IMPORT_STATEMENT_H

#include "ast_node.h"
#include <string>

class PackageImportStatement : public AstNode {
public:
  std::string PackageName;

  PackageImportStatement(std::string &packageName) : PackageName(packageName) {}
};

#endif // PACKAGE_IMPORT_STATEMENT_H