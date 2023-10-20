#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexicalAnalyzer.h>

// Implements Parse Machine
struct {
    char *token;
    char *lexeme;
} token;