#ifndef PARSER_H
#define PARSER_H

#include <lexicalAnalyzer.h>

typedef struct
{
  LexicalAnalyzer *lexicalAnalyzer;
  Token token;
} Parser;

Parser *createParser(LexicalAnalyzer *lexicalAnalyzer);

void Program(Parser *parser);
// void T(Parser *parser);
// void El(Parser *parser);
// void OP(Parser *parser);

#endif