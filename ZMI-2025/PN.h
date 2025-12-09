#pragma once
#include "stdafx.h"
#include "Lex.h"

namespace Polish {
	bool StartPolish(Lex::LEX& lex);

	// !!! ИЗМЕНЕНИЕ: Используем ';' вместо LEX_SEMICOLON !!!
	// Это позволяет не подключать LT.h сюда и избежать ошибок кругового включения.
	bool PolishNotation(int i, Lex::LEX& lex, char terminator = ';');
};