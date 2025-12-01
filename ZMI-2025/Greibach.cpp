#include "stdafx.h"
#include "Greibach.h"

namespace GRB {

    // Грамматика Грейбах для твоего варианта
    // S - стартовый символ
    // N - операторы (инструкции) внутри функций
    // E - выражения
    // T - тело функции или блока
    // F - параметры функции

    Greibach greibach(
        NS('S'),    // Стартовый нетерминал
        TS('$'),    // Дно стека
        12,         // Количество правил

        // 1. S (Start) - Структура программы: функции и main
        Rule(
            NS('S'),
            GRB_ERROR_SERIES + 0,
            8,
            // main { ... }
            Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
            // integer function(...) { ... } ; S
            Rule::Chain(7, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S')),
            Rule::Chain(8, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S')),
            // string function(...) ...
            Rule::Chain(7, TS('s'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S')),
            Rule::Chain(8, TS('s'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S')),
            // char function(...) ... (НОВОЕ)
            Rule::Chain(7, TS('c'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S')),
            Rule::Chain(8, TS('c'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S')),
            // Просто main в конце рекурсии
            Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), NS('S'))
        ),

        // 2. T (Body) - Тело функции (блок с return)
        Rule(
            NS('T'),
            GRB_ERROR_SERIES + 0,
            4,
            // { N }
            Rule::Chain(3, TS('{'), NS('N'), TS('}')),
            // { N return ; }
            Rule::Chain(5, TS('{'), NS('N'), TS('r'), TS(';'), TS('}')),
            // { N return E ; }
            Rule::Chain(6, TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
            // { return E ; }
            Rule::Chain(5, TS('{'), TS('r'), NS('E'), TS(';'), TS('}'))
        ),

        // 3. N (Next/Statements) - Список операторов
        Rule(
            NS('N'),
            GRB_ERROR_SERIES + 1,
            24,
            // Объявления переменных: type id;
            Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')), // integer i; N
            Rule::Chain(3, TS('t'), TS('i'), TS(';')),          // integer i; (конец)
            Rule::Chain(4, TS('s'), TS('i'), TS(';'), NS('N')), // string i; N
            Rule::Chain(3, TS('s'), TS('i'), TS(';')),
            Rule::Chain(4, TS('c'), TS('i'), TS(';'), NS('N')), // char i; N
            Rule::Chain(3, TS('c'), TS('i'), TS(';')),

            // Присваивание: id = Expr;
            Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
            Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),

            // Вызов функции: id(args);
            Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')), // с параметрами
            Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
            Rule::Chain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('N')),       // без параметров
            Rule::Chain(4, TS('i'), TS('('), TS(')'), TS(';')),

            // COUT: cout << Expr; (лексема << это 'v')
            // Правило: o v E ;
            Rule::Chain(5, TS('o'), TS('v'), NS('E'), TS(';'), NS('N')),
            Rule::Chain(4, TS('o'), TS('v'), NS('E'), TS(';')),
            // Цепочка cout: cout << E << E ...
            // Для упрощения сделаем пока один вывод, или рекурсию через E?
            // Оставим пока простой вывод одного выражения.

            // SWITCH: switch(E) { case ... }
            // Лексемы: h (switch), a (case), d (default)
            // Правило: h ( E ) { K }
            Rule::Chain(6, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('K')), // Переход в K (Cases)

            // Стандартные функции вывода (print/runout), если остались
            Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
            Rule::Chain(3, TS('p'), NS('E'), TS(';'))
        ),

        // 4. K (Cases) - Блок case-ов внутри switch
        // Структура: case L: N break; ... default: N
        // Упростим: case L: N
        Rule(
            NS('K'),
            GRB_ERROR_SERIES + 6,
            4,
            // case Lit: N; K (рекурсия)
            Rule::Chain(6, TS('a'), TS('l'), TS(':'), NS('N'), TS('}'), NS('N')), // Конец switch и продолжение N
            Rule::Chain(5, TS('a'), TS('l'), TS(':'), NS('N'), NS('K')),          // Еще один case

            // default: N; }
            Rule::Chain(5, TS('d'), TS(':'), NS('N'), TS('}'), NS('N')),
            // Просто закрывающая скобка (пустой switch или конец)
            Rule::Chain(2, TS('}'), NS('N'))
        ),

        // 5. E (Expression) - Выражения
        // (Арифметика, литералы, скобки)
        Rule(
            NS('E'),
            GRB_ERROR_SERIES + 2,
            12,
            Rule::Chain(1, TS('i')),        // id
            Rule::Chain(1, TS('l')),        // literal (число, char, bool)
            Rule::Chain(3, TS('('), NS('E'), TS(')')), // (E)

            // Операции: i + E, l + E, (E) + E
            Rule::Chain(2, TS('i'), NS('M')),
            Rule::Chain(2, TS('l'), NS('M')),
            Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),

            // Вызов функции в выражении: i(...)
            Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
            Rule::Chain(3, TS('i'), TS('('), TS(')')),
            Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')), // func() + ...
            Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),

            // ! E (логическое НЕ, если нужно)
            // Rule::Chain(2, TS('!'), NS('E'))

            // true/false лексемы (T, F) как литералы E
            Rule::Chain(1, TS('T')),
            Rule::Chain(1, TS('F'))
        ),

        // 6. M (Math/Operations suffix) - продолжение выражения
        // + E, - E, * E, / E, < E, > E, == E ...
        // Все операторы у нас 'v'
        Rule(
            NS('M'),
            GRB_ERROR_SERIES + 2,
            2,
            Rule::Chain(2, TS('v'), NS('E')),         // + E
            Rule::Chain(3, TS('v'), NS('E'), NS('M')) // + E + ...
        ),

        // 7. F (Func params definition) - Объявление параметров: type id, ...
        Rule(
            NS('F'),
            GRB_ERROR_SERIES + 3,
            6,
            Rule::Chain(2, TS('t'), TS('i')), // int id
            Rule::Chain(2, TS('s'), TS('i')), // string id
            Rule::Chain(2, TS('c'), TS('i')), // char id
            Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')),
            Rule::Chain(4, TS('s'), TS('i'), TS(','), NS('F')),
            Rule::Chain(4, TS('c'), TS('i'), TS(','), NS('F'))
        ),

        // 8. W (Call params) - Параметры при вызове: id, lit, ...
        Rule(
            NS('W'),
            GRB_ERROR_SERIES + 4,
            4,
            Rule::Chain(1, TS('i')),
            Rule::Chain(1, TS('l')),
            Rule::Chain(3, TS('i'), TS(','), NS('W')),
            Rule::Chain(3, TS('l'), TS(','), NS('W'))
        ),

        // Вспомогательные правила, если нужны были для условий, удалены или упрощены
        // так как switch у нас теперь основной
        Rule(NS('I'), GRB_ERROR_SERIES + 6, 1, Rule::Chain(1, TS('l'))), // Заглушка
        Rule(NS('B'), GRB_ERROR_SERIES + 6, 1, Rule::Chain(1, TS('l'))), // Заглушка
        Rule(NS('X'), GRB_ERROR_SERIES + 6, 1, Rule::Chain(1, TS('l'))), // Заглушка
        Rule(NS('O'), GRB_ERROR_SERIES + 7, 1, Rule::Chain(1, TS('l')))  // Заглушка
    );

    // ... (Методы класса Rule::Chain и Greibach остаются без изменений)
    // Просто скопируй их из своего старого файла снизу, начиная с 
    // Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
    // ...

    // Я продублирую их здесь, чтобы было удобно скопировать целиком файл:

    Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
    {
        nt = new GRBALPHABET[size = psize];
        int* p = (int*)&s;
        for (short i = 0; i < psize; i++) nt[i] = (GRBALPHABET)p[i];
    }
    char* Rule::Chain::getCChain(char* b)
    {
        for (int i = 0; i < size; i++)
            b[i] = alphabet_to_char(nt[i]);
        b[size] = 0x00;
        return b;
    }
    Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
    {
        nn = pnn;
        iderror = piderror;
        chains = new Chain[size = psize];
        Chain* p = &c;
        for (int i = 0; i < size; i++) chains[i] = p[i];
    }
    char* Rule::getCRule(char* b, short nchain)
    {
        char bchain[200];
        b[0] = Chain::alphabet_to_char(nn);
        b[1] = '-';
        b[2] = '>';
        b[3] = 0x00;
        chains[nchain].getCChain(bchain);
        strcat_s(b, sizeof(bchain) + 5, bchain);
        return b;
    }
    short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
    {
        short rc = -1;
        while (j < size && chains[j].nt[0] != t)
            j++;
        rc = (j < size ? j : -1);
        if (rc >= 0)
            pchain = chains[rc];
        return rc;
    }
    Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
    {
        startN = pstartN;
        stbottomT = pstbottom;
        rules = new Rule[size = psize];
        Rule* p = &r;
        for (int i = 0; i < size; i++)
            rules[i] = p[i];
    }
    short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
    {
        short rc = -1;
        short k = 0;
        while (k < size && rules[k].nn != pnn)
            k++;
        if (k < size)
            prule = rules[rc = k];
        return rc;
    }
    Rule Greibach::getRule(short n)
    {
        Rule rc;
        if (n < size)
            rc = rules[n];
        return rc;
    }
    Greibach getGreibach()
    {
        return greibach;
    }
}