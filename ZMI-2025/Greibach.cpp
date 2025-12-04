#include "stdafx.h"
#include "Greibach.h"
#include <cstdarg>

namespace GRB {

    // --- Chain ---
    Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
    {
        size = psize;
        if (size > GRB_MAX_LEN) size = GRB_MAX_LEN;
        nt[0] = s;
        va_list args;
        va_start(args, s);
        for (short i = 1; i < size; i++) nt[i] = (GRBALPHABET)va_arg(args, int);
        va_end(args);
    }

    char* Rule::Chain::getCChain(char* b) {
        for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
        b[size] = 0; return b;
    }

    // --- Rule ---
    Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) {
        nn = pnn; iderror = piderror; size = 0;
    }

    void Rule::AddChain(short psize, GRBALPHABET s, ...)
    {
        if (size >= GRB_MAX_CHAINS) return;
        chains[size].size = psize;
        chains[size].nt[0] = s;
        va_list args;
        va_start(args, s);
        for (short i = 1; i < psize; i++) {
            chains[size].nt[i] = (GRBALPHABET)va_arg(args, int);
        }
        va_end(args);
        size++;
    }

    char* Rule::getCRule(char* b, short nchain) {
        char bchain[200]; b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0;
        chains[nchain].getCChain(bchain); strcat_s(b, sizeof(bchain) + 5, bchain); return b;
    }

    short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) {
        short rc = -1;
        while (j < size && chains[j].nt[0] != t) j++;
        rc = (j < size ? j : -1);
        if (rc >= 0) pchain = chains[rc];
        return rc;
    }

    Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...) {
        startN = pstartN; stbottomT = pstbottom; size = 0;
    }

    short Greibach::getRule(GRBALPHABET pnn, Rule& prule) {
        short rc = -1; short k = 0;
        while (k < size && rules[k].nn != pnn) k++;
        if (k < size) prule = rules[rc = k];
        return rc;
    }
    Rule Greibach::getRule(short n) { return n < size ? rules[n] : Rule(); }


    // --- ФИНАЛЬНАЯ ГРАММАТИКА ("ВСЕЯДНАЯ") ---
    Greibach getGreibach()
    {
        Greibach g;
        g.startN = NS('S');
        g.stbottomT = TS('$');
        g.size = 8;

        // 1. Rule S (Start)
        g.rules[0].nn = NS('S'); g.rules[0].iderror = GRB_ERROR_SERIES + 0;

        // Функции
        g.rules[0].AddChain(7, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(8, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, TS('s'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(8, TS('s'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, TS('c'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(8, TS('c'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));

        // Main { N (N съест закрывающую скобку)
        g.rules[0].AddChain(4, TS('m'), TS('{'), NS('N'));
        g.rules[0].AddChain(5, TS('m'), TS('{'), NS('N'), NS('S'));

        // 2. Rule T (Body)
        g.rules[1].nn = NS('T'); g.rules[1].iderror = GRB_ERROR_SERIES + 0;
        g.rules[1].AddChain(2, TS('{'), NS('N'));

        // 3. Rule N (Universal Statement List)
        // Это правило умеет ВСЁ. Оно обрабатывает код внутри {} пока не встретит '}'
        g.rules[2].nn = NS('N'); g.rules[2].iderror = GRB_ERROR_SERIES + 1;

        // !!! ГЛАВНОЕ: ВЫХОД ПО СКОБКЕ !!!
        g.rules[2].AddChain(1, TS('}'));

        // Переменные
        g.rules[2].AddChain(4, TS('t'), TS('i'), TS(';'), NS('N'));
        g.rules[2].AddChain(4, TS('s'), TS('i'), TS(';'), NS('N'));
        g.rules[2].AddChain(4, TS('c'), TS('i'), TS(';'), NS('N'));

        // Присваивание
        g.rules[2].AddChain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N'));

        // Вызовы
        g.rules[2].AddChain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N'));
        g.rules[2].AddChain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('N'));

        // Cout <
        g.rules[2].AddChain(5, TS('o'), TS('v'), NS('E'), TS(';'), NS('N'));

        // Return
        g.rules[2].AddChain(4, TS('r'), NS('E'), TS(';'), NS('N'));
        g.rules[2].AddChain(3, TS('r'), TS(';'), NS('N'));

        // Switch: switch(...) { N (N съест '}', потом продолжит выполнение)
        // НО! Внутри switch нам нужен новый цикл N.
        // switch(...) { N
        // Внутренний N будет обрабатывать case, default и break, пока не встретит '}'.
        // Потом он выйдет, и мы вернемся во внешний N.
        // НО! Внешний N должен ПРОДОЛЖИТЬСЯ.
        // Поэтому: h ( E ) { N N (Первый N для свича, второй для продолжения функции)
        g.rules[2].AddChain(7, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), NS('N'));

        // !!! CASE и DEFAULT теперь легальны внутри N !!!
        // case L : N
        g.rules[2].AddChain(6, TS('a'), TS('l'), TS(':'), NS('N'));
        // default : N
        g.rules[2].AddChain(5, TS('d'), TS(':'), NS('N'));

        // Print
        g.rules[2].AddChain(4, TS('p'), NS('E'), TS(';'), NS('N'));

        // Пустая ;
        g.rules[2].AddChain(2, TS(';'), NS('N'));

        // 4. Rule K (БОЛЬШЕ НЕ НУЖЕН, НО ОСТАВИМ ЗАГЛУШКУ ЧТОБЫ ИНДЕКСЫ НЕ СБИЛИСЬ)
        g.rules[3].nn = NS('K'); g.rules[3].iderror = GRB_ERROR_SERIES + 6;
        g.rules[3].AddChain(1, TS('}'));

        // 5. Rule E (Expression)
        g.rules[4].nn = NS('E'); g.rules[4].iderror = GRB_ERROR_SERIES + 2;
        g.rules[4].AddChain(3, TS('i'), TS('v'), TS('i'));
        g.rules[4].AddChain(3, TS('i'), TS('v'), TS('l'));
        g.rules[4].AddChain(3, TS('l'), TS('v'), TS('i'));
        g.rules[4].AddChain(3, TS('l'), TS('v'), TS('l'));
        g.rules[4].AddChain(4, TS('i'), TS('('), NS('W'), TS(')'));
        g.rules[4].AddChain(3, TS('i'), TS('('), TS(')'));
        g.rules[4].AddChain(3, TS('('), NS('E'), TS(')'));
        g.rules[4].AddChain(1, TS('i'));
        g.rules[4].AddChain(1, TS('l'));
        g.rules[4].AddChain(1, TS('T'));
        g.rules[4].AddChain(1, TS('F'));
        g.rules[4].AddChain(3, TS('i'), TS('v'), NS('E'));

        // 6. Rule M (Заглушка)
        g.rules[5].nn = NS('M'); g.rules[5].iderror = GRB_ERROR_SERIES + 2;
        g.rules[5].AddChain(1, TS('l'));

        // 7. Rule F (Params)
        g.rules[6].nn = NS('F'); g.rules[6].iderror = GRB_ERROR_SERIES + 3;
        g.rules[6].AddChain(4, TS('t'), TS('i'), TS(','), NS('F'));
        g.rules[6].AddChain(4, TS('s'), TS('i'), TS(','), NS('F'));
        g.rules[6].AddChain(4, TS('c'), TS('i'), TS(','), NS('F'));
        g.rules[6].AddChain(2, TS('t'), TS('i'));
        g.rules[6].AddChain(2, TS('s'), TS('i'));
        g.rules[6].AddChain(2, TS('c'), TS('i'));

        // 8. Rule W (Args)
        g.rules[7].nn = NS('W'); g.rules[7].iderror = GRB_ERROR_SERIES + 4;
        g.rules[7].AddChain(3, TS('i'), TS(','), NS('W'));
        g.rules[7].AddChain(3, TS('l'), TS(','), NS('W'));
        g.rules[7].AddChain(1, TS('i'));
        g.rules[7].AddChain(1, TS('l'));

        return g;
    }
}