#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <lexicalAnalyzer.h>
#include "../includes/Error.h"
#include "../includes/Parser.h"

enum EKeywords
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

const char *keywords[] = {
    "program",
    "end",
    "var",
    "print",
    "int",
    "float",
    "string",
    "bool",
};

Parser *createParser(LexicalAnalyzer *lexicalAnalyzer);
void controlNextToken(Parser *parser);
void Program(Parser *parser);
void Statement(Parser *parser);
void PrintStatement(Parser *parser);
void String(Parser *parser);
void Parenthesis(Parser *parser);
void VariableDeclaration(Parser *parser);
void Assignment(Parser *parser);
void Expression(Parser *parser);
void Term(Parser *parser);
void ExpressionTail(Parser *parser);

Parser *createParser(LexicalAnalyzer *lexicalAnalyzer)
{
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    if (parser == NULL)
    {
        return NULL;
    }
    parser->lexicalAnalyzer = lexicalAnalyzer;
    return parser;
}

void controlNextToken(Parser *parser)
{
    parser->token = nextToken(parser->lexicalAnalyzer);

    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END_LINE") == 0)
        parser->token = nextToken(parser->lexicalAnalyzer);
}

/**
 * @details Implements <program>
 *
 * Rules -> Terminals initialize with lowercase
 *       -> Non-terminals initialize with uppercase
 */
void Program(Parser *parser)
{
    printf("Program\n");
    parser->token = nextToken(parser->lexicalAnalyzer);

    if (
        strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0 ||
        strcmp(parser->token.value, keywords[PROGRAM]) == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        while (1)
        {
            Statement(parser);
            if ((parser->token.value && strcmp(parser->token.value, keywords[END]) == 0) || strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END") == 0)
            {
                printf("End of program\n");
                break;
            }
        }
    }
}

/**
 * @details Implements <statement>
 */
void Statement(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);

    if (strcmp(parser->token.value, keywords[VAR]) == 0)
    {
        VariableDeclaration(parser);
    }
    else if (strcmp(parser->token.value, keywords[PRINT]) == 0)
    {
        PrintStatement(parser);
    }
    else if ((parser->token.value && strcmp(parser->token.value, keywords[END]) == 0) || strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END") == 0)
    {
        return;
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
    {
        Assignment(parser);
    }
    else
    {
        throwError(1, "Error: Expected identifier, print or end\n");
        exit(1);
    }
}

/**
 * @details Implements <print_statement>
 */
void PrintStatement(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_LEFT_PARENTHESIS") == 0)
    {
        Expression(parser);
        if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_RIGHT_PARENTHESIS") == 0)
        {
            printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
            controlNextToken(parser);
        }
        else
        {
            throwError(1, "Error: Expected )\n");
            exit(1);
        }
    }
    else
    {
        throwError(1, "Error: Expected identifier\n");
        exit(1);
    }

    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
}

void String(Parser *parser)
{
    controlNextToken(parser);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        controlNextToken(parser);
    }
    else
    {
        throwError(1, "Error: Expected string\n");
        exit(1);
    }
}

void Parenthesis(Parser *parser)
{
    controlNextToken(parser);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_LEFT_PARENTHESIS") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        controlNextToken(parser);

        if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_NUMBER") == 0)
        {
            printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
            controlNextToken(parser);
        }
        else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
        {
            printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
            controlNextToken(parser);
        }
        else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
        {
            printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
            controlNextToken(parser);
        }
        else
        {
            throwError(1, "Error: Expected TOKEN_TYPE_NUMBER, TOKEN_TYPE_IDENTIFIER\n");
            exit(1);
        }

        if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_RIGHT_PARENTHESIS") == 0)
        {
            printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
            controlNextToken(parser);
        }
        else
        {
            throwError(1, "Error: Expected )\n");
            exit(1);
        }
    }
    else
    {
        throwError(1, "Error: Expected (\n");
        exit(1);
    }
}

void VariableDeclaration(Parser *parser)
{
    controlNextToken(parser);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);

        if (strcmp(parser->token.value, keywords[INT]) == 0)
        {
            printf("INT\n");
        }
        else if (strcmp(parser->token.value, keywords[FLOAT]) == 0)
        {
            printf("FLOAT\n");
        }
        else if (strcmp(parser->token.value, keywords[STRING]) == 0)
        {
            printf("STRING\n");
        }
        else if (strcmp(parser->token.value, keywords[BOOL]) == 0)
        {
            printf("BOOL\n");
        }
        else
        {
            throwError(1, "Error: Expected int, float, string or bool\n");
            exit(1);
        }

        controlNextToken(parser);

        if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
        {
            printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
            controlNextToken(parser);
        }
        else
        {
            throwError(1, "Error: Expected identifier\n");
            exit(1);
        }
    }
    else
    {
        throwError(1, "Error: Expected identifier\n");
        exit(1);
    }

    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
}

void Assignment(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);

    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_OPERATOR") == 0 && strcmp(parser->token.value, "=") == 0)
    {
        Expression(parser);
    }
    else
    {
        throwError(1, "Error: Expected =\n");
        exit(1);
    }
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
}

void Term(Parser *parser)
{
    controlNextToken(parser);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_NUMBER") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        controlNextToken(parser);
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        controlNextToken(parser);
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        controlNextToken(parser);
    }
    else
    {
        throwError(1, "Error: Expected TOKEN_TYPE_NUMBER, TOKEN_TYPE_IDENTIFIER\n");
        exit(1);
    }
}

void ExpressionTail(Parser *parser)
{
    controlNextToken(parser);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_OPERATOR") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        Term(parser);
    }
}

void Expression(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);

    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_NUMBER") == 0)
    {
        controlNextToken(parser);
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
    {
        controlNextToken(parser);
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
    {
        controlNextToken(parser);
    }
    else
    {
        Term(parser);
        ExpressionTail(parser);
    }
}
