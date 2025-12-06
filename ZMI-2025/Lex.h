#pragma once
#include "stdafx.h"
#include "Log.h"
#include "In.h"
#include "LT.h"  // <--- ВАЖНО: Подключаем Таблицу Лексем
#include "IT.h"  // <--- ВАЖНО: Подключаем Таблицу Идентификаторов

namespace Lex {

	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
	};

	LEX lexAnaliz(Log::LOG log, In::IN in);
}