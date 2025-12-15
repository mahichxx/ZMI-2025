#pragma once
#include <iostream>

namespace FST {
    struct RELATION {
        unsigned char symbol;
        short nnode;

        RELATION(unsigned char c = 0x00, short ns = 0);
    };

    struct NODE {
        short n_relation;
        RELATION* relations;

        NODE();
        NODE(short n, RELATION rel, ...);

        NODE(const NODE& other);           
        NODE& operator=(const NODE& other);  
        ~NODE();                        
    };

    struct FST {
        unsigned char* string;
        short position;
        short nstates;
        NODE* nodes;
        short* rstates;

        FST(unsigned char* s, short ns, NODE n, ...);

        FST(const FST&) = delete;
        FST& operator=(const FST&) = delete;

        ~FST(); 
    };

    bool execute(FST& fst);
};