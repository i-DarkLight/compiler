#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdio.h>
using namespace std;
map<string, string> lexemes;
// Function to check if a lexeme is a keyword
// If the lexeme is a keyword we'll treat it as a keyword in switch cases
bool isKeyWord(string word)
{
    string keywords[21] = {"cond", "newcond", "condmirror","div", "multi", "minus", "major", "minor", "plus", "inc", "dec", "func", "cycle", "start", "eq", "iseq", "noteq", "sendback", "end", "pace", "call"};
    for (int i = 0; i < 21; i++)
        if (word == keywords[i])
            return true;
    return false;
}
// Function to return a lexeme with its ID Number if the lexeme was already in list
string returnID(string lexeme)
{
    map<string, string>::iterator pointer = lexemes.begin();
    while (pointer != lexemes.end())
    {
        if (pointer->second == lexeme)
            return pointer->first;
        pointer++;
    }
    return 0;
}
// Function to see if a lexeme was already declared so we can reuse the same ID for the lexeme
bool isDeclared(string lexeme)
{
    map<string, string>::iterator pointer = lexemes.begin();
    while (pointer != lexemes.end())
    {
        if (pointer->second == lexeme)
            return true;
        pointer++;
    }
    return false;
}
// Function to treat special characters as an endpoint of words
bool isInArray(char c)
{
    char ch = c;
    char operands[14] = {
        '=',
        '*',
        '(',
        ')',
        '{',
        '}',
        '[',
        ']',
        ';',
        ':',
        '"',
        ',',
        ' ',
        '\n'};
    for (int i = 0; i < 14; i++)
        if (ch == operands[i])
            return true;
    return false;
}
// Here we will go through input code and generate every token
void tokenGenerator(string code, int length)
{
    ofstream compiledFile(".\\data\\compiled.txt");
    ofstream errorFile(".\\data\\errors.txt");
    int eq = 1, iseq = 1, var = 1, lencode = length, id = 0;
    int cond = 1, noteq = 1, sendback = 1, func = 1, minor = 1, major = 1, plus = 1, minus = 1, inc = 1, dec = 1, newcond = 1, cycle = 1, start = 1, end = 1, pace = 1, div = 1, multi = 1, call = 1;
    int lineCounter = 1, column = -1;
    string number, lexeme, fixed;
    for (int i = 0; i < lencode; i++)
    {
        // count lines and cols to return errors if we face any with percise address
        if (code[i - 1] == 32 && i > 1)
            compiledFile << ' ';
        if (code[i - 1] == '\n')
        {
            compiledFile << '\n';
            lineCounter++;
            column = 0;
        }
        else
            column++;
        // statement to pass string values as fixed values
        if (code[i] == '"')
        {
            while (code[i + 1] != '"')
            {
                fixed += code[i];
                i++;
                column++;
            }
            fixed += code[i];
            fixed += code[i + 1];
            i += 1;
            column += 2;
            compiledFile << "[string," << fixed << "]";
            fixed = "";
            continue;
        }
        // if we are not looking at string values we can generate other tokens
        else
        {
            // statement for approved characters, if not print error in else statement
            if (isInArray(code[i]) || (code[i] >= 'a' && code[i] <= 'z') || (code[i] >= 'A' && code[i] <= 'Z') || (code[i] >= '0' && code[i] <= '9') || code[i] == '_' || code[i] == EOF || code[i] == '.')
            {
                // statement to print error if there is '.' in any other place than float numbers
                if (code[i] == '.' && (!(code[i - 1] >= '0' && code[i - 1] <= '9') || !(code[i + 1] >= '0' && code[i + 1] <= '9')))
                {
                    errorFile << "ERROR! unexpected '.' in line " << lineCounter << " col " << column << ">" << endl;
                    //  cout << "ERROR! unexpected '.' in line " << lineCounter << " col " << column << endl;
                }
                else
                {
                    // pass numbers as fixed values and check if float numbers are valued correctly, if not print error
                    if (code[i] >= '0' && code[i] <= '9' && isInArray(code[i - 1]))
                    {
                        number += code[i];
                        continue;
                    }
                    else if ((code[i] >= '0' && code[i] <= '9') || code[i] == '.')
                    {
                        number += code[i];
                        continue;
                    }
                    else if ((isInArray(code[i]) || code[i] == EOF) && ((code[i - 1] >= '0' && code[i - 1] <= '9') || code[i] == '.'))
                    {
                        int index = 0, counter = 0;
                        for (index = 0; index < number.length(); index++)
                            if (number[index] == '.')
                                counter++;
                        if (counter > 1)
                        {
                            errorFile << "line " << lineCounter << " ERROR! for float numbers use only one '.'" << endl;
                            //       cout << "line " << lineCounter << " ERROR! for float numbers use only one '.'" << endl;
                        }
                        else
                            compiledFile << "[" << number << "]";
                        number = "";
                        continue;
                    }
                }
                // All DFAs that are programmed into switch cases
                switch (noteq)
                {
                case 1:
                    if (code[i] == 'n' && isInArray(code[i - 1]) == true && code[i + 1] == 'o')
                    {
                        noteq = 2;
                        break;
                    }
                    else
                    {
                        noteq = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'o' && code[i - 1] == 'n')
                    {
                        noteq = 3;
                        break;
                    }
                    else
                    {
                        noteq = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 't' && code[i - 1] == 'o')
                    {
                        noteq = 4;
                        break;
                    }
                    else
                    {
                        noteq = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'e' && code[i - 1] == 't')
                    {
                        noteq = 5;
                        break;
                    }
                    else
                    {
                        noteq = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'q' && code[i - 1] == 'e')
                    {
                        noteq = 6;
                        break;
                    }
                    else
                    {
                        noteq = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'q')
                    {
                        compiledFile << "[notequal]";
                        noteq = 1;
                        break;
                    }
                    else
                    {
                        noteq = 1;
                        break;
                    }
                }
                switch (div)
                {
                case 1:
                    if (code[i] == 'd' && isInArray(code[i - 1]) == true && code[i + 1] == 'i')
                    {
                        div = 2;
                        break;
                    }
                    else
                    {
                        div = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'i' && code[i - 1] == 'd')
                    {
                        div = 3;
                        break;
                    }
                    else
                    {
                        div = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'v' && code[i - 1] == 'i')
                    {
                        div = 4;
                        break;
                    }
                    else
                    {
                        div = 1;
                        break;
                    }
                case 4:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'v')
                    {
                        compiledFile << "[div]";
                        div = 1;
                        break;
                    }
                    else
                    {
                        div = 1;
                        break;
                    }
                }
                switch (call)
                {
                case 1:
                    if (code[i] == 'c' && isInArray(code[i - 1]) == true && code[i + 1] == 'a')
                    {
                        call = 2;
                        break;
                    }
                    else
                    {
                        call = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'a' && code[i - 1] == 'c')
                    {
                        call = 3;
                        break;
                    }
                    else
                    {
                        call = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'l' && code[i - 1] == 'a')
                    {
                        call = 4;
                        break;
                    }
                    else
                    {
                        call = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'l' && code[i - 1] == 'l')
                    {
                        call = 5;
                        break;
                    }
                    else
                    {
                        call = 1;
                        break;
                    }
                case 5:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'l')
                    {
                        compiledFile << "[call]";
                        call = 1;
                        break;
                    }
                    else
                    {
                        call = 1;
                        break;
                    }
                }
                switch (newcond)
                {
                case 1:
                    if (code[i] == 'n' && isInArray(code[i - 1]) == true)
                    {
                        newcond = 2;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'e' && code[i - 1] == 'n')
                    {
                        newcond = 3;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'w' && code[i - 1] == 'e')
                    {
                        newcond = 4;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'c' && code[i - 1] == 'w')
                    {
                        newcond = 5;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'o' && code[i - 1] == 'c')
                    {
                        newcond = 6;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 6:
                    if (code[i] == 'n' && code[i - 1] == 'o')
                    {
                        newcond = 7;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 7:
                    if (code[i] == 'd' && code[i - 1] == 'n')
                    {
                        newcond = 8;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                case 8:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'd')
                    {
                        compiledFile << "[elseif]";
                        newcond = 1;
                        break;
                    }
                    else
                    {
                        newcond = 1;
                        break;
                    }
                }
                switch (pace)
                {
                case 1:
                    if (code[i] == 'p' && isInArray(code[i - 1]) == true && code[i + 1] == 'a')
                    {
                        pace = 2;
                        break;
                    }
                    else
                    {
                        pace = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'a' && code[i - 1] == 'p')
                    {
                        pace = 3;
                        break;
                    }
                    else
                    {
                        pace = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'c' && code[i - 1] == 'a')
                    {
                        pace = 4;
                        break;
                    }
                    else
                    {
                        pace = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'e' && code[i - 1] == 'c')
                    {
                        pace = 5;
                        break;
                    }
                    else
                    {
                        pace = 1;
                        break;
                    }
                case 5:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'e')
                    {
                        compiledFile << "[pace]";
                        pace = 1;
                        break;
                    }
                    else
                    {
                        pace = 1;
                        break;
                    }
                }
                switch (plus)
                {
                case 1:
                    if (code[i] == 'p' && isInArray(code[i - 1]) == true && code[i + 1] == 'l')
                    {
                        plus = 2;
                        break;
                    }
                    else
                    {
                        plus = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'l' && code[i - 1] == 'p')
                    {
                        plus = 3;
                        break;
                    }
                    else
                    {
                        plus = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'u' && code[i - 1] == 'l')
                    {
                        plus = 4;
                        break;
                    }
                    else
                    {
                        plus = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 's' && code[i - 1] == 'u')
                    {
                        plus = 5;
                        break;
                    }
                    else
                    {
                        plus = 1;
                        break;
                    }
                case 5:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 's')
                    {
                        compiledFile << "[plus]";
                        plus = 1;
                        break;
                    }
                    else
                    {
                        plus = 1;
                        break;
                    }
                }
                switch (minus)
                {
                case 1:
                    if (code[i] == 'm' && isInArray(code[i - 1]) == true && code[i + 1] == 'i' && code[i + 2] == 'n' && code[i + 3] == 'u')
                    {
                        minus = 2;
                        break;
                    }
                    else
                    {
                        minus = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'i' && code[i - 1] == 'm')
                    {
                        minus = 3;
                        break;
                    }
                    else
                    {
                        minus = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'n' && code[i - 1] == 'i')
                    {
                        minus = 4;
                        break;
                    }
                    else
                    {
                        minus = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'u' && code[i - 1] == 'n')
                    {
                        minus = 5;
                        break;
                    }
                    else
                    {
                        minus = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 's' && code[i - 1] == 'u')
                    {
                        minus = 6;
                        break;
                    }
                    else
                    {
                        minus = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 's')
                    {
                        compiledFile << "[minus]";
                        minus = 1;
                        break;
                    }
                    else
                    {
                        minus = 1;
                        break;
                    }
                }
                switch (inc)
                {
                case 1:
                    if (code[i] == 'i' && isInArray(code[i - 1]) == true && code[i + 1] == 'n')
                    {
                        inc = 2;
                        break;
                    }
                    else
                    {
                        inc = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'n' && code[i - 1] == 'i')
                    {
                        inc = 3;
                        break;
                    }
                    else
                    {
                        inc = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'c' && code[i - 1] == 'n')
                    {
                        inc = 4;
                        break;
                    }
                    else
                    {
                        inc = 1;
                        break;
                    }
                case 4:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'c')
                    {
                        compiledFile << "[inc]";
                        inc = 1;
                        break;
                    }
                    else
                    {
                        inc = 1;
                        break;
                    }
                }
                switch (multi)
                {
                case 1:
                    if (code[i] == 'm' && isInArray(code[i - 1]) == true && code[i + 1] == 'u')
                    {
                        multi = 2;
                        break;
                    }
                    else
                    {
                        multi = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'u' && code[i - 1] == 'm')
                    {
                        multi = 3;
                        break;
                    }
                    else
                    {
                        multi = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'l' && code[i - 1] == 'u')
                    {
                        multi = 4;
                        break;
                    }
                    else
                    {
                        multi = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 't' && code[i - 1] == 'l')
                    {
                        multi = 5;
                        break;
                    }
                    else
                    {
                        multi = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'i' && code[i - 1] == 't')
                    {
                        multi = 6;
                        break;
                    }
                    else
                    {
                        multi = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'i')
                    {
                        compiledFile << "[multi]";
                        multi = 1;
                        break;
                    }
                    else
                    {
                        multi = 1;
                        break;
                    }
                }
                switch (dec)
                {
                case 1:
                    if (code[i] == 'd' && isInArray(code[i - 1]) == true)
                    {
                        dec = 2;
                        break;
                    }
                    else
                    {
                        dec = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'e' && code[i - 1] == 'd')
                    {
                        dec = 3;
                        break;
                    }
                    else
                    {
                        dec = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'c' && code[i - 1] == 'e')
                    {
                        dec = 4;
                        break;
                    }
                    else
                    {
                        dec = 1;
                        break;
                    }
                case 4:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'c')
                    {
                        compiledFile << "[dec]";
                        dec = 1;
                        break;
                    }
                    else
                    {
                        dec = 1;
                        break;
                    }
                }
                switch (major)
                {
                case 1:
                    if (code[i] == 'm' && isInArray(code[i - 1]) == true && code[i + 1] == 'a' && code[i + 2] == 'j')
                    {
                        major = 2;
                        break;
                    }
                    else
                    {
                        major = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'a' && code[i - 1] == 'm')
                    {
                        major = 3;
                        break;
                    }
                    else
                    {
                        major = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'j' && code[i - 1] == 'a')
                    {
                        major = 4;
                        break;
                    }
                    else
                    {
                        major = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'o' && code[i - 1] == 'j')
                    {
                        major = 5;
                        break;
                    }
                    else
                    {
                        major = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'r' && code[i - 1] == 'o')
                    {
                        major = 6;
                        break;
                    }
                    else
                    {
                        major = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'r')
                    {
                        compiledFile << "[major]";
                        major = 1;
                        break;
                    }
                    else
                    {
                        major = 1;
                        break;
                    }
                }
                switch (minor)
                {
                case 1:
                    if (code[i] == 'm' && isInArray(code[i - 1]) == true)
                    {
                        minor = 2;
                        break;
                    }
                    else
                    {
                        minor = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'i' && code[i - 1] == 'm')
                    {
                        minor = 3;
                        break;
                    }
                    else
                    {
                        minor = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'n' && code[i - 1] == 'i')
                    {
                        minor = 4;
                        break;
                    }
                    else
                    {
                        minor = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'o' && code[i - 1] == 'n')
                    {
                        minor = 5;
                        break;
                    }
                    else
                    {
                        minor = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'r' && code[i - 1] == 'o')
                    {
                        minor = 6;
                        break;
                    }
                    else
                    {
                        minor = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'r')
                    {
                        compiledFile << "[minor]";
                        minor = 1;
                        break;
                    }
                    else
                    {
                        minor = 1;
                        break;
                    }
                }
                switch (start)
                {
                case 1:
                    if (code[i] == 's' && isInArray(code[i - 1]) == true && code[i + 1] == 't')
                    {
                        start = 2;
                        break;
                    }
                    else
                    {
                        start = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 't' && code[i - 1] == 's')
                    {
                        start = 3;
                        break;
                    }
                    else
                    {
                        start = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'a' && code[i - 1] == 't')
                    {
                        start = 4;
                        break;
                    }
                    else
                    {
                        start = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'r' && code[i - 1] == 'a')
                    {
                        start = 5;
                        break;
                    }
                    else
                    {
                        start = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 't' && code[i - 1] == 'r')
                    {
                        start = 6;
                        break;
                    }
                    else
                    {
                        start = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 't')
                    {
                        compiledFile << "[start]";
                        start = 1;
                        break;
                    }
                    else
                    {
                        start = 1;
                        break;
                    }
                }
                switch (end)
                {
                case 1:
                    if (code[i] == 'e' && isInArray(code[i - 1]) == true && code[i + 1] == 'n')
                    {
                        end = 2;
                        break;
                    }
                    else
                    {
                        end = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'n' && code[i - 1] == 'e')
                    {
                        end = 3;
                        break;
                    }
                    else
                    {
                        end = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'd' && code[i - 1] == 'n')
                    {
                        end = 4;
                        break;
                    }
                    else
                    {
                        end = 1;
                        break;
                    }
                case 4:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'd')
                    {
                        compiledFile << "[end]";
                        end = 1;
                        break;
                    }
                    else
                    {
                        end = 1;
                        break;
                    }
                }
                switch (cycle)
                {
                case 1:
                    if (code[i] == 'c' && isInArray(code[i - 1]) == true && code[i + 1] == 'y')
                    {
                        cycle = 2;
                        break;
                    }
                    else
                    {
                        cycle = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'y' && code[i - 1] == 'c')
                    {
                        cycle = 3;
                        break;
                    }
                    else
                    {
                        cycle = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'c' && code[i - 1] == 'y')
                    {
                        cycle = 4;
                        break;
                    }
                    else
                    {
                        cycle = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'l' && code[i - 1] == 'c')
                    {
                        cycle = 5;
                        break;
                    }
                    else
                    {
                        cycle = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'e' && code[i - 1] == 'l')
                    {
                        cycle = 6;
                        break;
                    }
                    else
                    {
                        cycle = 1;
                        break;
                    }
                case 6:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'e')
                    {
                        compiledFile << "[cycle]";
                        cycle = 1;
                        break;
                    }
                    else
                    {
                        cycle = 1;
                        break;
                    }
                }
                switch (cond)
                {
                case 1:
                    if (code[i] == 'c' && isInArray(code[i - 1]) == true)
                    {
                        cond = 2;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'o' && code[i - 1] == 'c')
                    {
                        cond = 3;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'n' && code[i - 1] == 'o')
                    {
                        cond = 4;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'd' && code[i - 1] == 'n')
                    {
                        cond = 5;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 5:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'd')
                    {
                        compiledFile << "[if]";
                        cond = 1;
                        break;
                    }
                    else if (code[i] == 'm' && code[i - 1] == 'd')
                    {
                        cond = 6;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }

                case 6:
                    if (code[i] == 'i' && code[i - 1] == 'm')
                    {
                        cond = 7;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 7:
                    if (code[i] == 'r' && code[i - 1] == 'i')
                    {
                        cond = 8;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 8:
                    if (code[i] == 'r' && code[i - 1] == 'r')
                    {
                        cond = 9;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 9:
                    if (code[i] == 'o' && code[i - 1] == 'r')
                    {
                        cond = 10;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 10:
                    if (code[i] == 'r' && code[i - 1] == 'o')
                    {
                        cond = 11;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                case 11:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'r')
                    {
                        compiledFile << "[else]";
                        cond = 1;
                        break;
                    }
                    else
                    {
                        cond = 1;
                        break;
                    }
                }
                switch (sendback)
                {
                case 1:
                    if (code[i] == 's' && isInArray(code[i - 1]) == true)
                    {
                        sendback = 2;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'e' && code[i - 1] == 's')
                    {
                        sendback = 3;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'n' && code[i - 1] == 'e')
                    {
                        sendback = 4;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'd' && code[i - 1] == 'n')
                    {
                        sendback = 5;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                case 5:
                    if (code[i] == 'b' && code[i - 1] == 'd')
                    {
                        sendback = 6;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }

                case 6:
                    if (code[i] == 'a' && code[i - 1] == 'b')
                    {
                        sendback = 7;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                case 7:
                    if (code[i] == 'c' && code[i - 1] == 'a')
                    {
                        sendback = 8;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                case 8:
                    if (code[i] == 'k' && code[i - 1] == 'c')
                    {
                        sendback = 9;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                case 9:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'k')
                    {
                        compiledFile << "[return]";
                        sendback = 1;
                        break;
                    }
                    else
                    {
                        sendback = 1;
                        break;
                    }
                }
                switch (func)
                {
                case 1:
                    if (code[i] == 'f' && isInArray(code[i - 1]) == true)
                    {
                        func = 2;
                        break;
                    }
                    else
                    {
                        func = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 'u' && code[i - 1] == 'f')
                    {
                        func = 3;
                        break;
                    }
                    else
                    {
                        func = 1;
                        break;
                    }
                case 3:
                    if (code[i] == 'n' && code[i - 1] == 'u')
                    {
                        func = 4;
                        break;
                    }
                    else
                    {
                        func = 1;
                        break;
                    }
                case 4:
                    if (code[i] == 'c' && code[i - 1] == 'n')
                    {
                        func = 5;
                        break;
                    }
                    else
                    {
                        func = 1;
                        break;
                    }
                case 5:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'c')
                    {
                        compiledFile << "[func]";
                        func = 1;
                        break;
                    }
                    else
                    {
                        func = 1;
                        break;
                    }
                }
                switch (eq)
                {
                case 1:
                    if (code[i] == 'e' && isInArray(code[i - 1]) == true)
                    {
                        eq = 2;
                        break;
                    }
                    else
                    {
                        eq = 1;
                        break;
                    }
                case 2:
                    if (code[i] == 'q' && code[i - 1] == 'e')
                    {
                        eq = 3;
                        break;
                    }
                    else
                    {
                        eq = 1;
                        break;
                    }

                case 3:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'q')
                    {
                        compiledFile << "[assign]";
                        eq = 1;
                        break;
                    }
                    else
                    {
                        eq = 1;
                        break;
                    }
                }
                switch (iseq)
                {
                case 1:
                    if (code[i] == 'i' && isInArray(code[i - 1]) == true)
                    {
                        iseq = 2;
                        break;
                    }
                    else
                    {
                        iseq = 1;
                        break;
                    }

                case 2:
                    if (code[i] == 's' && code[i - 1] == 'i')
                    {
                        iseq = 3;
                        break;
                    }
                    else
                    {
                        iseq = 1;
                        break;
                    }

                case 3:
                    if (code[i] == 'e' && code[i - 1] == 's')
                    {
                        iseq = 4;
                        break;
                    }
                    else
                    {
                        iseq = 1;
                        break;
                    }

                case 4:
                    if (code[i] == 'q' && code[i - 1] == 'e')
                    {
                        iseq = 5;
                        break;
                    }
                    else
                    {
                        iseq = 1;
                        break;
                    }

                case 5:
                    if ((isInArray(code[i]) || code[i] == EOF) && code[i - 1] == 'q')
                    {
                        compiledFile << "[compare]";
                        iseq = 1;
                        break;
                    }
                    else
                    {
                        iseq = 1;
                        break;
                    }
                }
                // switch case to determine var/func names
                switch (var)
                {
                case 1:
                    if ((code[i] >= 'A' && code[i] <= 'Z') || code[i] == '_' || (code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z') && isInArray(code[i - 1]) == true)
                    {
                        var = 2;
                        lexeme += code[i];
                        break;
                    }
                    else
                    {
                        lexeme = "";
                        var = 1;
                        break;
                    }

                case 2:
                    if ((code[i] >= 'A' && code[i] <= 'Z') || code[i] == '_' || (code[i] >= '0' && code[i] <= '9') || (code[i] >= 'a' && code[i] <= 'z'))
                    {
                        var = 2;
                        lexeme += code[i];
                        break;
                    }
                    // if read lexeme is approved generate token
                    else if ((isInArray(code[i]) || code[i] == EOF) && isKeyWord(lexeme) == false && (lexeme[0] >= 'a' && lexeme[0] <= 'z'))
                    {
                        // if lexeme was previously declared reuse its ID existing in lexemes map
                        if (isDeclared(lexeme))
                        {
                            var = 1;
                            compiledFile << "[" << returnID(lexeme) << "]";
                            lexeme = "";
                            break;
                        }
                        // if not create a new ID and write ID and lexeme as key:value pair to lexemes map
                        else
                        {
                            id++;
                            var = 1;
                            lexemes["id" + to_string(id)] = lexeme;
                            compiledFile << "[id" << id << "]";
                            lexeme = "";
                            break;
                        }
                    }
                    // if lexeme pattern not valid and lexeme is not a keyword print error
                    else if (isKeyWord(lexeme) == false)
                    {
                        errorFile << "Invalid Identifier! var/func name you have used in line " << lineCounter << ": '" << lexeme << "' does not follow the pattern!" << endl;
                        //          cout << "Invalid Identifier! var/func name you have used in line " << lineCounter << ": '" << lexeme << "' does not follow the pattern!" << endl;
                        lexeme = "";
                        var = 1;
                        break;
                    }
                    // if lexeme is correct but it is a keyword do nothing
                    else
                    {
                        lexeme = "";
                        var = 1;
                        break;
                    }
                }
                // multiple if statements to generate tokens for special characters
                if (code[i] == ':')
                    compiledFile << "[Colon]";
                if (code[i] == ';')
                    compiledFile << "[semiColon]";
                if (code[i] == ',')
                    compiledFile << "[Cama]";
                if (code[i] == '{')
                    compiledFile << "[aculadOpen]";
                if (code[i] == '}')
                    compiledFile << "[aculadClose]";
                if (code[i] == '[')
                    compiledFile << "[bracketOpen]";
                if (code[i] == ']')
                    compiledFile << "[bracketClose]";
                if (code[i] == '(')
                    compiledFile << "[parantOpen]";
                if (code[i] == ')')
                    compiledFile << "[parantClose]";
                if (code[i] == '=' && code[i + 1] == '=')
                {
                    compiledFile << "[Or]";
                }
                else if (code[i] == '=' && (code[i - 1] != '=' && code[i + 1] != '='))
                {
                    errorFile << "ERROR! Unexpected = in line " << lineCounter << " col " << column << endl;
                    //  cout << "ERROR! Unexpected = in line " << lineCounter << " col " << column << endl;
                }
                if (code[i] == '*' && code[i + 1] == '*')
                {
                    compiledFile << "[And]";
                }
                else if (code[i] == '*' && (code[i - 1] != '*' && code[i + 1] != '*'))
                {
                    errorFile << "ERROR! Unexpected * in line " << lineCounter << " col " << column << endl;
                    // cout << "ERROR! Unexpected * in line " << lineCounter << " col " << column << endl;
                }
            }
            // if input character is not accepted print error
            else
            {
                errorFile << "Invalid CHAR! not accepted character at line " << lineCounter << " col " << column << endl;
                //  cout << "Invalid CHAR! not accepted character at line " << lineCounter << " col " << column << endl;
            }
        }
    }
    compiledFile.close();
}
int main()
{
    string FileContent;
    ifstream ReadFile(".\\data\\code.txt");
    while (ReadFile)
    {
        FileContent += ReadFile.get();
    }
    FileContent = " " + FileContent + " ";
    int length = FileContent.length();
    // generate all tokens and write them to a separate txt file
    tokenGenerator(FileContent, length);
    return 0;
}