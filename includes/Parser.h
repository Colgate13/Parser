#ifndef PARSER_H
#define PARSER_H

#include <lexicalAnalyzer.h>

typedef enum
{
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING,
  TYPE_BOOL
} Type;

enum EStatementsType
{
  PRINT_STATEMENT = 0,
  ASSIGNMENT_STATEMENT,
  VARIABLE_DECLARATION_STATEMENT,
};

enum EKeywords
{
  PROGRAM = 0,
  END,
  VAR,
  PRINT,
  INT,
  FLOAT,
  STRING,
  BOOL,
};

// Location = { start: 0, end: 0, fileName: "test.txt" }
typedef struct
{
  size_t start;
  size_t end;
  char *fileName;
} Location;

// <identifier> --> [a-zA-Z_][a-zA-Z0-9_]*
typedef struct Identifier
{
  char *name;
  Location *location;
} Identifier;

// <number> --> [0-9]+
typedef struct Number
{
  int value;
  Location *location;
} Number;

// <string> --> '"' [a-zA-Z0-9_]* '"'
typedef struct String
{
  char *value;
  Location *location;
} String;

// Term = Number | Identifier | String
typedef struct Term
{
  Number *number;
  // |
  Identifier *identifier;
  // |
  String *string;
  Location *location;
} Term;

// <expression_tail> --> "+" <term> <expression_tail> | "-" <term> <expression_tail> | ε
typedef struct ExpressionTail
{
  char op;
  Term *term;
  struct ExpressionTail *next;
  Location *location;
} ExpressionTail;

// <expression> --> <term> <expression_tail> | <term>
typedef struct Expression
{
  Term *term;
  ExpressionTail *expression_tail;
  Location *location;
} Expression;

// <assignment> --> <identifier> "=" <expression> ";"
typedef struct Assignment
{
  Identifier *identifier;
  Expression *expression;
  Location *location;
} Assignment;

// <variable_declaration> --> "var" <type> <identifier> ";"
typedef struct VariableDeclaration
{
  Type type;
  Identifier *identifier;
  Location *location;
} VariableDeclaration;

// <print_statement> --> "print(" <expression> ");"
typedef struct PrintStatement
{
  Expression *expression;
  Location *location;
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
  Location *location;
  unsigned short int type;
} Statement;

// <program> -> "program" <statement> "end;"
typedef struct Program
{
  Statement *statements;
  Location *location;
} Program;

typedef struct Ast
{
  Program *program;
} Ast;

typedef struct
{
  LexicalAnalyzer *lexicalAnalyzer;
  Ast *ast;
  Token token;
} Parser;

void controlNextToken(Parser *parser);
void ParserProgram(Parser *parser);
void logToken(Parser *parser);
char *removeQuotes(char *str);

Parser *createParser(LexicalAnalyzer *lexicalAnalyzer);
Statement *ParserStatement(Parser *parser);
PrintStatement *ParserPrintStatement(Parser *parser);
Expression *ParserExpression(Parser *parser);
Term *ParserTerm(Parser *parser);

Location *cl(Parser *parser);
// void ParserString(Parser *parser);
// void ParserParenthesis(Parser *parser);
// void ParserVariableDeclaration(Parser *parser);
// void ParserAssignment(Parser *parser);
// void ParserExpressionTail(Parser *parser);

#endif