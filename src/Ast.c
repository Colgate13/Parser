#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Ast.h"
#include "cjson/cJSON.h"
// #define NULL ((void *)0)

char *fileOutput = "output.js";
char *fileOutputAst = "ast-output.json";

/**
 * @Program
 */
Program createProgram(AstFileLocation *fileLocation)
{
  Program program;

  program.statements = NULL;
  program.fileLocation = fileLocation;
  return program;
}

/**
 * @AstFileLocation
 */
AstFileLocation *createAstFileLocation(char *fileName, size_t start, size_t end)
{
  AstFileLocation *fileLocation = malloc(sizeof(AstFileLocation));

  fileLocation->fileName = strdup(fileName);
  fileLocation->start = start;
  fileLocation->end = end;
  return fileLocation;
}

/**
 * @Statement
 */
Statement *createStatement_Assignment(AstFileLocation *fileLocation, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));

  s->fileLocation = fileLocation;
  s->type = ASSIGNMENT_STATEMENT;

  s->assignment = assignment;
  s->variable_declaration = NULL;
  s->print_statement = NULL;
  s->next = NULL;
  return s;
}

/**
 * @Statement
 */
Statement *createStatement_VariableDeclaration(AstFileLocation *fileLocation, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));

  s->fileLocation = fileLocation;
  s->type = VARIABLE_DECLARATION_STATEMENT;

  s->assignment = NULL;
  s->variable_declaration = vd;
  s->print_statement = NULL;
  s->next = NULL;
  return s;
}

/**
 * @Statement
 */
Statement *createStatement_PrintStatement(AstFileLocation *fileLocation, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));

  s->fileLocation = fileLocation;
  s->type = PRINT_STATEMENT;

  s->assignment = NULL;
  s->variable_declaration = NULL;
  s->print_statement = ps;
  s->next = NULL;
  return s;
}

/**
 * @VariableDeclaration
 */
VariableDeclaration *createVariableDeclaration(AstFileLocation *fileLocation, Type type, Identifier *identifier)
{
  VariableDeclaration *vd = malloc(sizeof(VariableDeclaration));

  vd->fileLocation = fileLocation;

  vd->type = type;
  vd->identifier = identifier;
  return vd;
}

/**
 * @Assignment
 */
Assignment *createAssignment(AstFileLocation *fileLocation, Identifier *identifier, Expression *expression)
{
  Assignment *a = malloc(sizeof(Assignment));

  a->fileLocation = fileLocation;

  a->identifier = identifier;
  a->expression = expression;
  return a;
}

/**
 * @PrintStatement
 */
PrintStatement *createPrintStatement(AstFileLocation *fileLocation, Expression *expression)
{
  PrintStatement *ps = malloc(sizeof(PrintStatement));

  ps->fileLocation = fileLocation;
  ps->expression = expression;
  return ps;
}

/**
 * @Expression
 */
Expression *createExpression_Term_ExpressionTail(AstFileLocation *fileLocation, Term *term, ExpressionTail *tail)
{
  Expression *expr = malloc(sizeof(Expression));

  expr->fileLocation = fileLocation;

  expr->term = term;
  expr->expression_tail = tail;
  return expr;
}

/**
 * @Expression
 */
Expression *createExpression_Term(AstFileLocation *fileLocation, Term *term)
{
  Expression *expr = malloc(sizeof(Expression));

  expr->fileLocation = fileLocation;

  expr->term = term;
  expr->expression_tail = NULL;
  return expr;
}

/**
 * @ExpressionTail
 */
ExpressionTail *createExpressionTail(AstFileLocation *fileLocation, char op, Term *term, ExpressionTail *next)
{
  ExpressionTail *tail = malloc(sizeof(ExpressionTail));

  tail->fileLocation = fileLocation;

  tail->op = op;
  tail->term = term;
  tail->next = next;
  return tail;
}

/**
 * @Term
 */
Term *createTerm_number(AstFileLocation *fileLocation, Number *number)
{
  Term *term = malloc(sizeof(Term));

  term->fileLocation = fileLocation;

  term->number = number;
  term->identifier = NULL;
  term->string = NULL;
  return term;
}

/**
 * @Term
 */
Term *createTerm_identifier(AstFileLocation *fileLocation, Identifier *identifier)
{
  Term *term = malloc(sizeof(Term));

  term->fileLocation = fileLocation;

  term->number = NULL;
  term->identifier = identifier;
  term->string = NULL;
  return term;
}

/**
 * @Term
 */
Term *createTerm_string(AstFileLocation *fileLocation, String *string)
{
  Term *term = malloc(sizeof(Term));

  term->fileLocation = fileLocation;

  term->number = NULL;
  term->identifier = NULL;
  term->string = string;
  return term;
}

/**
 * @String
 */
String *createString(AstFileLocation *fileLocation, char *value)
{
  String *str = malloc(sizeof(String));

  str->fileLocation = fileLocation;

  str->value = strdup(value);
  return str;
}

/**
 * @Number
 */
Number *createNumber(AstFileLocation *fileLocation, int value)
{
  Number *number = malloc(sizeof(Number));

  number->fileLocation = fileLocation;

  number->value = value;
  return number;
}

/**
 * @Identifier
 */
Identifier *createIdentifier(AstFileLocation *fileLocation, char *name)
{
  Identifier *identifier = malloc(sizeof(Identifier));

  identifier->fileLocation = fileLocation;

  identifier->name = strdup(name);
  return identifier;
}

/**
 * @Type
 */
Type getType(char *type)
{
  if (strcmp(type, "int") == 0)
  {
    return TYPE_INT;
  }
  else if (strcmp(type, "string") == 0)
  {
    return TYPE_STRING;
  }
  else if (strcmp(type, "bool") == 0)
  {
    return TYPE_BOOL;
  }
  else if (strcmp(type, "float") == 0)
  {
    return TYPE_FLOAT;
  }
  else
  {
    printf("Type unknow: %s\n", type);
    exit(1);
  }
}

cJSON *checkFileLocation(AstFileLocation *fileLocation)
{
  if (fileLocation == NULL)
  {
    printf("FileLocation without fileLocation\n");
    exit(1);
  }

  if (fileLocation->fileName == NULL)
  {
    printf("FileLocation without fileName\n");
    exit(1);
  }

  if (fileLocation->start == 0)
  {
    printf("FileLocation without start\n");
    exit(1);
  }

  if (fileLocation->end == 0)
  {
    printf("FileLocation without end\n");
    exit(1);
  }

  cJSON *jsonFileLocation = cJSON_CreateObject();
  cJSON_AddItemToObject(jsonFileLocation, "FileName", cJSON_CreateString(fileLocation->fileName));
  cJSON_AddItemToObject(jsonFileLocation, "Start", cJSON_CreateNumber(fileLocation->start));
  cJSON_AddItemToObject(jsonFileLocation, "End", cJSON_CreateNumber(fileLocation->end));

  return jsonFileLocation;
}

cJSON *AstConsumerTerm(Term *term)
{
  if (term == NULL)
  {
    printf("Term without term\n");
    exit(1);
  }

  cJSON *jsonTerm = cJSON_CreateObject();

  checkFileLocation(term->fileLocation);

  if (term->number == NULL && term->identifier == NULL && term->string == NULL)
  {
    printf("Term without number, identifier or string\n");
    exit(1);
  }

  if (term->number != NULL)
  {
    // printf("Term\n");
    // printf("Number\n");
    // printf("Value: %d\n", term->number->value);
    cJSON_AddNumberToObject(jsonTerm, "Number", term->number->value);
  }

  if (term->identifier != NULL)
  {
    // printf("Term\n");
    // printf("Identifier\n");
    // printf("Value: %s\n", term->identifier->name);
    cJSON_AddStringToObject(jsonTerm, "Identifier", term->identifier->name);
  }

  if (term->string != NULL)
  {
    // printf("Term\n");
    // printf("String\n");
    // printf("Value: %s\n", term->string->value);
    cJSON_AddStringToObject(jsonTerm, "String", term->string->value);
  }

  cJSON_AddItemToObject(jsonTerm, "Location", checkFileLocation(term->fileLocation));

  return jsonTerm;
}

cJSON *AstConsumerExpression(Expression *expr)
{
  if (expr == NULL)
  {
    printf("Expression without expression\n");
    exit(1);
  }

  cJSON *jsonExpr = cJSON_CreateObject();
  cJSON_AddItemToObject(jsonExpr, "Term", AstConsumerTerm(expr->term));
  cJSON_AddItemToObject(jsonExpr, "Location", checkFileLocation(expr->fileLocation));

  // TODO: expression_tail

  return jsonExpr;
}

cJSON *AstConsumerPrintStatement(PrintStatement *ps)
{
  if (ps == NULL)
  {
    printf("PrintStatement without print_statement\n");
    exit(1);
  }

  cJSON *jsonPs = cJSON_CreateObject();
  cJSON_AddItemToObject(jsonPs, "Expression", AstConsumerExpression(ps->expression));
  cJSON_AddItemToObject(jsonPs, "Location", checkFileLocation(ps->fileLocation));

  return jsonPs;
}

void createOutputFile(cJSON *json)
{
  FILE *file = fopen(fileOutputAst, "w");
  if (file == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  fprintf(file, "%s", cJSON_Print(json));
  fclose(file);
}

void AstConsumer(Program program)
{

  if (program.statements == NULL)
  {
    printf("Program without statements\n");
    exit(1);
  }

  cJSON *jsonProgram = cJSON_CreateObject();
  cJSON *jsonStatements = cJSON_CreateArray();
  
  cJSON_AddItemToObject(jsonProgram, "Statements", jsonStatements);

  Statement *currentStatement = program.statements;

  cJSON *jsonStatement = cJSON_CreateObject();
  while (currentStatement != NULL)
  {

    switch (currentStatement->type)
    {
    case PRINT_STATEMENT:
      if (currentStatement->print_statement == NULL)
      {
        printf("PrintStatement without print_statement\n");
        exit(1);
      }

      cJSON *jsonPrintStatement = AstConsumerPrintStatement(currentStatement->print_statement);
      cJSON_AddItemToObject(jsonStatement, "PrintStatement", jsonPrintStatement);

      currentStatement = currentStatement->next;
      break;

    default:
      printf("Statement type unknow\n");
      break;
    }

    cJSON_AddItemToArray(jsonStatements, jsonStatement);
  }

  cJSON *jsonFileLocation = checkFileLocation(program.fileLocation);
  cJSON_AddItemToObject(jsonProgram, "Location", jsonFileLocation);
  createOutputFile(jsonProgram);
  cJSON_Delete(jsonProgram);
}

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

  // program.statements = createStatement_PrintStatement(
  //     createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //     NULL,
  //     NULL,
  //     createPrintStatement(
  //         createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //         createExpression_Term(
  //             createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //             createTerm_string(
  //                 createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //                 createString(
  //                     createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //                     "Hello World")))));

  // program.statements->next = createStatement_VariableDeclaration(
  //     createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 5, 5),
  //     NULL,
  //     createVariableDeclaration(
  //         createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 5, 5),
  //         getType("int"),
  //         createIdentifier(
  //             createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 5, 5),
  //             "num1")),
  //     NULL);

  // program.statements->next->next = createStatement_VariableDeclaration(
  //     createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 6, 6),
  //     NULL,
  //     createVariableDeclaration(
  //         createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 6, 6),
  //         getType("int"),
  //         createIdentifier(
  //             createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 6, 6),
  //             "num2")),
  //     NULL);

  // program.statements->next->next->next = createStatement_VariableDeclaration(
  //     createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 7, 7),
  //     NULL,
  //     createVariableDeclaration(
  //         createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 7, 7),
  //         getType("int"),
  //         createIdentifier(
  //             createAstFileLocation("/home/gabriel/projetos/Parser/utils/example.code", 7, 7),
  //             "result")),
  //     NULL);

  // printf("Program\n");
  // printf("Statement\n");
  // printf("Assignment\n");

  /*
  program

    print("Hello World");

    var int num1;
    var int num2;
    var int result;

    num1 = 1;
    num2 = 2;

    result = num1 + num2;

    print(result);

  end;
  */

  // AstConsumer(program);
  // if (program.statements) {
  //   printf("Statement\n");
  //   if (program.statements->print_statement) {
  //     printf("PrintStatement\n");
  //     if (program.statements->print_statement->expression) {
  //       printf("Expression\n");
  //       if (program.statements->print_statement->expression->term) {
  //         printf("Term\n");
  //         if (program.statements->print_statement->expression->term->string) {
  //           printf("String\n");
  //           printf("Value: %s\n", program.statements->print_statement->expression->term->string->value);
  //         }
  //       }
  //     }
  //   }
  // }

  return 0;
}