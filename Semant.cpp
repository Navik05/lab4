#include "Semant.hpp"

// конструктор создает узел с заданными связями и данными
Tree::Tree (Tree * l, Tree * r, Tree * u, Node * d){
    n= new Node();
    Up=u; Left=l; Right=r;          // установили ссылки
    memcpy(n, d, sizeof(Node));      // установили данные
}

// конструктор создает новый узел с пустыми связями и данными
Tree::Tree (void){
    n= new Node();
    Up=NULL; Left=NULL; Right=NULL;
    memcpy(n,&("------"), sizeof(Node));
}

// создать левого потомка от текущей вершины this
void Tree::SetLeft (Node * Data)
{
    Tree * a= new Tree(NULL,NULL,this,Data);        // новая вершина
    Left=a;                                         // связали this с новой вершиной
}

// создать правого потомка от текущей вершины this
    void Tree::SetRight(Node * Data){
    Tree * a= new Tree(NULL,NULL,this,Data);            // новая вершина
    Right=a;                                            // связали this с новой вершиной     
}

// поиск данных в дереве, начиная от текущей вершины this до его корня вверх по связям
Tree * Tree::FindUp(TypeLex id){
    Tree * i=this->Cur;          // текущая вершина поиска
    while( (i!=NULL) && (memcmp(id, i->n->id, std::max(strlen(i->n->id),strlen(id)))!=0) )
        i=i->Up;            // поднимаемся наверх по связям
    return i;
}

// Поиск элемента id вверх по дереву от текущей вершины From. Поиск осуществляется на одном уровне вложенности по левым связям
Tree * Tree::FindUpOneLevel(Tree * From, TypeLex id){
    Tree * i=From;          // текущая вершина поиска
    while( (i!=NULL) && ( i->Up->Right != i)){
        if (memcmp(id, i->n->id, std::max(strlen(i->n->id),strlen(id)))==0)
        return i;               // нaшли совпадающий идентификатор
        i=i->Up;                // поднимаемся наверх по связям
    }
    return NULL;
}

// отладочная программа печати дерева
void Tree::Print (void){
    printf("Вершина с данными %s ----->", n->id );
    if (Left !=NULL) printf("слева данные %s", Left->n->id );
    if (Right!=NULL) printf("справа данные %s", Right->n->id );
    printf("\n");
    if (Left!=NULL) Left->Print();
    if (Right!=NULL) Right->Print();
}

Tree* Tree::Cur=(Tree*)NULL;

// установить текущий узел дерева
    void Tree::SetCur(Tree * a){
    Cur=a;
}

// получить значение текущего узла дерева
    Tree * Tree::GetCur(void){
    return Cur;
}

// занесение идентификатора a в таблицу с типом t
Tree * Tree::SemInclude(TypeLex a, DATA_TYPE t){
    if (DupControl(Cur, a))
        TScaner::PrintError("Повторное описание идентификатора ",a);

    Tree * v;
    Node b;
    if (t!=TYPE_FUNCT){
        memcpy(b.id,a,strlen(a)+1);
        b.DataType=t;
        b.Data=NULL;
        if(t==TYPE_CONST)
            b.FlagConst = 1;
        Cur->SetLeft (&b);      // сделали вершину - переменную или константу
        Cur = Cur->Left;
        return Cur;
    }
    else
    {
        memcpy(b.id,a,strlen(a)+1);
        b.DataType=t;
        b.Data=NULL;
        Cur->SetLeft (&b);          // сделали вершину - функцию
        Cur = Cur->Left;
        v=Cur;                      // это точка возврата после выхода из функции
        memcpy(&b.id,&"",2);
        b.DataType=TYPE_UNKNOWN;
        b.Data=NULL;
        Cur->SetRight (&b);         // сделали пустую вершину
        Cur = Cur->Right;
        return v;
    }
}

// установить тип t для переменной по адресу Addr
void Tree::SemSetType(Tree* Addr, DATA_TYPE t) {
    Addr->n->DataType=t;
}

// найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
Tree * Tree::SemGetType(TypeLex a){
    Tree * v=FindUp(a);
    if (v==NULL) TScaner::PrintError("Отсутствует описание идентификатора ",a);
    if (v->n->DataType==TYPE_FUNCT) TScaner::PrintError("Неверное использование вызова функции ",a);
    return v;
}

// найти в таблице функцию с именем a и вернуть ссылку на соответствующий элемент дерева
Tree * Tree::SemGetFunct(TypeLex a) {
    Tree * v=FindUp(a);
    if (v==NULL) TScaner::PrintError("Отсутствует описание функции ",a);
    if (v->n->DataType!=TYPE_FUNCT) TScaner::PrintError("Не является функцией идентификатор ",a);
    return v;
}

// Проверка идентификатора а на повторное описание внутри блока
int Tree::DupControl(Tree* Addr, TypeLex a){
    if (FindUpOneLevel(Addr, a)==NULL) return 0;
    return 1;
}