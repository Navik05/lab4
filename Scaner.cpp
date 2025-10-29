#include "Scaner.hpp"
#include "Semant.hpp"

TScaner::TScaner(const char *FileName){
    currentLine = 1;
    currentColumn = 1;
    // Инициализируем массив начал строк
    for(int i = 0; i < MaxText; i++) {
        lineStarts[i] = 0;
    }
    GetData(FileName);
    PutUK(0);
}

void TScaner::UpdatePosition() {
    // Пересчитываем позицию с начала файла
    currentLine = 1;
    currentColumn = 1;
    
    for(int pos = 0; pos < uk && pos < MaxText; pos++) {
        if(t[pos] == '\n') {
            currentLine++;
            currentColumn = 1;
        } else {
            currentColumn++;
        }
    }
}

void TScaner::PutUK(int i){
    uk = i;
    UpdatePosition();
}

int TScaner::GetUK(){
    return uk;
}

int TScaner::GetCurrentLine() {
    return currentLine;
}

int TScaner::GetCurrentColumn() {
    return currentColumn;
}

void TScaner::PrintError(const char * err, const char * a, int line, int column){
    if (line == -1) {
        // Если строка не указана, используем информацию о текущей позиции
        if (a[0]=='\0')
            printf("Ошибка : %s \n",err);
        else
            printf("Ошибка : %s. Неверный символ '%s'\n",err,a);
    } else {
        // Выводим информацию о строке и позиции
        if (a[0]=='\0')
            printf("Ошибка в строке %d, позиция %d: %s \n", line, column, err);
        else
            printf("Ошибка в строке %d, позиция %d: %s. Неверный символ '%s'\n", line, column, err, a);
    }
    exit(0);
}

void TScaner::GetData(const char *FileName)
{
    // Открытие файла
    FILE * in = fopen(FileName,"r");
    if(in==NULL){
        PrintError("Не удалось открыть файл", "");
        exit(EXIT_FAILURE);
    }

    // Чтение файла и запись в массив 
    char symbol;
    int i;
    int len = MaxText-1;

    for(i = 0; !feof(in); i++){
        fscanf(in, "%c", &symbol);
        if(!feof(in)) {
            t[i]=symbol;
        }
        if(i>=len){
            PrintError("Слишком большой размер исходного модуля", "");
            break;
        }
    }

    t[i]='\0';
    fclose(in);
}

TypeLex Keyword[MaxKeyw]={ "const", "bool",
    "char", "double", "int", "return",
    "for",  "main", "true", "false"
};

int IndexKeyword[MaxKeyw]={ TConst, TBool,
    TChar, TDouble, TInt, TReturn, 
    TFor, TMain, TTrue, TFalse
};

int TScaner::Scaner(TypeLex l){
    int i;          // текущая длина лексемы
    int errorLine = currentLine;   // сохраняем позицию для ошибок
    int errorColumn = currentColumn;
    
    // Очистили поле лексемы
    for (i=0;i<MaxLex;i++) l[i]=0;
    
    i=0;
    
    start:
    // Пропуск незначащих элементов
    while((t[uk]==' ') || (t[uk]=='\n') || (t[uk]=='\t')) {
        if(t[uk] == '\n') {
            errorLine++;
            errorColumn = 1;
        } else {
            errorColumn++;
        }
        uk++;
    }

    // Обновляем позицию после пропуска пробелов
    errorLine = currentLine;
    errorColumn = currentColumn;

    // Пропуск комментария
    if((t[uk]=='/') && (t[uk+1]=='/')){
        for(uk+=2; t[uk]!='\n' && t[uk]!='\0'; uk++) {
            if(t[uk] == '\n') {
                errorLine++;
                errorColumn = 1;
            } else {
                errorColumn++;
            }
        }
        goto start;
    }

    // Конец исходного модуля
    if (t[uk]=='\0') {
        l[0]='\0'; 
        return TEnd;
    }

    // Константы вещественные в экспоненциальной форме
    if ((t[uk]>='0') && (t[uk]<='9')){
        l[i++]=t[uk++];
        while ((t[uk]>='0') && (t[uk]<='9'))
            if (i<MaxLex-1) 
                l[i++]=t[uk++]; 
            else uk++;

        if (t[uk]=='.'){
N3:         l[i++]=t[uk++];
            while ((t[uk]>='0') && (t[uk]<='9'))
            if (i<MaxLex-1) 
                l[i++]=t[uk++]; 
            else uk++;

            if( (t[uk]=='E')||(t[uk]=='e') ){
N1:             l[i++]=t[uk++]; 
                if((t[uk]=='+')||(t[uk]=='-')){
                    l[i++]=t[uk++]; 
                    if ((t[uk]>='0') && (t[uk]<='9')){
N2:                     l[i++]=t[uk++];
                        while ((t[uk]>='0') && (t[uk]<='9'))
                            if (i<MaxLex-1) 
                                l[i++]=t[uk++]; 
                            else uk++;
                        return TConsExp;
                    }
                }
                if ((t[uk]>='0') && (t[uk]<='9'))
                    goto N2;
            }
        }    
        if( (t[uk]=='E')||(t[uk]=='e') ) 
            goto N1; 
        
        PrintError("Неверная вещественная константа в экспоненциальной форме", "", errorLine, errorColumn);
        return TErr;
    }
    else if (t[uk]=='.'){
        l[i++]=t[uk++];
        if ((t[uk]>='0') && (t[uk]<='9'))
            goto N3;
        else{
            PrintError("Неверная вещественная константа в экспоненциальной форме", "", errorLine, errorColumn);
            return TErr;
        }
    }

    // Идентификатор
    else if ( ((t[uk]>='a') && (t[uk]<='z')) || ((t[uk]>='A') && (t[uk]<='Z')) || t[uk]=='_'){
        l[i++]=t[uk++];
        while (((t[uk]>='a') && (t[uk]<='z')) || ((t[uk]>='A') && (t[uk]<='Z')) || 
        t[uk]=='_' || ((t[uk]>='0') && (t[uk]<='9')))
        // Обрезали длинный идентификатор
            if (i<MaxLex-1) 
                l[i++]=t[uk++]; 
            else uk++;

        // Проверка на ключевое слово
        for(int j=0; j<MaxKeyw; j++)
            if(strcmp(l,Keyword[j])==0) return IndexKeyword[j];

        return TIdent;
    }

    // Символьная константа
    else if(t[uk]=='\''){
        uk++;
        if (((t[uk]>='a') && (t[uk]<='z')) || ((t[uk]>='A') && (t[uk]<='Z')) || 
        t[uk]=='_' || ((t[uk]>='0') && (t[uk]<='9'))){
            l[i++]=t[uk++];
            if(t[uk]=='\''){
                uk++;
                return TConstChar;
            }     
        }
        PrintError("Неверная символьная константа","", errorLine, errorColumn);
        return TErr;
    }

    // Специальные знаки
    else if (t[uk]==','){ 
        l[i++]=t[uk++]; 
        return TZpt; 
    }
    else if (t[uk]==';'){ 
        l[i++]=t[uk++]; 
        return TTZpt; 
    }
    else if (t[uk]=='('){ 
        l[i++]=t[uk++]; 
        return TLS; 
    }
    else if (t[uk]==')'){ 
        l[i++]=t[uk++]; 
        return TPS; 
    }
    else if (t[uk]=='{'){ 
        l[i++]=t[uk++]; 
        return TFLS; 
    }
    else if (t[uk]=='}'){ 
        l[i++]=t[uk++]; 
        return TFPS; 
    }

    // Знаки операций
    else if (t[uk]=='<'){
        l[i++]=t[uk++];
        if (t[uk]=='='){ 
            l[i++]=t[uk++]; 
            return TLE; 
        }
        return TLT;
    }
    else if (t[uk]=='>'){
        l[i++]=t[uk++];
        if (t[uk]=='='){ 
            l[i++]=t[uk++]; 
            return TGE; 
        }
        return TGT;
    }
    else if (t[uk]=='!'){
        l[i++]=t[uk++];
        if (t[uk]=='='){ 
            l[i++]=t[uk++]; 
            return TNEQ; 
        }
        return TNot;
    }
    else if (t[uk]=='='){
        l[i++]=t[uk++];
        if (t[uk]=='='){ 
            l[i++]=t[uk++]; 
            return TEQ; 
        }
        return TSave;
    }
    else if (t[uk]=='+'){
         l[i++]=t[uk++]; 
         return TPlus; 
        }
    else if (t[uk]=='-'){
         l[i++]=t[uk++]; 
         return TMinus; 
        }
    else if (t[uk]=='*'){
         l[i++]=t[uk++]; 
         return TMult; 
        }
    else if (t[uk]=='/'){
         l[i++]=t[uk++]; 
         return TDiv; 
        }
    else if (t[uk]=='&'){
        l[i++]=t[uk++];
        if (t[uk]=='&'){ 
            l[i++]=t[uk++]; 
            return TAnd; 
        }
        PrintError("Неверная логическая операция", "", errorLine, errorColumn);
        return TErr;
    }
    else if (t[uk]=='|'){
        l[i++]=t[uk++];
        if (t[uk]=='|'){ 
            l[i++]=t[uk++]; 
            return TOr; 
        }
        PrintError("Неверная логическая операция", "", errorLine, errorColumn);
        return TErr;
    }
    
    // Несуществующий символ
    else { 
        // Создаем строку с ошибочным символом
        char errorChar[2] = {t[uk], '\0'};
        PrintError("Несуществующий символ", errorChar, errorLine, errorColumn);
        uk++;
        return TErr;
    }
}