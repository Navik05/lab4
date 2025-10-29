#ifndef SEMANT_HPP
#define SEMANT_HPP

#include "defs.hpp"
#include "Scaner.hpp"
#include <string.h>
#include <algorithm>

extern class TScaner* globalScanner;

enum DATA_TYPE {TYPE_UNKNOWN=1, TYPE_BOOL, TYPE_CHAR, TYPE_DOUBLE, 
    TYPE_INTEGER, TYPE_FUNCT, TYPE_CONST
};

struct Node{
	// данные, общие для всех типов объектов
	TypeLex id;		// идентификатор переменной
	int DataType;		// тип значения
	 
	// обязательные данные для некоторых типов объектов
	int FlagConst = 0;		// признак константы
	char * Data;		// ссылка на значение константы или NULL
	// необязательные данные
	int FlagInit;		// Флаг начальной инициализации
};

class Tree{
Node * n;				// данные таблицы
Tree * Up, * Left, * Right;		// родитель, левый и правый потомок

public:
static Tree * Cur; // текущий элемент дерева

// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА
Tree(Tree * l, Tree * r, Tree * u, Node * Data);	// конструктор создает узел с заданными связями и данными
Tree();		// конструктор создает новый узел с пустыми связями и данными
~Tree();
void SetLeft (Node * Data);		// создать левого потомка от вершины From
void SetRight(Node * Data); 		// создать правого потомка от вершины From
Tree * FindUp (TypeLex id); 	// поиск данных в дереве, начиная от текущей вершины this до его корня вверх по связям
/* поиск элемента id вверх по дереву от текущей вершины From. 
Поиск осуществляется на одном уровне вложенности по левым связям */
Tree * FindUpOneLevel(Tree * From, TypeLex id); 
void Print(void);			// отладочная программа печати дерева

// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
void SetCur(Tree * a) ;                         // установить текущий узел дерева
Tree * GetCur(void);                            // получить значение текущего узла дерева
Tree * SemInclude(TypeLex a, DATA_TYPE t);      // занесение идентификатора a в таблицу с типом t
void SemSetType(Tree *Addr, DATA_TYPE t);       // установить тип t для переменной по адресу Addr
Tree * SemGetType(TypeLex a);           // найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
Tree * SemGetFunct(TypeLex a);          // найти в таблице функцию с именем a и вернуть ссылку на соответствующий элемент дерева
Tree * SemGetConst(TypeLex a);          // найти в таблице константу с именем a и вернуть ссылку на соответствующий элемент дерева
int DupControl(Tree *Addr, TypeLex a);  // проверка идентификатора а на повторное описание внутри блока
void SemEnterBlock(void);               // создать новую область видимости для составного оператора
void SemLeaveBlock(void);               //выйти из текущей области видимости
};
#endif