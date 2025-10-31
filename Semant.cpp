#include "Semant.hpp"

TScaner* globalScanner = nullptr;

// Деструктор
Tree::~Tree() {
    if (n != NULL) {
        delete n;
    }
}

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
    strcpy(n->id, "------");
    n->IDType = ID_UNKNOWN;
    n->DataType = DATA_UNKNOWN;
    n->Data[0] = '\0';
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
    Tree * i = From;  // текущая вершина поиска
    
    while (i != NULL && i->Up != NULL) {
        // Проверяем, что мы на том же уровне (левый потомок)
        if (i->Up->Right != i) {
            // Сравниваем идентификаторы
            if (memcmp(id, i->n->id, std::max(strlen(i->n->id), strlen(id))) == 0) {
                return i;  // нашли совпадающий идентификатор
            }
        }
        i = i->Up;  // поднимаемся наверх по связям
    }
    return NULL;  // не нашли
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

// занесение идентификатора a в таблицу
Tree * Tree::SemInclude(TypeLex a, ID_TYPE i, DATA_TYPE t){
    if (DupControl(Cur, a))
        globalScanner->PrintError("Повторное описание идентификатора ",a,   globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    Tree * v;
    Node b;
    if (i==TYPE_CONST){
        memcpy(b.id,a,strlen(a)+1);
        b.IDType = i;
        b.DataType=t;
        b.Data[0] = '\0';
        Cur->SetLeft (&b);      // сделали вершину - константу
        Cur = Cur->Left;
        return Cur;
    }
    else if(i==TYPE_VAR){
        memcpy(b.id,a,strlen(a)+1);
        b.IDType=i;
        b.DataType=t;
        b.Data[0] = '\0';
        Cur->SetLeft (&b);      // сделали вершину - переменную
        Cur = Cur->Left;
        return Cur;
    }
    else 
    {
        memcpy(b.id,a,strlen(a)+1);     // создаём вершину - функцию
        b.IDType=i;
        b.DataType=t;
        b.Data[0] = '\0';
        Cur->SetLeft (&b);          
        Cur = Cur->Left;

        v=Cur;                      // это точка возврата после выхода из функции
        
        memcpy(b.id,a,strlen(a)+1);
        b.IDType= ID_UNKNOWN;
        b.DataType=DATA_UNKNOWN;
        b.Data[0] = '\0';

        Cur->SetRight (&b);         // сделали пустую вершину
        Cur = Cur->Right;
        return v;
    }
}

// найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
Tree * Tree::SemGetType(TypeLex a){
    Tree * v=FindUp(a);
    if (v==NULL) globalScanner->PrintError("Отсутствует описание идентификатора ",a,  globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    if (v->n->IDType==TYPE_FUNCT) globalScanner->PrintError("Неверное использование вызова функции ",a,  globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    return v;
}

// найти в таблице функцию с именем a и вернуть ссылку на соответствующий элемент дерева
Tree * Tree::SemGetFunct(TypeLex a) {
    Tree * v=FindUp(a);
    if (v==NULL) globalScanner->PrintError("Отсутствует описание функции ",a,  globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    if (v->n->IDType!=TYPE_FUNCT) globalScanner->PrintError("Не является функцией идентификатор ",a,  globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    return v;
}

// Проверка идентификатора а на повторное описание внутри блока
int Tree::DupControl(Tree* Addr, TypeLex a){
    if (FindUpOneLevel(Addr, a)==NULL) return 0;
    return 1;
}

// Заненсения значения переменной
void Tree::SetData(TypeLex a, TypeLex data){
    Tree * v = FindUp(a);
    if (v->n->IDType == TYPE_CONST && v->n->Data[0]!='\0'){
            globalScanner->PrintError("Нельзя изменить значение константы", a, globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
            return;
        }
    switch(v->n->DataType) {        
         case TYPE_DOUBLE: {
            // Проверяем, что значение - вещественное число
            if (data[0] != '.' && (data[0] < '0' || data[0] > '9')) {
                globalScanner->PrintError("Несоответствие типов: ожидается вещественное число", data, globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
                return;
            }
            else
                strcpy(v->n->Data, data);
            break;
        } 
        
        case TYPE_CHAR: {
            // Проверяем, что значение - символьная константа
            if (strlen(data) != 1) {
                globalScanner->PrintError("Несоответствие типов: ожидается символьная константа", data, globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
                return;
            }
            else
                strcpy(v->n->Data, data);
            break;
        } 
        
        case TYPE_BOOL: {
            // Проверяем, что значение - логическая константа
            if(strcmp(data, "true") != 0 && strcmp(data, "false") != 0){
                globalScanner->PrintError("Несоответствие типов: ожидается true или false", data, globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
                return;
            }
            else
                strcpy(v->n->Data, data);
            break;
        }
        
        default:
            globalScanner->PrintError("Неизвестный тип данных переменной", a, globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    }
}

// Проверка типа возвращаемого значения функции
void Tree::CheckReturnType(DATA_TYPE expectedType, DATA_TYPE actualType) {
    if (expectedType != actualType) {
        globalScanner->PrintError("Тип возвращаемого значения не совпадает с объявленным типом функции", "", globalScanner->GetCurrentLine(), globalScanner->GetCurrentColumn());
    }
}