#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

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
  Identifier *identifier;
  String *string;
  AstFileLocation *fileLocation;
} Term;

// <expression_tail> --> "+" <term> <expression_tail> | "-" <term> <expression_tail> | ε
typedef struct ExpressionTail
{
  char operator;
  Term *term;
  struct ExpressionTail *next;
  AstFileLocation *fileLocation;
} ExpressionTail;

// <expression> --> <term> <expression_tail> | <number> | <string> | <identifier>
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
  VariableDeclaration *variable_declaration;
  PrintStatement *print_statement;
  struct Statement *next;
  AstFileLocation *fileLocation;
} Statement;

// <program> -> "program" <statement> "end;"
typedef struct Program
{
  Statement *statements;
  AstFileLocation *fileLocation;
} Program;

Statement *create_statement(Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));
  s->assignment = assignment;
  s->variable_declaration = vd;
  s->print_statement = ps;
  s->next = NULL;
  return s;
}

PrintStatement *create_print_statement(Expression *expression)
{
  PrintStatement *ps = malloc(sizeof(PrintStatement));
  ps->expression = expression;
  return ps;
}

Term *create_term_string(String *string)
{
  Term *term = malloc(sizeof(Term));
  term->number = NULL;
  term->identifier = NULL;
  term->string = string;
  return term;
}

String *create_string(char *value)
{
  String *str = malloc(sizeof(String));
  str->value = strdup(value);
  return str;
}

Expression *create_expression(Term *term, ExpressionTail *tail)
{
  Expression *expr = malloc(sizeof(Expression));
  expr->term = term;
  expr->expression_tail = tail;
  return expr;
}

// Função para imprimir detalhes da expressão
void print_expression(Expression *expr, int depth)
{
  if (expr->term->number != NULL)
  {
    printf("\"number\": \"%d\"", expr->term->number->value);
  }
  else if (expr->term->identifier != NULL)
  {
    printf("\"identifier\": \"%s\"", expr->term->identifier->name);
  }
  else if (expr->term->string != NULL)
  {
    printf("\"string\": \"%s\"", expr->term->string->value);
  }
  // Adicione aqui o tratamento para ExpressionTail se necessário
}

void print_json(Statement *statement, int depth)
{
  while (statement != NULL)
  {
    for (int i = 0; i < depth; i++)
      printf("\t");

    if (statement->print_statement != NULL)
    {
      printf("{\n");
      for (int i = 0; i <= depth; i++)
        printf("\t");
      printf("\"print_statement\": {\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"command\": \"print\",\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"expression\": {\n");
      for (int i = 0; i <= depth + 2; i++)
        printf("\t");
      print_expression(statement->print_statement->expression, depth + 2);
      printf("\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("}\n");
      for (int i = 0; i <= depth; i++)
        printf("\t");
      printf("}");

      printf("\n}");
    }
    else if (statement->variable_declaration != NULL)
    {
      printf("{\n");
      for (int i = 0; i <= depth; i++)
        printf("\t");
      printf("\"variable_declaration\": {\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"keyword\": \"var\",\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"type\": \"%s\",\n", statement->variable_declaration->type == TYPE_INT ? "int" : "other");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"identifier\": \"%s\"\n", statement->variable_declaration->identifier->name);
      for (int i = 0; i <= depth; i++)
        printf("\t");
      printf("}");

      printf("\n}");
    }
    else if (statement->assignment != NULL)
    {
      printf("{\n");
      for (int i = 0; i <= depth; i++)
        printf("\t");
      printf("\"assignment\": {\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"identifier\": \"%s\",\n", statement->assignment->identifier->name);
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("\"expression\": {\n");
      for (int i = 0; i <= depth + 2; i++)
        printf("\t");
      print_expression(statement->assignment->expression, depth + 2);
      printf("\n");
      for (int i = 0; i <= depth + 1; i++)
        printf("\t");
      printf("}\n");
      for (int i = 0; i <= depth; i++)
        printf("\t");
      printf("}");

      printf("\n}");
    }

    if (statement->next != NULL)
    {
      printf(",\n");
    }
    else
    {
      printf("\n");
    }

    statement = statement->next;
  }
}

int main()
{
  printf("Hello, World!\n");

  Program program;

  program.statements = create_statement(
      NULL,
      NULL,
      create_print_statement(
          create_expression(
              create_term_string(
                  create_string("Hello World")),
              NULL)));

  /*
  
  program

    print("Hello World");
  
  */

  // Imprimindo a árvore
  printf("{\n");
  printf("\t\"program\": {\n");
  printf("\t\t\"statements\": [\n");
  print_json(program.statements, 3);
  printf("\t\t]\n");
  printf("\t}\n");
  printf("}\n");


  return 0;
}