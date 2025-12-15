#include "stdafx.h"
#include "FST.h"
#include <cstdarg>
#include <algorithm> 

namespace FST {
    RELATION::RELATION(unsigned char c, short ns)
        : symbol(c), nnode(ns)
    {}

    NODE::NODE()
        : n_relation(0), relations(nullptr)
    {}

    NODE::NODE(short n, RELATION rel, ...)
        : n_relation(n)
    {
        relations = new RELATION[n];
        relations[0] = rel;

        va_list args;
        va_start(args, rel);
        for (short i = 1; i < n; ++i) {
            relations[i] = va_arg(args, RELATION);
        }
        va_end(args);
    }

    NODE::NODE(const NODE& other)
        : n_relation(other.n_relation)
    {
        if (n_relation > 0) {
            relations = new RELATION[n_relation];
            std::copy(other.relations, other.relations + n_relation, relations);
        }
        else {
            relations = nullptr;
        }
    }

    NODE& NODE::operator=(const NODE& other) {
        if (this == &other) return *this;

        delete[] relations;

        n_relation = other.n_relation;
        if (n_relation > 0) {
            relations = new RELATION[n_relation];
            std::copy(other.relations, other.relations + n_relation, relations);
        }
        else {
            relations = nullptr;
        }
        return *this;
    }

    NODE::~NODE() {
        delete[] relations;
    }
    
    FST::FST(unsigned char* s, short ns, NODE n, ...)
        : string(s), position(-1), nstates(ns)
    {
        nodes = new NODE[ns];
        nodes[0] = n;

        va_list args;
        va_start(args, n);
        for (int i = 1; i < ns; ++i) {
            nodes[i] = va_arg(args, NODE);
        }
        va_end(args);

        rstates = new short[nstates];
        for (int i = 0; i < nstates; i++) rstates[i] = -1;
        rstates[0] = 0;
    }

    FST::~FST() {
        delete[] nodes;
        delete[] rstates;
    }

    bool step(FST& fst, short*& rstates) {
        bool rc = false;

        std::swap(rstates, fst.rstates);

        for (int k = 0; k < fst.nstates; k++) fst.rstates[k] = -1;

        for (short i = 0; i < fst.nstates; ++i) {
            if (rstates[i] == fst.position) {
                for (short j = 0; j < fst.nodes[i].n_relation; ++j) {
                    if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {
                        fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
                        rc = true;
                    }
                }
            }
        }
        return rc;
    }

    bool execute(FST& fst) {
        short* rstates = new short[fst.nstates];

        for (int i = 0; i < fst.nstates; i++) rstates[i] = -1;

        short lstring = (short)strlen((const char*)fst.string);
        bool rc = true;

        fst.position = -1;
        for (int i = 0; i < fst.nstates; i++) fst.rstates[i] = -1;
        fst.rstates[0] = 0;

        for (short i = 0; i < lstring && rc; ++i) {
            fst.position++;
            rc = step(fst, rstates);
        }

        delete[] rstates;

        return rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc;
    }

} 