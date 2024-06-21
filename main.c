/*
Example:
Input:
    WIN+RAR=DEXT
Output:
        0 = 'E'
        1 = 'D'
        2 = 'W'
        3 = 'I'
        4 = 'T'
        5 = 'A'
        6 = 'N'
        7 = ''
        8 = 'R'
        9 = 'X'

Examples:
    SEND+MORE=MONEY
    K+Q+KE+OL+XB=UI
    ABCDEF+FEDCBA=GGGGGG
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define RESULT_SZ 10

typedef struct _Equation
{
    char *equation;
    int equation_sz;
    char result[RESULT_SZ];
    char unique_letters[11];
    int unique_letters_count;
    int word_n;
} Equation;


int CheckArguments(const int argc, const char **argv);
int CheckEquation(Equation *p_equation);
int SetupEquation(Equation *p_equation, const char *p_expression);
void ClearEquation(Equation *p_equation);
int FindSolution(Equation *p_equation, char letterIdx);
void PrintSolution(Equation *p_equation);
int CheckEquationResult(Equation *p_equation);
int CheckEquationLetters(Equation *p_equation);
int ReplaceLettersWithNumbers(Equation *p_equation, char **p_dst);
void ReplaceChar(char *p_str, char oldChar, char newChar);
void CountWords(Equation *p_equation);


int main(const int argc, const char **argv)
{
    if (CheckArguments(argc, argv) != 0)
    {
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        Equation equation;

        if (SetupEquation(&equation, argv[i]) != 0)
        {
            printf("Failed to setup equation %s\n", argv[i]);
            return -1;
        }

        if (CheckEquation(&equation) != 0)
        {
            printf("Invalid equation %s\n", argv[i]);
            return -1;
        }

        printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("Equation: %s\n", equation.equation);
        if (FindSolution(&equation, 0) != 0)
        {
            printf("No solution found for equation %s\n", argv[i]);
        }
        else
        {
            PrintSolution(&equation);
        }

        ClearEquation(&equation);
    }
}


int CheckArguments(const int argc, const char **argv)
{
    if (argc < 2)
    {
        printf("Usage %s <equation 1> [equation 2] ... [equation n]\n", argv[0]);
        return -1;
    }

    return 0;
}


int CheckEquation(Equation *p_equation)
{
    if (p_equation->word_n < 2 ||
        p_equation->unique_letters_count > 10)
    {
        return -1;
    }
    
    return 0;
}


int SetupEquation(Equation *p_equation, const char *p_expression)
{
    memset(p_equation, 0, sizeof(Equation));

    p_equation->equation_sz = (strlen(p_expression) + 1) * sizeof(char);
    p_equation->equation = (char *)calloc(strlen(p_expression) + 1, sizeof(char));
    if (p_equation->equation == NULL)
    {
        return -1;
    }

    strcpy(p_equation->equation, p_expression);

    p_equation->unique_letters_count = 0;
    for (int i = 0; i < p_equation->equation_sz; i++)
    {
        if (isalpha(p_equation->equation[i]))
        {
            if (strchr(p_equation->unique_letters, p_equation->equation[i]) == NULL)
            {
                p_equation->unique_letters[p_equation->unique_letters_count++] = p_equation->equation[i];
            }
        }
    }

    CountWords(p_equation);

    return 0;
}


void ClearEquation(Equation *p_equation)
{
    free(p_equation->equation);
    memset(p_equation, 0, sizeof(Equation));
}


int FindSolution(Equation *p_equation, char letterIdx)
{
    for (int i = 0; i < RESULT_SZ; i++)
    {
        char letter = p_equation->result[i];
        p_equation->result[i] = p_equation->unique_letters[letterIdx];
        if (letterIdx + 1 < p_equation->unique_letters_count)
        {
            if (FindSolution(p_equation, letterIdx + 1) == 0)
            {
                return 0;
            }
        } else {
            if (CheckEquationResult(p_equation) == 0)
            {
                return 0;
            }
        }
        p_equation->result[i] = letter;
    }

    return -1;
}


void PrintSolution(Equation *p_equation)
{
    for (int i = 0; i < RESULT_SZ; i++)
    {
        printf("%d = '%c'\n", i, p_equation->result[i]);
    }
}


int CheckEquationResult(Equation *p_equation)
{
    if (CheckEquationLetters(p_equation) != 0)
    {
        return -1;
    }
    
    char *p_buf;
    if (ReplaceLettersWithNumbers(p_equation, &p_buf) != 0)
    {
        return -1;
    }

    int *numbers = (int *)calloc(p_equation->word_n + 1, sizeof(int));
    if (numbers == NULL)
    {
        free(p_buf);
        return -1;
    }

    char *current_position = p_buf;
    for (int i = 0; i < p_equation->word_n + 1; i++)
    {
        int characters_read;

        // Read number
        sscanf(current_position, "%d%n", &numbers[i], &characters_read);
        current_position += characters_read;

        // Skip character (e.g. `+` or `=`)
        current_position++;
    }

    int result = 0;
    for (int i = 0; i < p_equation->word_n; i++)
    {
        result += numbers[i];
    }

    if (result != numbers[p_equation->word_n])
    {
        free(p_buf);
        free(numbers);
        return -1;
    }

    free(p_buf);
    free(numbers);

    return 0;
}


int CheckEquationLetters(Equation *p_equation)
{
    for (int i = 0; i < p_equation->unique_letters_count; i++)
    {
        if (!memchr(p_equation->result, p_equation->unique_letters[i], RESULT_SZ))
        {
            return -1;
        }
    }

    return 0;
}


int ReplaceLettersWithNumbers(Equation *p_equation, char **p_dst)
{
    char *p_buf;

    p_buf = (char *)calloc(strlen(p_equation->equation) + 1, sizeof(char));
    if (p_buf == NULL)
    {
        return -1;
    }

    strcpy(p_buf, p_equation->equation);

    for (int i = 0; i < RESULT_SZ; i++)
    {
        ReplaceChar(p_buf, p_equation->result[i], i + '0');
    }

    *p_dst = p_buf;

    return 0;
}


void ReplaceChar(char *str, char oldChar, char newChar) {
    if (oldChar == 0)
    {
        return;
    }

    char *current_pos = strchr(str, oldChar);
    while (current_pos) {
        *current_pos = newChar;
        current_pos = strchr(current_pos + 1, oldChar);
    }
}


void CountWords(Equation *p_equation)
{
    int count = 1;
    char *p = p_equation->equation;
    while (*p != '=')
    {
        if (!isalpha(*p))
        {
            count++;
        }

        p++;
    }

    p_equation->word_n = count;
}
