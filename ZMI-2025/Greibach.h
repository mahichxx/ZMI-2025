#pragma once
#include "Error.h"
#include <iostream>
#include <cstring> // Для strcpy/strcat

#define GRB_ERROR_SERIES 600
#define GRB_MAX_CHAINS 32   
#define GRB_MAX_LEN 32      
#define GRB_MAX_RULES 32    

typedef short GRBALPHABET;

// Макросы оставим, они удобные
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

namespace GRB
{
	struct Rule
	{
		GRBALPHABET nn;
		int iderror;
		short size; // Количество цепочек

		struct Chain
		{
			short size; // Длина цепочки
			GRBALPHABET nt[GRB_MAX_LEN];

			Chain() { size = 0; }

			// Конструктор через va_list оставим, если он используется внутри AddChain
			// Но лучше сделать явную инициализацию

			static GRBALPHABET T(char t) { return GRBALPHABET(t); }
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }
			static bool isT(GRBALPHABET s) { return s > 0; }
			static bool isN(GRBALPHABET s) { return !isT(s); }
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); }

			char* getCChain(char* b);
		};

		Chain chains[GRB_MAX_CHAINS];

		Rule() { nn = 0x00; size = 0; iderror = -1; }

		// Этот конструктор опасен, уберем его использование в getGreibach
		// Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...);

		// Используем этот метод для добавления цепочек
		void AddChain(short psize, GRBALPHABET s, ...);

		char* getCRule(char* b, short nchain);
		short getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j);
	};

	struct Greibach
	{
		short size; // Количество правил
		GRBALPHABET startN;
		GRBALPHABET stbottomT;

		Rule rules[GRB_MAX_RULES];

		Greibach() { size = 0; startN = 0; stbottomT = 0; }

		short getRule(GRBALPHABET pnn, Rule& prule);
		Rule getRule(short n);
	};

	Greibach getGreibach();
};