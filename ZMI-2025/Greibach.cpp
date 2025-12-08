#include "stdafx.h"
#include "Greibach.h"
#include <cstdarg>
#include <cstdio> // Для sprintf_s

namespace GRB {

    // --- Chain Methods ---
    char* Rule::Chain::getCChain(char* b) {
        for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
        b[size] = 0; return b;
    }

    // --- Rule Methods ---
    void Rule::AddChain(short psize, GRBALPHABET s, ...)
    {
        if (size >= GRB_MAX_CHAINS) return;
        chains[size].size = psize;
        chains[size].nt[0] = s;

        va_list args;
        va_start(args, s);
        for (short i = 1; i < psize; i++) {
            // ВАЖНО: va_arg берет int, так как short в varargs приводится к int
            chains[size].nt[i] = (GRBALPHABET)va_arg(args, int);
        }
        va_end(args);
        size++;
    }

    char* Rule::getCRule(char* b, short nchain) {
        char bchain[200];
        b[0] = Chain::alphabet_to_char(nn);
        b[1] = '-'; b[2] = '>'; b[3] = 0;
        chains[nchain].getCChain(bchain);
        strcat_s(b, 200, bchain);
        return b;
    }

    short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) {
        short rc = -1;
        // Ищем цепочку, начинающуюся с терминала t (или нетерминала, если это LL(1), но у нас Грейбах)
        // В Грейбах все правые части начинаются с терминала.
        while (j < size && chains[j].nt[0] != t) j++;
        rc = (j < size ? j : -1);
        if (rc >= 0) pchain = chains[rc];
        return rc;
    }

    // --- Greibach Methods ---
    short Greibach::getRule(GRBALPHABET pnn, Rule& prule) {
        short rc = -1;
        short k = 0;
        while (k < size && rules[k].nn != pnn) k++;
        if (k < size) {
            prule = rules[k];
            rc = k;
        }
        return rc;
    }

    Rule Greibach::getRule(short n) { return n < size ? rules[n] : Rule(); }


    // --- ГРАММАТИКА ---
    Greibach getGreibach()
    {
        Greibach g;
        g.startN = NS('S');
        g.stbottomT = TS('$');
        // Кол-во правил должно совпадать с тем, что мы заполним
        // Сейчас у нас индексы: 0 (S), 1 (T), 2 (N), 3 (K), 4 (E), 5 (F), 6 (W)
        // Итого 7 правил. Если добавишь еще, увеличь g.size.
        g.size = 7;

        // 1. Rule S (Start)
        g.rules[0].nn = NS('S'); g.rules[0].iderror = GRB_ERROR_SERIES + 0;
        g.rules[0].AddChain(3, TS('m'), TS('{'), NS('N'));
        // Добавил функции (func type id ...)
        // t=int, s=string, c=char, v=void (если есть в лексере)
        // В твоем лексере типы: t(int), s(string), c(char), n(void)
        // Внимательно с токенами! 
        // В LT.h: LEX_INTEGER 't', LEX_STRING 's', LEX_CHAR 'c', LEX_VOID 'n'

        // Функции
        g.rules[0].AddChain(8, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));

        g.rules[0].AddChain(8, TS('s'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, TS('s'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));

        g.rules[0].AddChain(8, TS('c'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, TS('c'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));

        // Void функции
        g.rules[0].AddChain(8, TS('n'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, TS('n'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));

        // Main
        g.rules[0].AddChain(4, TS('m'), TS('{'), NS('N'), NS('S'));
        // Пустая цепочка? Нет, S не должен исчезать, но рекурсия должна где-то дна достичь
        // У тебя 'm' (main) это и есть дно, или функции.
        // Если программа это список функций и main, то S -> Func S | Main. 
        // Если Main последний, то после него S не нужен? 
        // В твоей грамматике Main вызывает S рекурсивно: TS('m')...NS('S'). Это значит, что после Main может быть еще Main?
        // Обычно Main один. Сделаем так: Main завершает цепочку.
        // ИСПРАВЛЕНИЕ: Убрал NS('S') в конце для Main
        // g.rules[0].AddChain(3, TS('m'), TS('{'), NS('N')); 


        // 2. Rule T (Body of function) -> { N }
        g.rules[1].nn = NS('T'); g.rules[1].iderror = GRB_ERROR_SERIES + 0;
        g.rules[1].AddChain(2, TS('{'), NS('N'));

        // 3. Rule N (Statements inside body)
        g.rules[2].nn = NS('N'); g.rules[2].iderror = GRB_ERROR_SERIES + 1;
        g.rules[2].AddChain(1, TS('}')); // Конец блока

        // Объявления переменных: type id;
        g.rules[2].AddChain(4, TS('t'), TS('i'), TS(';'), NS('N'));
        g.rules[2].AddChain(4, TS('s'), TS('i'), TS(';'), NS('N'));
        g.rules[2].AddChain(4, TS('c'), TS('i'), TS(';'), NS('N'));

        // Присваивание: id = Expr;
        g.rules[2].AddChain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N'));

        // Вызов функции: id( Args );
        g.rules[2].AddChain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N'));
        g.rules[2].AddChain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('N'));

        // Вывод: cout << Expr; (Токен 'o' = cout, 'v' = << (если << это LEX_OPERATOR)
        // Но в твоем лексере cout это 'o'. А дальше?
        // В грамматике у тебя написано TS('o'), TS('v')...
        // Если cout << ... то << это оператор.
        g.rules[2].AddChain(5, TS('o'), TS('v'), NS('E'), TS(';'), NS('N'));

        // Return
        g.rules[2].AddChain(4, TS('r'), NS('E'), TS(';'), NS('N'));
        g.rules[2].AddChain(3, TS('r'), TS(';'), NS('N'));

        // Switch: switch(Expr) { K }
        // Токен switch = 'h'
        g.rules[2].AddChain(7, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('K'), NS('N'));

        // Печать (print?) 'p'? В лексере нет p. Оставим, если это cout.
        // g.rules[2].AddChain(4, TS('p'), NS('E'), TS(';'), NS('N'));

        // Пустой оператор
        g.rules[2].AddChain(2, TS(';'), NS('N'));


        // 4. Rule K (Switch Cases)
        g.rules[3].nn = NS('K'); g.rules[3].iderror = GRB_ERROR_SERIES + 6;
        g.rules[3].AddChain(1, TS('}')); // Конец switch
        // case literal : { N }
        // case = 'a', literal = 'l', : = ':', { = '{'
        g.rules[3].AddChain(6, TS('a'), TS('l'), TS(':'), TS('{'), NS('N'), NS('K'));
        // default : { N }
        // default = 'd'
        g.rules[3].AddChain(5, TS('d'), TS(':'), TS('{'), NS('N'), NS('K'));


        // 5. Rule E (Expression)
        g.rules[4].nn = NS('E'); g.rules[4].iderror = GRB_ERROR_SERIES + 2;

        // (E) op E
        g.rules[4].AddChain(5, TS('('), NS('E'), TS(')'), TS('v'), NS('E'));

        // Вызовы функций внутри выражений
        g.rules[4].AddChain(4, TS('i'), TS('('), NS('W'), TS(')'));
        g.rules[4].AddChain(3, TS('i'), TS('('), TS(')'));

        // Просто (E)
        g.rules[4].AddChain(3, TS('('), NS('E'), TS(')'));

        // id op E (Рекурсия)
        g.rules[4].AddChain(3, TS('i'), TS('v'), NS('E'));

        // lit op E
        g.rules[4].AddChain(3, TS('l'), TS('v'), NS('E'));

        // Терминальные состояния
        g.rules[4].AddChain(1, TS('i'));
        g.rules[4].AddChain(1, TS('l'));


        // 6. Rule F (Params definition in function decl)
        g.rules[5].nn = NS('F'); g.rules[5].iderror = GRB_ERROR_SERIES + 3;
        // type id, F
        g.rules[5].AddChain(4, TS('t'), TS('i'), TS(','), NS('F'));
        g.rules[5].AddChain(4, TS('s'), TS('i'), TS(','), NS('F'));
        g.rules[5].AddChain(4, TS('c'), TS('i'), TS(','), NS('F'));
        // type id
        g.rules[5].AddChain(2, TS('t'), TS('i'));
        g.rules[5].AddChain(2, TS('s'), TS('i'));
        g.rules[5].AddChain(2, TS('c'), TS('i'));


        // 7. Rule W (Args in function call)
        g.rules[6].nn = NS('W'); g.rules[6].iderror = GRB_ERROR_SERIES + 4;
        // id, W
        g.rules[6].AddChain(3, TS('i'), TS(','), NS('W'));
        g.rules[6].AddChain(3, TS('l'), TS(','), NS('W'));
        // id
        g.rules[6].AddChain(1, TS('i'));
        g.rules[6].AddChain(1, TS('l'));

        return g;
    }
}