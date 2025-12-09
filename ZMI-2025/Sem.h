#pragma once
#include "stdafx.h"
#include "Log.h"

// !!! FORWARD DECLARATION !!!
// Мы говорим компилятору: "Где-то есть эти структуры",
// но не подключаем тяжелые файлы заголовков сюда.
namespace LT { struct LexTable; }
namespace IT { struct IdTable; }

namespace Sem
{
	// Так как мы передаем по ссылке (&), компилятору не нужен полный размер структур
	bool SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log);
}