#include "Scaner.hpp"
#include "Diagram.hpp"
#include "Semant.hpp"

int main()
{
    const char* FileName = "input.txt";
    TScaner* scaner = new TScaner(FileName);
    globalScanner = scaner;

    TDiagram* diagram = new TDiagram(scaner);
    diagram->S();

    int type;
    TypeLex lex;

    type = scaner->Scaner(lex);
    if(type==TEnd) {
        printf("ошибок не обнаружено\n");
        
        // ДОБАВЛЕНО: Вывод семантического дерева
        printf("\n=== СЕМАНТИЧЕСКОЕ ДЕРЕВО ===\n");
        Tree* rootTree = diagram->GetSemanticTree();
        if (rootTree != nullptr) {
            rootTree->Print();
        } else {
            printf("Дерево не построено\n");
        }
        printf("=============================\n");
    }
    else {
        scaner->PrintError("лишний текст в конце программы","", scaner->GetCurrentLine(), scaner->GetCurrentColumn());
    }

    delete diagram;
    delete scaner;
    globalScanner = nullptr;
    
    return 0;
}