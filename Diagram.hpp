#ifndef DIAGRAM_HPP
#define DIAGRAM_HPP

#include "defs.hpp"
#include "Scaner.hpp"
#include "Semant.hpp"

class TDiagram
{
private:
    TScaner *sc;
    Tree *Root;  // Указатель на семантическое дерево

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
    DATA_TYPE GetDataType(int token);
public:
    TDiagram(TScaner * s);
    ~TDiagram();
    Tree* GetSemanticTree() { return Root; }
    void S(); // программа
};
#endif