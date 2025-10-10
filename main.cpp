#include "Scaner.hpp"

int main()
{
    const char* FileName = "input.txt";
    TScaner * scaner = new TScaner(FileName);
    int type;
    TypeLex lex;

    do{
        type = scaner->Scaner(lex);
        printf("%s - тип %d \n",lex, type);
    }while(type!=TEnd);

    return 0;
}