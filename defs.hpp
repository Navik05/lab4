#ifndef DEFS_HPP
#define DEFS_HPP

#define MaxText 100000          // максимальная длина текста исходнго модуля
#define MaxLex 20               // максимальная длина лексемы
#define MaxKeyw 10              // число ключевых слов

typedef char TypeMod[MaxText];  // текст исходного модуля
typedef char TypeLex[MaxLex];   // лексема

// Ключевые слова
#define TConst 1
#define TBool 2
#define TChar 3
#define TDouble 4
#define TInt 5
#define TReturn 6
#define TFor 7
#define TMain 8

// Идентификаторы
#define TIdent 20

// Логические константы
#define TTrue 30
#define TFalse 31

// Константы вещественные  в экспоненциальной форме
#define TConsExp 32

// Символьные константы
#define TConstChar 33

// Специальные знаки
#define TZpt 41
#define TTZpt 42
#define TLS 43
#define TPS 44
#define TFLS 45
#define TFPS 46

// Знаки операций
#define TLT 50
#define TLE 51
#define TGT 52
#define TGE 53
#define TEQ 54
#define TNEQ 55
#define TPlus 56
#define TMinus 57
#define TMult 58
#define TDiv 59
#define TSave 60
#define TNot 61
#define TAnd 62
#define TOr 63

// Конец исходного модуля
#define TEnd 100

// Ошибочный символ
#define TErr 200

#endif