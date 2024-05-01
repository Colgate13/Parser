<h1 align="center">Parser</h1>

<p align="center">
   <img src="https://github.com/Colgate13/Parser/assets/54006467/f2fe3f29-23c1-467b-af2d-597a378a31b2" alt="lexial" width="400"/>
</p>

<p align="center">	
  <img alt="Repository size" src="https://img.shields.io/github/repo-size/Colgate13/Parser?color=774DD6">

  <a href="https://github.com/Colgate13/Parser/commits">
    <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Colgate13/Parser?color=774DD6">
  </a> 
  <img alt="License" src="https://img.shields.io/badge/license-MIT-8257E5">
  <a href="https://github.com/Colgate13/Parser/stargazers">
    <img alt="Stargazers" src="https://img.shields.io/github/stars/Colgate13/Parser?color=8257E5&logo=github">
  </a>
</p>

<p align="center">
    <a href="README.md">English</a>
 </p>

  <p align="center">
  <a href="#about">About</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#technologies">Technologies</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#features">Features</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#docs">Docs</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#license">License</a>
</p>

## :notebook: About

<div id="about"></div>

This project is an my implementation of a basic a Syntactic Parser developed in C using the Clang compiler, designed to construct Abstract Syntax Trees (AST) from tokens provided by a lexical analyzer(https://github.com/Colgate13/LexicalAnalyzer). This project is ideal for students, educators, and developers interested in understanding and applying fundamental concepts of syntactic analysis in compilers.

Using this [grammar](https://github.com/Colgate13/Parser/blob/main/utils/gram.txt):

```
<program>            --> "program" <statement> "end;"

<statement>          --> <print_statement>
                    | <variable_declaration>
                    | <assignment>

<variable_declaration> --> "var" <type> <identifier> ";"

<assignment>         --> <identifier> "=" <expression> ";"

<print_statement>    --> "print(" <expression> ");"

<expression>         --> <term> <expression_tail>
                    | <term>

<expression_tail>    --> "+" <term> <expression_tail>
                    | "-" <term> <expression_tail>
                    | "*" <term> <expression_tail>
                    | "/" <term> <expression_tail>
                    | "%" <term> <expression_tail>
                    | ε

<term>               --> <number>
                    | <identifier>
                    | <string>

<number>             --> [0-9]+

<string>             --> '"' [a-zA-Z0-9_]* '"'

<identifier>         --> [a-zA-Z_][a-zA-Z0-9_]*

<type>               --> "int"
                    | "float"
                    | "string"

```

## Technologies 🐱‍🏍🎂

<div id="technologies"></div>

- [C](https://www.gnu.org/software/gnu-c-manual/) - C
- [Clang](https://clang.llvm.org/) - C/C++ Compiler
- [Make | Makefile](https://www.gnu.org/software/make/) - Make and Makefile
- [Lexical Analyzer](https://github.com/Colgate13/LexicalAnalyzer) - Lexical Analyzer

### Features

<div id="features"></div>

- [x] Call Lexical Analyzer
- [x] Construct Abstract Syntax Trees (AST)
- [x] Error handling
- [x] Output AST in JSON format
- [x] Output AST in file

## Docs

<div id="docs"></div>

### Required

```sh
clang
LexicalAnalyzer - using README instructions to compile lib and shared
```

### Build and Run

```sh
git clone https://github.com/Colgate13/Parser.git
cd Parser
make

./bin/main ./utils/exemples/general/exemple.code
```

## License

<div id="license"></div>

MIT [LICENSE](LICENSE.md)
