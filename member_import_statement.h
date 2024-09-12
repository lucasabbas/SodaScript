#ifndef MEMBER_IMPORT_STATEMENT_H
#define MEMBER_IMPORT_STATEMENT_H

#include "ast_node.h"
#include <string>

class MemberImportStatement : public AstNode {
public:
  std::string PackageName;
  std::string MemberName;

  MemberImportStatement(std::string &packageName, std::string &memberName)
      : PackageName(packageName), MemberName(memberName) {}
};

#endif // MEMBER_IMPORT_STATEMENT_H