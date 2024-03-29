#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexicalAnalyzer.h>
#include "../includes/Parser.h"
#include "../includes/Ast.h"

#define MAX_LINE_SIZE 1024

int main(int argc, char *argv[])
{
  printf("Parser\n");
  printf("File attach: %s\n", argv[1]);

  if (argc < 2)
  {
    throwError(1, "File not found\n");
    exit(1);
  }

  LexicalAnalyzer *lexicalAnalyzer = createLexicalAnalyzer(argv[1]);
  Parser *parser = createParser(lexicalAnalyzer);
  ParserProgram(parser);
  AstConsumer(*parser->ast->program);
  closeLexicalAnalyzer(lexicalAnalyzer);
  return 0;
}
