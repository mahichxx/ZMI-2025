#pragma once
#include "stdafx.h"
#include "Lex.h"

namespace Polish {
	bool StartPolish(Lex::LEX& lex);
	bool PolishNotation(int i, Lex::LEX& lex, char terminator = ';');
};