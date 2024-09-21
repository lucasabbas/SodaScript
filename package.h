#ifndef PACKAGE_H
#define PACKAGE_H

#include "ast_node.h"
#include "member_import_statement.h"
#include "package_import_statement.h"
#include <memory>
#include <string>
#include <vector>


class Package : public AstNode {
public:
  std::string Name;
  std::vector<std::shared_ptr<AstNode>> Members;
  std::vector<std::shared_ptr<PackageImportStatement>> PackageImports;
  std::vector<std::shared_ptr<MemberImportStatement>> MemberImports;

  Package(
      const std::string &name,
      const std::vector<std::shared_ptr<AstNode>> &members,
      const std::vector<std::shared_ptr<PackageImportStatement>>
          &packageImports,
      const std::vector<std::shared_ptr<MemberImportStatement>> &memberImports)
      : Name(name), Members(members), PackageImports(packageImports),
        MemberImports(memberImports) {}

  ~Package() = default; // No need for manual memory management
};

#endif // PACKAGE_H