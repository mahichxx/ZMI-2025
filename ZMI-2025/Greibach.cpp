#include "stdafx.h"
#include "Greibach.h"
#include "LT.h"
#include <cstring>

namespace GRB {

    // --- Методы Chain и Rule ---
    char* Rule::Chain::getCChain(char* b) {
        for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
        b[size] = 0; return b;
    }

    void Rule::AddChain(const std::vector<GRBALPHABET>& chain) {
        if (size >= GRB_MAX_CHAINS) return;
        chains[size].size = (short)chain.size();
        for (size_t i = 0; i < chain.size(); i++) {
            if (i < GRB_MAX_LEN) chains[size].nt[i] = chain[i];
        }
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
        while (j < size && chains[j].nt[0] != t) j++;
        rc = (j < size ? j : -1);
        if (rc >= 0) pchain = chains[rc];
        return rc;
    }

    short Greibach::getRule(GRBALPHABET pnn, Rule& prule) {
        short rc = -1;
        short k = 0;
        while (k < size && rules[k].nn != pnn) k++;
        if (k < size) { prule = rules[k]; rc = k; }
        return rc;
    }

    Rule Greibach::getRule(short n) { return n < size ? rules[n] : Rule(); }

    // --- ГРАММАТИКА ---
    Greibach getGreibach()
    {
        Greibach g;
        g.startN = NS('S');
        g.stbottomT = TS('$');
        g.size = 8;

        // Rule 0: S (Start -> Functions or Main)
        g.rules[0].nn = NS('S'); g.rules[0].iderror = GRB_ERROR_SERIES + 0;

        // main { N } (Выход из рекурсии)
        g.rules[0].AddChain({ TS('m'), TS('{'), NS('N') });

        // Функции (рекурсия S)
        // type function id ( F ) { N } S
        g.rules[0].AddChain({ TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('t'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('s'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('s'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('c'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('c'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('n'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('n'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });


        // Rule 1: N (Statements - Тело)
        g.rules[1].nn = NS('N'); g.rules[1].iderror = GRB_ERROR_SERIES + 1;

        // Конец блока '}'
        g.rules[1].AddChain({ TS('}') });

        // Объявления: type id; N
        g.rules[1].AddChain({ TS('t'), TS('i'), TS(';'), NS('N') });
        g.rules[1].AddChain({ TS('s'), TS('i'), TS(';'), NS('N') });
        g.rules[1].AddChain({ TS('c'), TS('i'), TS(';'), NS('N') });

        // Присваивание: id = E; N
        g.rules[1].AddChain({ TS('i'), TS('='), NS('E'), TS(';'), NS('N') });

        // Вызов функции: id(W); N
        g.rules[1].AddChain({ TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N') });
        g.rules[1].AddChain({ TS('i'), TS('('), TS(')'), TS(';'), NS('N') });

        // Вывод: cout < E; N
        g.rules[1].AddChain({ TS('o'), TS('v'), NS('E'), TS(';'), NS('N') });

        // Return: return E; N
        g.rules[1].AddChain({ TS('r'), NS('E'), TS(';'), NS('N') });

        // === ВАШ SWITCH ===
        // switch(E) { K } N
        // h - switch, K - цепочка кейсов
        g.rules[1].AddChain({ TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('K'), NS('N') });

        // IF УБРАН, так как по заданию только "выбор из множества" (switch)

        // Пустой оператор ;
        g.rules[1].AddChain({ TS(';'), NS('N') });


        // Rule 2: K (Cases - внутренности switch)
        g.rules[2].nn = NS('K'); g.rules[2].iderror = GRB_ERROR_SERIES + 6;

        // Конец switch (когда кейсы закончились)
        g.rules[2].AddChain({ TS('}') });

        // case lit : { N } K  (a=case, l=literal)
        // Важно: блок { N } внутри кейса позволяет писать много строк кода
        g.rules[2].AddChain({ TS('a'), TS('l'), TS(':'), TS('{'), NS('N'), NS('K') });

        // default : { N } K   (d=default)
        g.rules[2].AddChain({ TS('d'), TS(':'), TS('{'), NS('N'), NS('K') });


        // Rule 3: E (Expressions)
        g.rules[3].nn = NS('E'); g.rules[3].iderror = GRB_ERROR_SERIES + 2;
        // Скобки и операции
        g.rules[3].AddChain({ TS('('), NS('E'), TS(')'), TS('v'), NS('E') });
        g.rules[3].AddChain({ TS('('), NS('E'), TS(')') });
        // Операнд + операция
        g.rules[3].AddChain({ TS('i'), TS('v'), NS('E') });
        g.rules[3].AddChain({ TS('l'), TS('v'), NS('E') });
        // Вызовы функций в выражениях
        g.rules[3].AddChain({ TS('i'), TS('('), NS('W'), TS(')') });
        g.rules[3].AddChain({ TS('i'), TS('('), TS(')') });
        // Одиночные операнды
        g.rules[3].AddChain({ TS('i') });
        g.rules[3].AddChain({ TS('l') });


        // Rule 4: F (Parameters definition)
        g.rules[4].nn = NS('F'); g.rules[4].iderror = GRB_ERROR_SERIES + 3;
        g.rules[4].AddChain({ TS('t'), TS('i'), TS(','), NS('F') });
        g.rules[4].AddChain({ TS('s'), TS('i'), TS(','), NS('F') });
        g.rules[4].AddChain({ TS('c'), TS('i'), TS(','), NS('F') });
        g.rules[4].AddChain({ TS('t'), TS('i') });
        g.rules[4].AddChain({ TS('s'), TS('i') });
        g.rules[4].AddChain({ TS('c'), TS('i') });


        // Rule 5: W (Arguments call)
        g.rules[5].nn = NS('W'); g.rules[5].iderror = GRB_ERROR_SERIES + 4;
        g.rules[5].AddChain({ TS('i'), TS(','), NS('W') });
        g.rules[5].AddChain({ TS('l'), TS(','), NS('W') });
        g.rules[5].AddChain({ TS('i') });
        g.rules[5].AddChain({ TS('l') });

        return g;
    }
}