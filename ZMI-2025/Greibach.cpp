#include "stdafx.h"
#include "Greibach.h"
#include "LT.h"
#include <cstring>

namespace GRB {
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

    Greibach getGreibach()
    {
        Greibach g;
        g.startN = NS('S');
        g.stbottomT = TS('$');
        g.size = 8;

        g.rules[0].nn = NS('S'); g.rules[0].iderror = GRB_ERROR_SERIES + 0;

        g.rules[0].AddChain({ TS('m'), TS('{'), NS('N') });

        g.rules[0].AddChain({ TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('t'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('s'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('s'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('c'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('c'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('n'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), NS('S') });
        g.rules[0].AddChain({ TS('n'), TS('f'), TS('i'), TS('('), TS(')'),        TS('{'), NS('N'), NS('S') });

        g.rules[1].nn = NS('N'); g.rules[1].iderror = GRB_ERROR_SERIES + 1;

        g.rules[1].AddChain({ TS('}') });

        g.rules[1].AddChain({ TS('t'), TS('i'), TS(';'), NS('N') });
        g.rules[1].AddChain({ TS('s'), TS('i'), TS(';'), NS('N') });
        g.rules[1].AddChain({ TS('c'), TS('i'), TS(';'), NS('N') });

        g.rules[1].AddChain({ TS('i'), TS('='), NS('E'), TS(';'), NS('N') });

        g.rules[1].AddChain({ TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N') });
        g.rules[1].AddChain({ TS('i'), TS('('), TS(')'), TS(';'), NS('N') });

        g.rules[1].AddChain({ TS('o'), TS('v'), NS('E'), TS(';'), NS('N') });
  
        g.rules[1].AddChain({ TS('r'), NS('E'), TS(';'), NS('N') });

        g.rules[1].AddChain({ TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('K'), NS('N') });

        g.rules[2].nn = NS('K'); g.rules[2].iderror = GRB_ERROR_SERIES + 6;

        g.rules[2].AddChain({ TS('}') });

        g.rules[2].AddChain({ TS('a'), TS('l'), TS(':'), TS('{'), NS('N'), NS('K') });

        g.rules[2].AddChain({ TS('d'), TS(':'), TS('{'), NS('N'), NS('K') });

        g.rules[3].nn = NS('E'); g.rules[3].iderror = GRB_ERROR_SERIES + 2;

        g.rules[3].AddChain({ TS('('), NS('E'), TS(')'), TS('v'), NS('E') });
        g.rules[3].AddChain({ TS('('), NS('E'), TS(')') });
    
        g.rules[3].AddChain({ TS('i'), TS('v'), NS('E') });
        g.rules[3].AddChain({ TS('l'), TS('v'), NS('E') });
    
        g.rules[3].AddChain({ TS('i'), TS('('), NS('W'), TS(')') });
        g.rules[3].AddChain({ TS('i'), TS('('), TS(')') });

        g.rules[3].AddChain({ TS('i') });
        g.rules[3].AddChain({ TS('l') });

        g.rules[4].nn = NS('F'); g.rules[4].iderror = GRB_ERROR_SERIES + 3;
        g.rules[4].AddChain({ TS('t'), TS('i'), TS(','), NS('F') });
        g.rules[4].AddChain({ TS('s'), TS('i'), TS(','), NS('F') });
        g.rules[4].AddChain({ TS('c'), TS('i'), TS(','), NS('F') });
        g.rules[4].AddChain({ TS('t'), TS('i') });
        g.rules[4].AddChain({ TS('s'), TS('i') });
        g.rules[4].AddChain({ TS('c'), TS('i') });

        g.rules[5].nn = NS('W'); g.rules[5].iderror = GRB_ERROR_SERIES + 4;
        g.rules[5].AddChain({ TS('i'), TS(','), NS('W') });
        g.rules[5].AddChain({ TS('l'), TS(','), NS('W') });
        g.rules[5].AddChain({ TS('i') });
        g.rules[5].AddChain({ TS('l') });

        return g;
    }
}