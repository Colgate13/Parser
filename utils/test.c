#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING,
  TYPE_BOOL
} Type;

typedef struct Identifier
{
  char *name;
} Identifier;

typedef struct Number
{
  int value;
} Number;

typedef struct String
{
  char *value;
} String;

typedef struct Term
{
  Number *number;
  Identifier *identifier;
  String *string;
} Term;

typedef struct ExpressionTail
{
  char operator; // '+' ou '-'
  Term *term;
  struct ExpressionTail *next;
} ExpressionTail;

typedef struct Expression
{
  Term *term;
  ExpressionTail *expression_tail;
} Expression;

typedef struct Assignment
{
  Identifier *identifier;
  Expression *expression;
} Assignment;

typedef struct VariableDeclaration
{
  Type type;
  Identifier *identifier;
} VariableDeclaration;

typedef struct PrintStatement
{
  Expression *expression;
} PrintStatement;

typedef struct Statement
{
  Assignment *assignment;
  VariableDeclaration *variable_declaration;
  PrintStatement *print_statement;
  struct Statement *next;
} Statement;

typedef struct Program
{
  Statement *statements;
} Program;

// Funções auxiliares para criar nós
Identifier *create_identifier(char *name)
{
  Identifier *id = malloc(sizeof(Identifier));
  id->name = strdup(name);
  return id;
}

Number *create_number(int value)
{
  Number *num = malloc(sizeof(Number));
  num->value = value;
  return num;
}

String *create_string(char *value)
{
  String *str = malloc(sizeof(String));
  str->value = strdup(value);
  return str;
}

Term *create_term_number(Number *number)
{
  Term *term = malloc(sizeof(Term));
  term->number = number;
  term->identifier = NULL;
  term->string = NULL;
  return term;
}

Term *create_term_identifier(Identifier *identifier)
{
  Term *term = malloc(sizeof(Term));
  term->number = NULL;
  term->identifier = identifier;
  term->string = NULL;
  return term;
}

Term *create_term_string(String *string)
{
  Term *term = malloc(sizeof(Term));
  term->number = NULL;
  term->identifier = NULL;
  term->string = string;
  return term;
}

Expression *create_expression(Term *term, ExpressionTail *tail)
{
  Expression *expr = malloc(sizeof(Expression));
  expr->term = term;
  expr->expression_tail = tail;
  return expr;
}

PrintStatement *create_print_statement(Expression *expression)
{
  PrintStatement *ps = malloc(sizeof(PrintStatement));
  ps->expression = expression;
  return ps;
}

VariableDeclaration *create_variable_declaration(Type type, Identifier *identifier)
{
  VariableDeclaration *vd = malloc(sizeof(VariableDeclaration));
  vd->type = type;
  vd->identifier = identifier;
  return vd;
}

Assignment *create_assignment(Identifier *identifier, Expression *expression)
{
  Assignment *a = malloc(sizeof(Assignment));
  a->identifier = identifier;
  a->expression = expression;
  return a;
}

Statement *create_statement(Assignment *assignment, VariableDeclaration *vd, PrintStatement *ps)
{
  Statement *s = malloc(sizeof(Statement));
  s->assignment = assignment;
  s->variable_declaration = vd;
  s->print_statement = ps;
  s->next = NULL;
  return s;
}

// Função para imprimir a árvore (simplificada)
void print_tree(Program *program)
{
  Statement *current = program->statements;
  while (current != NULL)
  {
    if (current->print_statement != NULL)
    {
      printf("Print Statement\n");
    }
    else if (current->variable_declaration != NULL)
    {
      printf("Variable Declaration: %s\n", current->variable_declaration->identifier->name);
    }
    else if (current->assignment != NULL)
    {
      printf("Assignment to: %s\n", current->assignment->identifier->name);
    }
    current = current->next;
  }
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
  // Construindo a árvore do programa
  Program program;
  program.statements = create_statement(NULL, NULL, create_print_statement(create_expression(create_term_string(create_string("Hello World")), NULL)));
  program.statements->next = create_statement(NULL, create_variable_declaration(TYPE_INT, create_identifier("num1")), NULL);
  program.statements->next->next = create_statement(NULL, create_variable_declaration(TYPE_INT, create_identifier("num2")), NULL);
  program.statements->next->next->next = create_statement(NULL, create_variable_declaration(TYPE_INT, create_identifier("result")), NULL);
  program.statements->next->next->next->next = create_statement(create_assignment(create_identifier("num1"), create_expression(create_term_number(create_number(1)), NULL)), NULL, NULL);
  program.statements->next->next->next->next->next = create_statement(create_assignment(create_identifier("num2"), create_expression(create_term_number(create_number(2)), NULL)), NULL, NULL);

  // Adicionando a atribuição de 'result'
  ExpressionTail *tail = malloc(sizeof(ExpressionTail));
  tail->operator= '+';
  tail->term = create_term_identifier(create_identifier("num2"));
  tail->next = NULL;

  Expression *result_expr = create_expression(create_term_identifier(create_identifier("num1")), tail);
  program.statements->next->next->next->next->next->next = create_statement(create_assignment(create_identifier("result"), result_expr), NULL, NULL);

  // Adicionando o último print
  program.statements->next->next->next->next->next->next->next = create_statement(NULL, NULL, create_print_statement(create_expression(create_term_identifier(create_identifier("result")), NULL)));

  // Imprimindo a árvore
  printf("{\n");
  printf("\t\"program\": {\n");
  printf("\t\t\"statements\": [\n");
  print_json(program.statements, 3);
  printf("\t\t]\n");
  printf("\t}\n");
  printf("}\n");

  // ... Liberação de memória (não implementada aqui) ...

  return 0;
}
