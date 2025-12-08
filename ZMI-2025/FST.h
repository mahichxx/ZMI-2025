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

        // Правило трех:
        NODE(const NODE& other);             // Конструктор копирования
        NODE& operator=(const NODE& other);  // Оператор присваивания
        ~NODE();                             // Деструктор
    };

    struct FST {
        unsigned char* string;
        short position;
        short nstates;
        NODE* nodes;
        short* rstates;

        FST(unsigned char* s, short ns, NODE n, ...);

        // FST тяжелый объект, запретим случайное копирование
        FST(const FST&) = delete;
        FST& operator=(const FST&) = delete;

        ~FST(); // Обязательный деструктор
    };

    bool execute(FST& fst);
};