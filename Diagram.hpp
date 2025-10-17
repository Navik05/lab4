#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP

#include "defs.hpp"
#include "Scaner.hpp"

class TDiagram
{
private:
    TScaner *sc;
    // функции синтаксических диаграмм
    void W(); // объявление константы
    void D(); // объявление функции
    void E(); // тип
    void F(); // список
    void H(); // оператор или описание
    void I(); // объявление переменной
    void K(); // присваивание
    void X(); // for
    void O(); // выражение
    void Y(); // логическоеИ
    void Q(); // равенство
    void R(); // сравнение
    void V(); // слагаемое
    void T(); // множитель
    void U(); // эл.выр.
    void C(); // константа
    void Z(); // вызов функции
public:
    TDiagram(TScaner * s) {sc=s;}
    ~TDiagram(){}
    void S(); // программа
};
#endif