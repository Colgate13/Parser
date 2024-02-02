#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "./Parser.h"
#include "cjson/cJSON.h"

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

Program createProgram(Location *location);
Location *createLocation(char *fileName, size_t start, size_t end);
Statement *createStatement_Assignment(Location *location, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps);
Statement *createStatement_VariableDeclaration(Location *Location, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps);
Statement *createStatement_PrintStatement(Location *location, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps);
VariableDeclaration *createVariableDeclaration(Location *location, Type type, Identifier *identifier);
Assignment *createAssignment(Location *location, Identifier *identifier, Expression *expression);
PrintStatement *createPrintStatement(Location *location, Expression *expression);
Expression *createExpression_Term_ExpressionTail(Location *location, Term *term, ExpressionTail *tail);
Expression *createExpression_Term(Location *location, Term *term);
ExpressionTail *createExpressionTail(Location *location, char op, Term *term, ExpressionTail *next);
Term *createTerm_number(Location *location, Number *number);
Term *createTerm_identifier(Location *location, Identifier *identifier);
Term *createTerm_string(Location *location, String *string);
String *createString(Location *location, char *value);
Number *createNumber(Location *location, int value);
Identifier *createIdentifier(Location *location, char *name);
Type getLiteralType(char *searchType);
cJSON *checkLocation(Location *Location);
cJSON *AstConsumerTerm(Term *term);
cJSON *AstConsumerExpression(Expression *expr);
cJSON *AstConsumerPrintStatement(PrintStatement *ps);
void createOutputFile(cJSON *json);
void AstConsumer(Program program);
