# abc-equation-solver
A program that takes alphabetical equations like `SEND+MORE=MONEY` and finds their numerical solutions.

# Usage
Compile app with
```
gcc main.c
```
Run
```
./a.out <equation 1> [equation 2] ... [equation n]
./a.out SEND+MORE=MONEY
./a.out SEND+MORE=MONEY A+B+C=D
```
Example of output:
```
Equation: SEND+MORE=MONEY
0 = 'M'
1 = 'N'
2 = 'S'
3 = 'O'
4 = ''
5 = 'Y'
6 = 'R'
7 = 'D'
8 = 'E'
9 = ''
```