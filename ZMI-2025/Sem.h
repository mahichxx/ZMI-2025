#pragma once
#include "stdafx.h"
#include "Log.h"

namespace LT { struct LexTable; }
namespace IT { struct IdTable; }

namespace Sem
{
	bool SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG& log);
}