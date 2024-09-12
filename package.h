#ifndef PACKAGE_H
#define PACKAGE_H

#include "ast_node.h"
#include "member_import_statement.h"
#include "package_import_statement.h"
#include <string>
#include <vector>

class Package : public AstNode {
public:
  std::string Name;
  std::vector<AstNode *> Members;
  std::vector<PackageImportStatement *> PackageImports;
  std::vector<MemberImportStatement *> MemberImports;

  Package(std::string &name, const std::vector<AstNode *> &members,
          const std::vector<PackageImportStatement *> &packageImports,
          const std::vector<MemberImportStatement *> &memberImports)
      : Name(name), Members(members), PackageImports(packageImports),
        MemberImports(memberImports) {
    for (auto &member : Members) {
      if (member)
        member->addRef();
    }
    for (auto &packageImport : PackageImports) {
      if (packageImport)
        packageImport->addRef();
    }
    for (auto &memberImport : MemberImports) {
      if (memberImport)
        memberImport->addRef();
    }
  }

  ~Package() {
    for (auto &member : Members) {
      if (member)
        member->release();
    }
    for (auto &packageImport : PackageImports) {
      if (packageImport)
        packageImport->release();
    }
    for (auto &memberImport : MemberImports) {
      if (memberImport)
        memberImport->release();
    }
  }
};

#endif // PACKAGE_H