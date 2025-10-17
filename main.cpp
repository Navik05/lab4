#include "Scaner.hpp"
#include "Diagram.hpp"

int main()
{
    const char* FileName = "input.txt";
    TScaner* scaner = new TScaner(FileName);

    TDiagram* diagram = new TDiagram(scaner );
    diagram->S();

    int type;
    TypeLex lex;

    type = scaner->Scaner(lex);
    if(type==TEnd) 
        printf("синтаксических ошибок не обнаружено\n");
    else
        scaner->PrintError("лишний текст в конце программы","");

    return 0;
}