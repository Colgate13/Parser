#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexicalAnalyzer.h>

#define MAX_LINE_SIZE 1024

int main(int argc, char *argv[])
{
  printf("Lexical Analyzer\n");
  printf("File attach: %s\n", argv[1]);

  if (argc < 2)
  {
    throwError(1, "Error: File not found\n");
    exit(1);
  }

  LexicalAnalyzer *lexicalAnalyzer = createLexicalAnalyzer(argv[1]);

  nextToken(lexicalAnalyzer);
  nextToken(lexicalAnalyzer);
  nextToken(lexicalAnalyzer);
  nextToken(lexicalAnalyzer);
  nextToken(lexicalAnalyzer);
  Token token = nextToken(lexicalAnalyzer);

  printf("Token: %s, value: %s\n", tokenTypeName(token.type), token.value);

  closeLexicalAnalyzer(lexicalAnalyzer);
  return 0;
}
