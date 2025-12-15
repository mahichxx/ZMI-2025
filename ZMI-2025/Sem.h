#pragma once
#include "stdafx.h"
#include "Log.h"

// Forward declarations
namespace LT { struct LexTable; }
namespace IT { struct IdTable; }

namespace Sem
{
	// Важно: Log::LOG& log (добавлен амперсанд)
	bool SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG& log);
}