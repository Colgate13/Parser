#ifndef PARSER_H
#define PARSER_H

#include <lexicalAnalyzer.h>

typedef struct
{
  LexicalAnalyzer *lexicalAnalyzer;
  Token token;
} Parser;

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

extern enum EKeywords
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

Parser *createParser(LexicalAnalyzer *lexicalAnalyzer);
void controlNextToken(Parser *parser);
void ParserProgram(Parser *parser);
void ParserStatement(Parser *parser);
void ParserPrintStatement(Parser *parser);
void ParserString(Parser *parser);
void ParserParenthesis(Parser *parser);
void ParserVariableDeclaration(Parser *parser);
void ParserAssignment(Parser *parser);
void ParserExpression(Parser *parser);
void ParserTerm(Parser *parser);
void ParserExpressionTail(Parser *parser);

#endif