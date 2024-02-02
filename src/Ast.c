#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Ast.h"

char *fileOutput = "output.js";
char *fileOutputAst = "ast-output.json";

/**
 * @Program
 */
Program createProgram(Location *location)
{
  Program program;

  program.statements = NULL;
  program.location = location;
  return program;
}

/**
 * @Location
 */
Location *createLocation(char *fileName, size_t start, size_t end)
{
  Location *Location = malloc(sizeof(Location));

  Location->fileName = strdup(fileName);
  Location->start = start;
  Location->end = end;
  return Location;
}

/**
 * @Statement
 */
Statement *createStatement_Assignment(Location *location, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));

  s->location = location;
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
Statement *createStatement_VariableDeclaration(Location *Location, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));

  s->location = Location;
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
Statement *createStatement_PrintStatement(Location *location, Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));

  s->location = location;
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
VariableDeclaration *createVariableDeclaration(Location *location, Type type, Identifier *identifier)
{
  VariableDeclaration *vd = malloc(sizeof(VariableDeclaration));

  vd->location = location;

  vd->type = type;
  vd->identifier = identifier;
  return vd;
}

/**
 * @Assignment
 */
Assignment *createAssignment(Location *location, Identifier *identifier, Expression *expression)
{
  Assignment *a = malloc(sizeof(Assignment));

  a->location = location;

  a->identifier = identifier;
  a->expression = expression;
  return a;
}

/**
 * @PrintStatement
 */
PrintStatement *createPrintStatement(Location *location, Expression *expression)
{
  PrintStatement *ps = malloc(sizeof(PrintStatement));

  ps->location = location;
  ps->expression = expression;
  return ps;
}

/**
 * @Expression
 */
Expression *createExpression_Term_ExpressionTail(Location *location, Term *term, ExpressionTail *tail)
{
  Expression *expr = malloc(sizeof(Expression));

  expr->location = location;

  expr->term = term;
  expr->expression_tail = tail;
  return expr;
}

/**
 * @Expression
 */
Expression *createExpression_Term(Location *location, Term *term)
{
  Expression *expr = malloc(sizeof(Expression));

  expr->location = location;

  expr->term = term;
  expr->expression_tail = NULL;
  return expr;
}

/**
 * @ExpressionTail
 */
ExpressionTail *createExpressionTail(Location *location, char op, Term *term, ExpressionTail *next)
{
  ExpressionTail *tail = malloc(sizeof(ExpressionTail));

  tail->location = location;

  tail->op = op;
  tail->term = term;
  tail->next = next;
  return tail;
}

/**
 * @Term
 */
Term *createTerm_number(Location *location, Number *number)
{
  Term *term = malloc(sizeof(Term));

  term->location = location;

  term->number = number;
  term->identifier = NULL;
  term->string = NULL;
  return term;
}

/**
 * @Term
 */
Term *createTerm_identifier(Location *location, Identifier *identifier)
{
  Term *term = malloc(sizeof(Term));

  term->location = location;

  term->number = NULL;
  term->identifier = identifier;
  term->string = NULL;
  return term;
}

/**
 * @Term
 */
Term *createTerm_string(Location *location, String *string)
{
  Term *term = malloc(sizeof(Term));

  term->location = location;

  term->number = NULL;
  term->identifier = NULL;
  term->string = string;
  return term;
}

/**
 * @String
 */
String *createString(Location *location, char *value)
{
  String *str = malloc(sizeof(String));

  str->location = location;

  str->value = strdup(value);
  return str;
}

/**
 * @Number
 */
Number *createNumber(Location *location, int value)
{
  Number *number = malloc(sizeof(Number));

  number->location = location;

  number->value = value;
  return number;
}

/**
 * @Identifier
 */
Identifier *createIdentifier(Location *location, char *name)
{
  Identifier *identifier = malloc(sizeof(Identifier));

  identifier->location = location;

  identifier->name = strdup(name);
  return identifier;
}

/**
 * @Type
 */
Type getLiteralType(char *searchType)
{
  if (strcmp(searchType, "int") == 0)
  {
    return TYPE_INT;
  }
  else if (strcmp(searchType, "string") == 0)
  {
    return TYPE_STRING;
  }
  else if (strcmp(searchType, "bool") == 0)
  {
    return TYPE_BOOL;
  }
  else if (strcmp(searchType, "float") == 0)
  {
    return TYPE_FLOAT;
  }
  else
  {
    printf("Type unknow: %s\n", searchType);
    exit(1);
  }
}

cJSON *checkLocation(Location *Location)
{
  if (Location == NULL)
  {
    printf("Location without Location\n");
    exit(1);
  }

  if (Location->fileName == NULL)
  {
    printf("Location without fileName\n");
    exit(1);
  }

  if (Location->start == 0)
  {
    printf("Location without start\n");
    exit(1);
  }

  if (Location->end == 0)
  {
    printf("Location without end\n");
    exit(1);
  }

  cJSON *jsonLocation = cJSON_CreateObject();
  cJSON_AddItemToObject(jsonLocation, "FileName", cJSON_CreateString(Location->fileName));
  cJSON_AddItemToObject(jsonLocation, "Start", cJSON_CreateNumber(Location->start));
  cJSON_AddItemToObject(jsonLocation, "End", cJSON_CreateNumber(Location->end));

  return jsonLocation;
}

cJSON *AstConsumerTerm(Term *term)
{
  if (term == NULL)
  {
    printf("Term without term\n");
    exit(1);
  }

  cJSON *jsonTerm = cJSON_CreateObject();

  checkLocation(term->location);

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

  cJSON_AddItemToObject(jsonTerm, "Location", checkLocation(term->location));

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
  cJSON_AddItemToObject(jsonExpr, "Location", checkLocation(expr->location));

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
  cJSON_AddItemToObject(jsonPs, "Location", checkLocation(ps->location));

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

  cJSON *jsonLocation = checkLocation(program.location);
  cJSON_AddItemToObject(jsonProgram, "Location", jsonLocation);
  createOutputFile(jsonProgram);
  cJSON_Delete(jsonProgram);
}

// int main()
// {
//   Program program;
//   program = createProgram(createLocation("/home/gabriel/projetos/Parser/utils/example.code", 1, 1));

//   program.statements = createStatement_PrintStatement(
//       createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
//       NULL,
//       NULL,
//       createPrintStatement(
//           createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
//           createExpression_Term(
//               createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
//               createTerm_string(
//                   createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
//                   createString(
//                       createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
//                       "Hello World")))));

//   AstConsumer(program);
// }
  // program.statements = createStatement_PrintStatement(
  //     createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //     NULL,
  //     NULL,
  //     createPrintStatement(
  //         createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //         createExpression_Term(
  //             createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //             createTerm_string(
  //                 createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //                 createString(
  //                     createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
  //                     "Hello World")))));

  // program.statements->next = createStatement_VariableDeclaration(
  //     createLocation("/home/gabriel/projetos/Parser/utils/example.code", 5, 5),
  //     NULL,
  //     createVariableDeclaration(
  //         createLocation("/home/gabriel/projetos/Parser/utils/example.code", 5, 5),
  //         getType("int"),
  //         createIdentifier(
  //             createLocation("/home/gabriel/projetos/Parser/utils/example.code", 5, 5),
  //             "num1")),
  //     NULL);

  // program.statements->next->next = createStatement_VariableDeclaration(
  //     createLocation("/home/gabriel/projetos/Parser/utils/example.code", 6, 6),
  //     NULL,
  //     createVariableDeclaration(
  //         createLocation("/home/gabriel/projetos/Parser/utils/example.code", 6, 6),
  //         getType("int"),
  //         createIdentifier(
  //             createLocation("/home/gabriel/projetos/Parser/utils/example.code", 6, 6),
  //             "num2")),
  //     NULL);

  // program.statements->next->next->next = createStatement_VariableDeclaration(
  //     createLocation("/home/gabriel/projetos/Parser/utils/example.code", 7, 7),
  //     NULL,
  //     createVariableDeclaration(
  //         createLocation("/home/gabriel/projetos/Parser/utils/example.code", 7, 7),
  //         getType("int"),
  //         createIdentifier(
  //             createLocation("/home/gabriel/projetos/Parser/utils/example.code", 7, 7),
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

//   return 0;
// }