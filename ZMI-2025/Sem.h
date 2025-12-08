#pragma once
#include "stdafx.h"
#include "Log.h"
#include "Lex.h" // ВАЖНО: Подключаем Lex.h, чтобы видеть определения таблиц

namespace Sem
{
	bool SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log);
}