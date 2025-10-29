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

public:
    void PutUK (int);
    int GetUK ();
    static void PrintError(const char *, const char *);
    int Scaner (TypeLex);
    void GetData(const char *);
    TScaner(const char *);
    ~TScaner() {}
};
#endif