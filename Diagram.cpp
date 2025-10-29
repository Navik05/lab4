#include "Diagram.hpp"
#include "Semant.hpp"

Tree * Root;

// программа
void TDiagram::S(){
    int t, uk, f;
    TypeLex l;

    f=1;
    while(f==1){
        f=0;
        uk=sc->GetUK();
        t=sc->Scaner(l);
        if(t==TConst) {
            f=1;
            sc->PutUK(uk);
            W();
        }
        else if(t==TInt) {
            f=1;
            sc->PutUK(uk);
            D();
        }
        else if(t==TBool || t==TChar || t==TDouble){
            t=sc->Scaner(l);
            if(t==TIdent){
                t=sc->Scaner(l);
                sc->PutUK(uk);
                if(t==TLS){
                    f=1;
                    D();
                }
                else if(t==TSave || t==TZpt || t==TTZpt){
                    f=1;
                    I();
                }
                else sc->PrintError("неверное объявление переменной или функции",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
            }
            else sc->PrintError("ожидался идентификатор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
        }
    }
}

// объявление функции
void TDiagram::D(){
    int t, uk;
    TypeLex l;

    uk=sc->GetUK();
    t=sc->Scaner(l);
    if(t==TInt){
        t=sc->Scaner(l);
        if(t!=TMain) sc->PrintError("ожидался символ main",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    }
    else{
        sc->PutUK(uk);
        E();
        t=sc->Scaner(l);
        if (t!=TIdent) sc->PrintError("ожидался идентификатор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    }

    t=sc->Scaner(l);
    if(t!=TLS) sc->PrintError("ожидался символ (",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if(t!=TPS) sc->PrintError("ожидался символ )",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if(t!=TFLS) sc->PrintError("ожидался символ {",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    F();
    t=sc->Scaner(l);
    if(t!=TFPS) sc->PrintError("ожидался символ }",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
}

// список
void TDiagram::F(){
    int t, uk, f=1;
    TypeLex l;

    while (f==1){
        f=0;
        uk=sc->GetUK();
        t=sc->Scaner(l);

        if(t==TBool || t==TChar || t==TDouble) {
            t=sc->Scaner(l);
            t=sc->Scaner(l);
            sc->PutUK(uk);
            if(t==TLS){
                D();
                f=1;
            }
            else {
                I();
                f=1;
            }
        }
        else if(t==TFLS || t==TFor || t==TConst || t==TReturn || 
            t==TIdent || t==TTZpt){
            sc->PutUK(uk);
            H();
            f=1;
        }
        else if(t==TFPS) {
            sc->PutUK(uk);
            return;
        }
    }
}

// объявление переменной
void TDiagram::I(){
    int t, uk;
    TypeLex l;

    E();
    do{
    uk=sc->GetUK();
    t=sc->Scaner(l);
    t=sc->Scaner(l);
    sc->PutUK(uk);

    if(t==TSave) K();
    else{
        t=sc->Scaner(l);
        if (t!=TIdent) sc->PrintError("ожидался идентификатор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    }

    t=sc->Scaner(l);
    }while(t==TZpt);

    if(t!=TTZpt) sc->PrintError("ожидался знак ;",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
}

// присваивание
void TDiagram::K(){
    int t;
    TypeLex l;

    t=sc->Scaner(l);
    if (t!=TIdent) sc->PrintError("ожидался идентификатор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if (t!=TSave) sc->PrintError("ожидался знак =",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    O();
}

// оператор или описание
void TDiagram::H(){
    int t, uk;
    TypeLex l;

    uk=sc->GetUK();
    t=sc->Scaner(l);

    if(t==TFLS){
        F();
        t=sc->Scaner(l);
        if(t!=TFPS) sc->PrintError("ожидался символ }",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    }
    else if(t==TFor){
        sc->PutUK(uk);
        X();
    }
    else{
        if(t==TConst){
            sc->PutUK(uk);
            W();
            t=sc->Scaner(l);
        }
        else if(t==TReturn) {
            O();
            t=sc->Scaner(l);
        }
        else if(t==TIdent){
            t=sc->Scaner(l);
            if(t==TLS){
                sc->PutUK(uk);
                Z();
                t=sc->Scaner(l);
            }
            else if(t==TSave){
                sc->PutUK(uk);
                K();
                t=sc->Scaner(l);
            }
            else sc->PrintError("неверный оператор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
        }

        if(t!=TTZpt) sc->PrintError("ожидался знак ;",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    }
}

// вызов функции
void TDiagram::Z(){
    int t;
    TypeLex l;

    t=sc->Scaner(l);
    if (t!=TIdent) sc->PrintError("ожидался идентификатор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if (t!=TLS) sc->PrintError("ожидался символ (",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if (t!=TPS) sc->PrintError("ожидался символ )",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
}

// for
void TDiagram::X(){
    int t, uk;
    TypeLex l;

    t=sc->Scaner(l);
    if (t!=TFor) sc->PrintError("ожидалось ключевое слово for",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if (t!=TLS) sc->PrintError("ожидался символ (",l, sc->GetCurrentLine(), sc->GetCurrentColumn());

    uk=sc->GetUK();
    t=sc->Scaner(l);
    sc->PutUK(uk);
    if(t!=TTZpt) K();

    t=sc->Scaner(l);
    if(t!=TTZpt) sc->PrintError("ожидался знак ;",l, sc->GetCurrentLine(), sc->GetCurrentColumn());

    uk=sc->GetUK();
    t=sc->Scaner(l);
    sc->PutUK(uk);
    if(t!=TTZpt) O();

    t=sc->Scaner(l);
    if(t!=TTZpt) sc->PrintError("ожидался знак ;",l, sc->GetCurrentLine(), sc->GetCurrentColumn());

    uk=sc->GetUK();
    t=sc->Scaner(l);
    sc->PutUK(uk);
    if(t!=TPS) K();
    
    t=sc->Scaner(l);
    if (t!=TPS) sc->PrintError("ожидался символ )",l, sc->GetCurrentLine(), sc->GetCurrentColumn());

    H();
}

// выражение
void TDiagram::O(){
    int t, uk;
    TypeLex l;

    Y();
    uk=sc->GetUK();
    t=sc->Scaner(l);

    while(t==TOr){
        Y();
        uk=sc->GetUK();
        t=sc->Scaner(l);
    } 
    sc->PutUK(uk);
}

// логическоеИ
void TDiagram::Y(){
    int t, uk;
    TypeLex l;

    Q();
    uk=sc->GetUK();
    t=sc->Scaner(l);

    while(t==TAnd){
        Q();
        uk=sc->GetUK();
        t=sc->Scaner(l);
    } 
    sc->PutUK(uk);
}

// равенство
void TDiagram::Q(){
    int t, uk;
    TypeLex l;

    R();
    uk=sc->GetUK();
    t=sc->Scaner(l);

    while(t==TEQ || t==TNEQ){
        R();
        uk=sc->GetUK();
        t=sc->Scaner(l);
    } 
    sc->PutUK(uk);
}

// сравнение
void TDiagram::R(){
    int t, uk;
    TypeLex l;

    V();
    uk=sc->GetUK();
    t=sc->Scaner(l);

    while(t==TLT || t==TLE || t==TGT || t==TGE){
        V();
        uk=sc->GetUK();
        t=sc->Scaner(l);
    } 
    sc->PutUK(uk);
}

// слагаемое
void TDiagram::V(){
    int t, uk;
    TypeLex l;

    T();
    uk=sc->GetUK();
    t=sc->Scaner(l);

    while(t==TPlus || t==TMinus){
        T();
        uk=sc->GetUK();
        t=sc->Scaner(l);
    } 
    sc->PutUK(uk);
}

// множитель
void TDiagram::T(){
    int t, uk;
    TypeLex l;

    U();
    uk=sc->GetUK();
    t=sc->Scaner(l);

    while(t==TMult || t==TDiv){
        U();
        uk=sc->GetUK();
        t=sc->Scaner(l);
    } 
    sc->PutUK(uk);
}

// эл.выр.
void TDiagram::U(){
    int t, uk, uk2;
    TypeLex l;

    uk=sc->GetUK();
    t=sc->Scaner(l);
    if(t!=TPlus && t!=TMinus && t!=TNot) sc->PutUK(uk);

    uk=sc->GetUK();
    t=sc->Scaner(l);
    if(t==TTrue && t==TFalse && t==TConsExp && t==TConstChar){
        sc->PutUK(uk);
        C();
    } 
    else if(t==TLS){
        O();
        t=sc->Scaner(l);
        if (t!=TPS) sc->PrintError("ожидался символ )",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    }
    else if(t==TIdent){
        uk2=sc->GetUK();
        t=sc->Scaner(l);
        if(t==TLS) {
            sc->PutUK(uk);
            Z();
        }
        else sc->PutUK(uk2);
    }
}

// объявление константы
void TDiagram::W(){
    int t;
    TypeLex l;

    t=sc->Scaner(l);
    if (t!=TConst) sc->PrintError("ожидалось ключевое слово const",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    E();

    do{
    t=sc->Scaner(l);
    if (t!=TIdent) sc->PrintError("ожидался идентификатор",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    t=sc->Scaner(l);
    if (t!=TSave) sc->PrintError("ожидался знак =",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
    C();
    t=sc->Scaner(l);
    }while(t==TZpt);

    if(t!=TTZpt) sc->PrintError("ожидался знак ;",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
}

// тип
void TDiagram::E(){
    int t;
    TypeLex l;
    t=sc->Scaner(l);
    if(t!=TBool && t!=TChar && t!=TDouble) sc->PrintError("несуществующий тип данных",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
}

// константа
void TDiagram::C(){
    int t;
    TypeLex l;
    t=sc->Scaner(l);
    if(t!=TTrue && t!=TFalse && t!=TConsExp && t!=TConstChar)
        sc->PrintError("ошибочное значение константы",l, sc->GetCurrentLine(), sc->GetCurrentColumn());
}