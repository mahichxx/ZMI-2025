#pragma once
#include "stdafx.h"
#include "Log.h"

// ѕредварительное объ€вление (Forward declaration)
namespace LT { struct LexTable; }
namespace IT { struct IdTable; }

namespace Sem
{
	// ¬ј∆Ќќ: «десь ќЅя«ј“≈Ћ№Ќќ должны быть значки '&' (амперсанды)
	bool SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log);
}