#pragma once
#include "Error.h"
#include <vector>
#include <iostream>

#define GRB_ERROR_SERIES 600
#define GRB_MAX_CHAINS 32   
#define GRB_MAX_LEN 32      
#define GRB_MAX_RULES 32    

// Определяем тип алфавита
typedef short GRBALPHABET;

// Макросы для удобства (NS - нетерминал, TS - терминал)
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

namespace GRB
{
	struct Rule
	{
		GRBALPHABET nn; // Нетерминал (например, 'S' или 'N')
		int iderror;    // Код ошибки, если правило не сработало
		short size;     // Количество цепочек

		struct Chain
		{
			short size; // Длина цепочки
			GRBALPHABET nt[GRB_MAX_LEN]; // Символы цепочки

			Chain() { size = 0; }

			// Преобразование символов
			static GRBALPHABET T(char t) { return GRBALPHABET(t); }
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }
			static bool isT(GRBALPHABET s) { return s > 0; }
			static bool isN(GRBALPHABET s) { return !isT(s); }
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); }

			char* getCChain(char* b); // Вывод цепочки в строку (для отладки)
		};

		Chain chains[GRB_MAX_CHAINS]; // Массив цепочек

		Rule() { nn = 0x00; size = 0; iderror = -1; }

		// Добавление цепочки через вектор (удобно!)
		void AddChain(const std::vector<GRBALPHABET>& chain);

		char* getCRule(char* b, short nchain);
		short getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j);
	};

	struct Greibach
	{
		short size;
		GRBALPHABET startN;    // Стартовый символ
		GRBALPHABET stbottomT; // Дно стека

		Rule rules[GRB_MAX_RULES];

		Greibach() { size = 0; startN = 0; stbottomT = 0; }

		short getRule(GRBALPHABET pnn, Rule& prule);
		Rule getRule(short n);
	};

	Greibach getGreibach(); // Главная функция получения грамматики
};