#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

enum EKeywords
{
  PRINT_STATEMENT = 0,
  ASSIGNMENT_STATEMENT,
  VARIABLE_DECLARATION_STATEMENT,
  EXPRESSION_TERM,
  TERM,
  STRING
};

typedef enum
{
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING,
  TYPE_BOOL
} Type;

// AstFileLocation = { start: 0, end: 0, fileName: "test.txt" }
typedef struct
{
  size_t start;
  size_t end;
  char *fileName;
} AstFileLocation;

// <identifier> --> [a-zA-Z_][a-zA-Z0-9_]*
typedef struct Identifier
{
  char *name;
  AstFileLocation *fileLocation;
} Identifier;

// <number> --> [0-9]+
typedef struct Number
{
  int value;
  AstFileLocation *fileLocation;
} Number;

// <string> --> '"' [a-zA-Z0-9_]* '"'
typedef struct String
{
  char *value;
  AstFileLocation *fileLocation;
} String;

// Term = Number | Identifier | String
typedef struct Term
{
  Number *number;
  // |
  Identifier *identifier;
  // |
  String *string;
  AstFileLocation *fileLocation;
} Term;

// <expression_tail> --> "+" <term> <expression_tail> | "-" <term> <expression_tail> | ε
typedef struct ExpressionTail
{
  char op;
  Term *term;
  struct ExpressionTail *next;
  AstFileLocation *fileLocation;
} ExpressionTail;

// <expression> --> <term> <expression_tail> | <term>
typedef struct Expression
{
  Term *term;
  ExpressionTail *expression_tail;
  AstFileLocation *fileLocation;
} Expression;

// <assignment> --> <identifier> "=" <expression> ";"
typedef struct Assignment
{
  Identifier *identifier;
  Expression *expression;
  AstFileLocation *fileLocation;
} Assignment;

// <variable_declaration> --> "var" <type> <identifier> ";"
typedef struct VariableDeclaration
{
  Type type;
  Identifier *identifier;
  AstFileLocation *fileLocation;
} VariableDeclaration;

// <print_statement> --> "print(" <expression> ");"
typedef struct PrintStatement
{
  Expression *expression;
  AstFileLocation *fileLocation;
} PrintStatement;

// <statement> -> <assignment> | <variable_declaration> | <print_statement>
typedef struct Statement
{
  Assignment *assignment;
  // |
  VariableDeclaration *variable_declaration;
  // |
  PrintStatement *print_statement;
  struct Statement *next;
  AstFileLocation *fileLocation;
  unsigned short int type;
} Statement;

// <program> -> "program" <statement> "end;"
typedef struct Program
{
  Statement *statements;
  AstFileLocation *fileLocation;
} Program;