Gramatica G:

G = (Vn, Vt, P, S)

Vn (Nao terminais) = {E, T, OP}
Vt (Terminais) = {id, number, +, -, *, /}

E -> E OP T | T
T -> id  | number
OP -> + | - | * | /

Temos recursividade a esquerda, entao precisamos fatorar:

Regras Sintaticas:

1a. E -> T E'
1b. E' -> OP T E' | &
2.  T -> id | number
3. OP -> + | - | * | /