#ifndef SCANER_HPP
#define SCANER_HPP

#include "defs.hpp"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

class TScaner
{
private:
    TypeMod t;          // исходный модуль
    int uk;             // указатель текущей позиции в исходном модуле
    int currentLine;    // текущая строка
    int currentColumn;  // текущий столбец
    int lineStarts[MaxText]; // массив начал строк
    void UpdatePosition();

public:
    void PutUK (int);
    int GetUK ();
    int GetCurrentLine();
    int GetCurrentColumn();
    static void PrintError(const char *, const char *, int line = -1, int column = -1);
    int Scaner (TypeLex);
    void GetData(const char *);
    TScaner(const char *);
    ~TScaner() {}
};
#endif