#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <lexicalAnalyzer.h>
#include "../includes/Error.h"
#include "../includes/Parser.h"
#include "../includes/Ast.h"

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
void ParserProgram(Parser *parser)
{
    printf("Program\n");

    Program program;
    program = createProgram(createLocation("/home/gabriel/projetos/Parser/utils/example.code", 1, 1));

    program.statements = createStatement_PrintStatement(
        createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
        NULL,
        NULL,
        createPrintStatement(
            createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
            createExpression_Term(
                createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
                createTerm_string(
                    createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
                    createString(
                        createLocation("/home/gabriel/projetos/Parser/utils/example.code", 3, 3),
                        "Hello World")))));

    AstConsumer(program);

    // parser->token = nextToken(parser->lexicalAnalyzer);

    // if (
    //     strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0 ||
    //     strcmp(parser->token.value, keywords[PROGRAM]) == 0)
    // {
    //     printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
    //     while (1)
    //     {
    //         ParserStatement(parser);
    //         if ((parser->token.value && strcmp(parser->token.value, keywords[END]) == 0) || strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END") == 0)
    //         {
    //             printf("End of program\n");
    //             break;
    //         }
    //     }
    // }
}

/**
 * @details Implements <statement>
 */
void ParserStatement(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);

    if (strcmp(parser->token.value, keywords[VAR]) == 0)
    {
        ParserVariableDeclaration(parser);
    }
    else if (strcmp(parser->token.value, keywords[PRINT]) == 0)
    {
        ParserPrintStatement(parser);
    }
    else if ((parser->token.value && strcmp(parser->token.value, keywords[END]) == 0) || strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END") == 0)
    {
        return;
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
    {
        ParserAssignment(parser);
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
void ParserPrintStatement(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_LEFT_PARENTHESIS") == 0)
    {
        ParserExpression(parser);
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

void ParserString(Parser *parser)
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

void ParserParenthesis(Parser *parser)
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

void ParserVariableDeclaration(Parser *parser)
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

void ParserAssignment(Parser *parser)
{
    controlNextToken(parser);
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);

    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_OPERATOR") == 0 && strcmp(parser->token.value, "=") == 0)
    {
        ParserExpression(parser);
    }
    else
    {
        throwError(1, "Error: Expected =\n");
        exit(1);
    }
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
}

void ParserTerm(Parser *parser)
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

void ParserExpressionTail(Parser *parser)
{
    controlNextToken(parser);
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_OPERATOR") == 0)
    {
        printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
        ParserTerm(parser);
    }
}

void ParserExpression(Parser *parser)
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
        ParserTerm(parser);
        ParserExpressionTail(parser);
    }
}
