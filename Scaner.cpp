#include "Scaner.hpp"

TScaner::TScaner(const char *FileName){
    GetData(FileName);
    PutUK(0);
}

void TScaner::PutUK(int i){
    uk=i;
}

int TScaner::GetUK(){
    return uk;
}

void TScaner::PrintError(const char * err, const char * a){
    if (a[0]=='\0')
        printf("Ошибка : %s \n",err);
    else
    printf("Ошибка : %s. Неверный символ %s\n",err,a);
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
        if(!feof(in)) t[i]=symbol;
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
    
    // Очистили поле лексемы
    for (i=0;i<MaxLex;i++) l[i]=0;
    
    i=0;
    
    start:
    // Пропуск незначащих элементов
    while((t[uk]==' ') || (t[uk]=='\n') || (t[uk]=='\t')) uk++;

    // Пропуск комментария
    if((t[uk]=='/') && (t[uk+1]=='/')){
        for(uk+=2; t[uk]!='\n' && t[uk]!='\0'; uk++) ;
        goto start;

    }

    // Конец исходного модуля
    if (t[uk]=='\0') {
        l[0]='\0'; 
        return TEnd;
    }

    // Константы вещественные  в экспоненциальной форме
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
        
        PrintError("Неверная вещественная константа в экспоненциальной форме", "");
        return TErr;
    }
    else if (t[uk]=='.'){
        l[i++]=t[uk++];
        if ((t[uk]>='0') && (t[uk]<='9'))
            goto N3;
        else{
            PrintError("Неверная вещественная константа в экспоненциальной форме", "");
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
        PrintError("Неверная символьная константа","");
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
        PrintError("Неверная логическая операция", "");
        return TErr;
    }
    else if (t[uk]=='|'){
        l[i++]=t[uk++];
        if (t[uk]=='|'){ 
            l[i++]=t[uk++]; 
            return TOr; 
        }
        PrintError("Неверная логическая операция", "");
        return TErr;
    }
    
    // Несуществующий символ
    else { 
        PrintError("Несуществующий символ", l);
        uk++;
        return TErr;
    }
}