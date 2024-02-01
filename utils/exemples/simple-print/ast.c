int main()
{
  Program program;
  program = createProgram(createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 1, 1));

  program.statements = createStatement_PrintStatement(
      createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
      NULL,
      NULL,
      createPrintStatement(
          createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
          createExpression_Term(
              createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
              createTerm_string(
                  createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
                  createString(
                      createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
                      "Hello World")))));

  AstConsumer(program);

  return 0;
}