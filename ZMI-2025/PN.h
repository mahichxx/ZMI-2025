#pragma once
#include "stdafx.h"
#include "Lex.h" // Нужно знать структуру LEX

namespace Polish {
	bool StartPolish(Lex::LEX& lex);
	// Добавляем третий параметр terminator с дефолтным значением
	bool PolishNotation(int i, Lex::LEX& lex, char terminator = LEX_SEMICOLON);
};