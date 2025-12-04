#include "stdafx.h"
#include "Greibach.h"
#include <vector> 

namespace GRB {

    // Этот конструктор больше НЕ ИСПОЛЬЗУЕТСЯ для инициализации данных
    // Мы используем его только как заглушку, чтобы старый код компилировался
    Rule::Chain::Chain(short psize, GRBALPHABET s, ...) {
        size = 0; // Игнорируем аргументы, всё равно они передаются криво
    }

    char* Rule::Chain::getCChain(char* b) {
        for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
        b[size] = 0; return b;
    }

    // --- Rule ---
    Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) {
        nn = pnn; iderror = piderror; size = 0;
    }

    // !!! НОВЫЙ БЕЗОПАСНЫЙ МЕТОД !!!
    // Добавляет цепочку, принимая массив символов, а не variadic arguments
    void Rule::AddChain(short psize, GRBALPHABET* signals)
    {
        if (size >= GRB_MAX_CHAINS) return;
        chains[size].size = psize;
        for (short i = 0; i < psize; i++) {
            chains[size].nt[i] = signals[i];
        }
        size++;
    }

    // Старый метод (для совместимости, но лучше не использовать)
    void Rule::AddChain(short psize, GRBALPHABET s, ...)
    {
        // Не используем, так как он вызывает баги
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


    // --- ГРАММАТИКА (БЕЗОПАСНАЯ СБОРКА) ---
    Greibach getGreibach()
    {
        Greibach g;
        g.startN = NS('S');
        g.stbottomT = TS('$');
        g.size = 8;

        // Вспомогательный макрос для создания массива (чтобы код был короче)
#define CHAIN(...) { __VA_ARGS__ }

// 1. Rule S
        g.rules[0].nn = NS('S'); g.rules[0].iderror = GRB_ERROR_SERIES + 0;

        GRBALPHABET c1[] = CHAIN(TS('m'), TS('{'), NS('N'));
        g.rules[0].AddChain(3, c1);

        GRBALPHABET c2[] = CHAIN(TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, c2);

        GRBALPHABET c3[] = CHAIN(TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(8, c3);

        GRBALPHABET c4[] = CHAIN(TS('s'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, c4);

        GRBALPHABET c5[] = CHAIN(TS('s'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(8, c5);

        GRBALPHABET c6[] = CHAIN(TS('c'), TS('f'), TS('i'), TS('('), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(7, c6);

        GRBALPHABET c7[] = CHAIN(TS('c'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), NS('T'), NS('S'));
        g.rules[0].AddChain(8, c7);

        GRBALPHABET c8[] = CHAIN(TS('m'), TS('{'), NS('N'), NS('S'));
        g.rules[0].AddChain(4, c8);

        // 2. Rule T
        g.rules[1].nn = NS('T'); g.rules[1].iderror = GRB_ERROR_SERIES + 0;
        GRBALPHABET t1[] = CHAIN(TS('{'), NS('N'));
        g.rules[1].AddChain(2, t1);

        // 3. Rule N
        g.rules[2].nn = NS('N'); g.rules[2].iderror = GRB_ERROR_SERIES + 1;
        GRBALPHABET n1[] = CHAIN(TS('}')); g.rules[2].AddChain(1, n1);

        GRBALPHABET n2[] = CHAIN(TS('t'), TS('i'), TS(';'), NS('N')); g.rules[2].AddChain(4, n2);
        GRBALPHABET n3[] = CHAIN(TS('s'), TS('i'), TS(';'), NS('N')); g.rules[2].AddChain(4, n3);
        GRBALPHABET n4[] = CHAIN(TS('c'), TS('i'), TS(';'), NS('N')); g.rules[2].AddChain(4, n4);

        GRBALPHABET n5[] = CHAIN(TS('i'), TS('='), NS('E'), TS(';'), NS('N')); g.rules[2].AddChain(5, n5);

        GRBALPHABET n6[] = CHAIN(TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')); g.rules[2].AddChain(6, n6);
        GRBALPHABET n7[] = CHAIN(TS('i'), TS('('), TS(')'), TS(';'), NS('N')); g.rules[2].AddChain(5, n7);

        GRBALPHABET n8[] = CHAIN(TS('o'), TS('v'), NS('E'), TS(';'), NS('N')); g.rules[2].AddChain(5, n8);

        GRBALPHABET n9[] = CHAIN(TS('r'), NS('E'), TS(';'), NS('N')); g.rules[2].AddChain(4, n9);
        GRBALPHABET n10[] = CHAIN(TS('r'), TS(';'), NS('N')); g.rules[2].AddChain(3, n10);

        GRBALPHABET n11[] = CHAIN(TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('K'), NS('N')); g.rules[2].AddChain(7, n11);

        GRBALPHABET n12[] = CHAIN(TS('p'), NS('E'), TS(';'), NS('N')); g.rules[2].AddChain(4, n12);

        GRBALPHABET n13[] = CHAIN(TS(';'), NS('N')); g.rules[2].AddChain(2, n13);

        // 4. Rule K
        g.rules[3].nn = NS('K'); g.rules[3].iderror = GRB_ERROR_SERIES + 6;
        GRBALPHABET k1[] = CHAIN(TS('}')); g.rules[3].AddChain(1, k1);

        GRBALPHABET k2[] = CHAIN(TS('a'), TS('l'), TS(':'), NS('M'), NS('K')); g.rules[3].AddChain(6, k2);
        GRBALPHABET k3[] = CHAIN(TS('d'), TS(':'), NS('M'), NS('K')); g.rules[3].AddChain(5, k3);
        GRBALPHABET k4[] = CHAIN(TS('}'), NS('N')); g.rules[3].AddChain(2, k4);

        // 5. Rule E
        g.rules[4].nn = NS('E'); g.rules[4].iderror = GRB_ERROR_SERIES + 2;
        GRBALPHABET e1[] = CHAIN(TS('i'), TS('v'), TS('i')); g.rules[4].AddChain(3, e1);
        GRBALPHABET e2[] = CHAIN(TS('i'), TS('v'), TS('l')); g.rules[4].AddChain(3, e2);
        GRBALPHABET e3[] = CHAIN(TS('l'), TS('v'), TS('i')); g.rules[4].AddChain(3, e3);
        GRBALPHABET e4[] = CHAIN(TS('l'), TS('v'), TS('l')); g.rules[4].AddChain(3, e4);
        GRBALPHABET e5[] = CHAIN(TS('i'), TS('('), NS('W'), TS(')')); g.rules[4].AddChain(4, e5);
        GRBALPHABET e6[] = CHAIN(TS('i'), TS('('), TS(')')); g.rules[4].AddChain(3, e6);
        GRBALPHABET e7[] = CHAIN(TS('('), NS('E'), TS(')')); g.rules[4].AddChain(3, e7);
        GRBALPHABET e8[] = CHAIN(TS('i')); g.rules[4].AddChain(1, e8);
        GRBALPHABET e9[] = CHAIN(TS('l')); g.rules[4].AddChain(1, e9);
        GRBALPHABET e10[] = CHAIN(TS('T')); g.rules[4].AddChain(1, e10);
        GRBALPHABET e11[] = CHAIN(TS('F')); g.rules[4].AddChain(1, e11);
        GRBALPHABET e12[] = CHAIN(TS('i'), TS('v'), NS('E')); g.rules[4].AddChain(3, e12);

        // 6. Rule M
        g.rules[5].nn = NS('M'); g.rules[5].iderror = GRB_ERROR_SERIES + 1;
        GRBALPHABET m1[] = CHAIN(TS('t'), TS('i'), TS(';')); g.rules[5].AddChain(3, m1);
        GRBALPHABET m2[] = CHAIN(TS('s'), TS('i'), TS(';')); g.rules[5].AddChain(3, m2);
        GRBALPHABET m3[] = CHAIN(TS('c'), TS('i'), TS(';')); g.rules[5].AddChain(3, m3);
        GRBALPHABET m4[] = CHAIN(TS('i'), TS('='), NS('E'), TS(';')); g.rules[5].AddChain(4, m4);
        GRBALPHABET m5[] = CHAIN(TS('i'), TS('('), NS('W'), TS(')'), TS(';')); g.rules[5].AddChain(5, m5);
        GRBALPHABET m6[] = CHAIN(TS('i'), TS('('), TS(')'), TS(';')); g.rules[5].AddChain(4, m6);
        GRBALPHABET m7[] = CHAIN(TS('o'), TS('v'), NS('E'), TS(';')); g.rules[5].AddChain(4, m7);
        GRBALPHABET m8[] = CHAIN(TS('r'), NS('E'), TS(';')); g.rules[5].AddChain(3, m8);
        GRBALPHABET m9[] = CHAIN(TS('r'), TS(';')); g.rules[5].AddChain(2, m9);
        GRBALPHABET m100[] = CHAIN(TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('K')); g.rules[5].AddChain(6, m100);
        GRBALPHABET m110[] = CHAIN(TS('p'), NS('E'), TS(';')); g.rules[5].AddChain(3, m110);
        GRBALPHABET m120[] = CHAIN(TS(';')); g.rules[5].AddChain(1, m120);

        // 7. Rule F
        g.rules[6].nn = NS('F'); g.rules[6].iderror = GRB_ERROR_SERIES + 3;
        GRBALPHABET f1[] = CHAIN(TS('t'), TS('i'), TS(','), NS('F')); g.rules[6].AddChain(4, f1);
        GRBALPHABET f2[] = CHAIN(TS('s'), TS('i'), TS(','), NS('F')); g.rules[6].AddChain(4, f2);
        GRBALPHABET f3[] = CHAIN(TS('c'), TS('i'), TS(','), NS('F')); g.rules[6].AddChain(4, f3);
        GRBALPHABET f4[] = CHAIN(TS('t'), TS('i')); g.rules[6].AddChain(2, f4);
        GRBALPHABET f5[] = CHAIN(TS('s'), TS('i')); g.rules[6].AddChain(2, f5);
        GRBALPHABET f6[] = CHAIN(TS('c'), TS('i')); g.rules[6].AddChain(2, f6);

        // 8. Rule W
        g.rules[7].nn = NS('W'); g.rules[7].iderror = GRB_ERROR_SERIES + 4;
        GRBALPHABET w1[] = CHAIN(TS('i'), TS(','), NS('W')); g.rules[7].AddChain(3, w1);
        GRBALPHABET w2[] = CHAIN(TS('l'), TS(','), NS('W')); g.rules[7].AddChain(3, w2);
        GRBALPHABET w3[] = CHAIN(TS('i')); g.rules[7].AddChain(1, w3);
        GRBALPHABET w4[] = CHAIN(TS('l')); g.rules[7].AddChain(1, w4);

        return g;
    }
}