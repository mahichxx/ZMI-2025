#pragma once
#include "Error.h"
#include "Log.h"
#include <iostream>

#define GRB_ERROR_SERIES 600
#define GRB_MAX_CHAINS 32   
#define GRB_MAX_LEN 32      
#define GRB_MAX_RULES 32    

typedef short GRBALPHABET;

#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

namespace GRB
{
	struct Rule
	{
		GRBALPHABET nn;
		int iderror;
		short size;

		struct Chain
		{
			short size;
			GRBALPHABET nt[GRB_MAX_LEN];

			Chain() { size = 0; }
			// Заглушка, чтобы не ломать старый код (но мы его не юзаем)
			Chain(short psize, GRBALPHABET s, ...);

			char* getCChain(char* b);
			static GRBALPHABET T(char t) { return GRBALPHABET(t); }
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }
			static bool isT(GRBALPHABET s) { return s > 0; }
			static bool isN(GRBALPHABET s) { return !isT(s); }
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); }

			~Chain() {};
		};

		Chain chains[GRB_MAX_CHAINS];

		Rule() { nn = 0x00; size = 0; }
		Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...);

		// !!! НОВЫЙ МЕТОД: БЕЗ МНОГОТОЧИЯ !!!
		void AddChain(short psize, GRBALPHABET* signals);

		// Старый метод (заглушка)
		void AddChain(short psize, GRBALPHABET s, ...);

		char* getCRule(char* b, short nchain);
		short getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j);

		~Rule() {};
	};

	struct Greibach
	{
		short size;
		GRBALPHABET startN;
		GRBALPHABET stbottomT;

		Rule rules[GRB_MAX_RULES];

		Greibach() { size = 0; startN = 0; stbottomT = 0; }
		Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...);

		short getRule(GRBALPHABET pnn, Rule& prule);
		Rule getRule(short n);

		~Greibach() {};
	};
	Greibach getGreibach();
};