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
    parser->ast = (Ast *)malloc(sizeof(Ast));

    if (parser->ast == NULL)
    {
        return NULL;
    }

    return parser;
}

void controlNextToken(Parser *parser)
{
    parser->token = nextToken(parser->lexicalAnalyzer);

    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END_LINE") == 0)
        parser->token = nextToken(parser->lexicalAnalyzer);
}

Location *cl(Parser *parser)
{
    return createLocation("*file*", parser->lexicalAnalyzer->lineCount, parser->lexicalAnalyzer->positionCount);
}

void logToken(Parser *parser)
{
    printf("Token: %s, value: %s\n", tokenTypeName(parser->token.type), parser->token.value);
}

char *removeQuotes(char *str)
{
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '\"')
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
    return str;
}

/**
 * @details Implements <program>
 *
 * Rules -> Terminals initialize with lowercase
 *       -> Non-terminals initialize with uppercase
 */
void ParserProgram(Parser *parser)
{
    Program program;
    program = createProgram(createLocation("*file*", 1, 1));
    parser->ast->program = &program;
    parser->token = nextToken(parser->lexicalAnalyzer);

    if (
        strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0 ||
        strcmp(parser->token.value, keywords[PROGRAM]) == 0)
    {
        logToken(parser);
        parser->ast->program->statements = ParserStatement(parser);
        controlNextToken(parser);
        if ((parser->token.value && strcmp(parser->token.value, keywords[END]) == 0) || strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_END") == 0)
        {
            return;
        }
    }
}

/**
 * @details Implements <statement>
 */
Statement *ParserStatement(Parser *parser)
{
    controlNextToken(parser);
    logToken(parser);

    if (strcmp(parser->token.value, keywords[PRINT]) == 0)
    {
        return createStatement_PrintStatement(
            cl(parser),
            ParserPrintStatement(parser));
    }
    else
    {
        throwError(1, "Expected identifier, print or end\n");
        exit(1);
    }
}

/**
 * @details Implements <print_statement>
 */
PrintStatement *ParserPrintStatement(Parser *parser)
{
    controlNextToken(parser);
    logToken(parser);

    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_LEFT_PARENTHESIS") == 0)
    {

        PrintStatement *printStatement = createPrintStatement(
            cl(parser), ParserExpression(parser));

        if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_RIGHT_PARENTHESIS") == 0)
        {
            logToken(parser);
            controlNextToken(parser);
            return printStatement;
        }
        else
        {
            throwError(1, "Expected )\n");
            exit(1);
        }
    }
    else
    {
        throwError(1, "Expected identifier\n");
        exit(1);
    }
}

Expression *ParserExpression(Parser *parser)
{
    controlNextToken(parser);
    logToken(parser);

    if (
        strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_NUMBER") == 0 || strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
    {
        Expression *expr = createExpression_Term(
            cl(parser), ParserTerm(parser));
        controlNextToken(parser);
        return expr;
    }
    else
    {
        throwError(1, "Expected TOKEN_TYPE_NUMBER\n");
        exit(1);
    }
}

Term *ParserTerm(Parser *parser)
{
    if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_NUMBER") == 0)
    {
        return createTerm_number(
            cl(parser), createNumber(cl(parser), atoi(parser->token.value)));
        controlNextToken(parser);
    }
    else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
    {
        return createTerm_string(
            cl(parser), createString(cl(parser), removeQuotes(parser->token.value)));
        controlNextToken(parser);
    }
    else
    {
        throwError(1, "Expected TOKEN_TYPE_NUMBER, TOKEN_TYPE_IDENTIFIER\n");
        exit(1);
    }
}

// void ParserString(Parser *parser)
// {
//     controlNextToken(parser);
//     if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
//     {
//         logToken(parser);
//         controlNextToken(parser);
//     }
//     else
//     {
//        throwError(1, "Expected string\n");
//         exit(1);
//     }
// }

// void ParserParenthesis(Parser *parser)
// {
//     controlNextToken(parser);
//     if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_LEFT_PARENTHESIS") == 0)
//     {
//         logToken(parser);
//         controlNextToken(parser);

//         if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_NUMBER") == 0)
//         {
//             logToken(parser);
//             controlNextToken(parser);
//         }
//         else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
//         {
//             logToken(parser);
//             controlNextToken(parser);
//         }
//         else if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_STRING") == 0)
//         {
//             logToken(parser);
//             controlNextToken(parser);
//         }
//         else
//         {
//            throwError(1, "Expected TOKEN_TYPE_NUMBER, TOKEN_TYPE_IDENTIFIER\n");
//             exit(1);
//         }

//         if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_RIGHT_PARENTHESIS") == 0)
//         {
//             logToken(parser);
//             controlNextToken(parser);
//         }
//         else
//         {
//            throwError(1, "Expected )\n");
//             exit(1);
//         }
//     }
//     else
//     {
//        throwError(1, "Expected (\n");
//         exit(1);
//     }
// }

// void ParserVariableDeclaration(Parser *parser)
// {
//     controlNextToken(parser);
//     if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
//     {
//         logToken(parser);

//         if (strcmp(parser->token.value, keywords[INT]) == 0)
//         {
//             printf("INT\n");
//         }
//         else if (strcmp(parser->token.value, keywords[FLOAT]) == 0)
//         {
//             printf("FLOAT\n");
//         }
//         else if (strcmp(parser->token.value, keywords[STRING]) == 0)
//         {
//             printf("STRING\n");
//         }
//         else if (strcmp(parser->token.value, keywords[BOOL]) == 0)
//         {
//             printf("BOOL\n");
//         }
//         else
//         {
//            throwError(1, "Expected int, float, string or bool\n");
//             exit(1);
//         }

//         controlNextToken(parser);

//         if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_IDENTIFIER") == 0)
//         {
//             logToken(parser);
//             controlNextToken(parser);
//         }
//         else
//         {
//            throwError(1, "Expected identifier\n");
//             exit(1);
//         }
//     }
//     else
//     {
//        throwError(1, "Expected identifier\n");
//         exit(1);
//     }

//     logToken(parser);
// }

// void ParserAssignment(Parser *parser)
// {
//     controlNextToken(parser);
//     logToken(parser);

//     if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_OPERATOR") == 0 && strcmp(parser->token.value, "=") == 0)
//     {
//         ParserExpression(parser);
//     }
//     else
//     {
//        throwError(1, "Expected =\n");
//         exit(1);
//     }
//     logToken(parser);
// }

// void ParserExpressionTail(Parser *parser)
// {
//     controlNextToken(parser);
//     if (strcmp(tokenTypeName(parser->token.type), "TOKEN_TYPE_OPERATOR") == 0)
//     {
//         logToken(parser);
//         ParserTerm(parser);
//     }
// }
