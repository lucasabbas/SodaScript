#ifndef SODA_SCRIPT_TOKEN_H
#define SODA_SCRIPT_TOKEN_H

enum SodaScriptToken {
  // Keywords
  PackageKeyword,     // Example : "package"
  ImportKeyword,      // Example : "import"
  LocalKeyword,       // Example : "local"
  VarKeyword,         // Example : "var"
  FuncKeyword,        // Example : "func"
  ReturnKeyword,      // Example : "return"
  IfKeyword,          // Example : "if"
  ElseKeyword,        // Example : "else"
  ForKeyword,         // Example : "for"
  ForeachKeyword,     // Example : "foreach"
  InKeyword,          // Example : "in"
  ContinueKeyword,    // Example : "continue"
  BreakKeyword,       // Example : "break"
  IsKeyword,          // Example : "is"
  ClassKeyword,       // Example : "class"
  ExtendsKeyword,     // Example : "extends"
  ConstructorKeyword, // Example : "constructor"
  StaticKeyword,      // Example : "static"
  NewKeyword,         // Example : "new"

  // Data Types
  StringType,     // Example : "String"
  IntType,        // Example : "Int"
  FloatType,      // Example : "Float"
  LongType,       // Example : "Long"
  DoubleType,     // Example : "Double"
  NumberType,     // Example : "Number"
  AnyType,        // Example : "Any"
  ArrayType,      // Example : "Array"
  DictionaryType, // Example : "Dictionary"
  BoolType,       // Example : "Bool"
  VoidType,       // Example : "Void"

  // Operators
  Plus,               // Example : "+"
  Minus,              // Example : "-"
  Multiply,           // Example : "*"
  Divide,             // Example : "/"
  Modulo,             // Example : "%"
  Assign,             // Example : "="
  AssignRef,          // Example : ":="
  Equals,             // Example : "=="
  NotEquals,          // Example : "!="
  LessThan,           // Example : "<"
  GreaterThan,        // Example : ">"
  LessThanOrEqual,    // Example : "<="
  GreaterThanOrEqual, // Example : ">="
  Arrow,              // Example : "=>"
  Not,                // Example : "!"
  Dot,                // Example : "."
  Comma,              // Example : ","
  Colon,              // Example : ":"
  Semicolon,          // Example : ";"
  Tilde,              // Example : "~"
  LParen,             // Example : "("
  RParen,             // Example : ")"
  LBracket,           // Example : "["
  RBracket,           // Example : "]"
  LBrace,             // Example : "{"
  RBrace,             // Example : "}"

  // Literals
  Identifier,     // Example : "identifier"
  StringLiteral,  // Example : "string literal"
  IntegerLiteral, // Example : "integer literal"
  FloatLiteral,   // Example : "float literal"
  LongLiteral,    // Example : "long literal"
  DoubleLiteral,  // Example : "double literal"
  BooleanLiteral, // Example : "boolean literal"

  // Miscellaneous
  Whitespace, // Example : "whitespace"
  Comment,    // Example : "comment"
  GenericType // Example : "T"
};

#endif