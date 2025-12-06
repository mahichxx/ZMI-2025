#pragma once
#include "stdafx.h"
#include "Log.h"
#include "LT.h" // <--- ÂÀÆÍÎ!
#include "IT.h" // <--- ÂÀÆÍÎ!

namespace Sem
{
	bool SemAnaliz(LT::LexTable lextable, IT::IdTable idtable, Log::LOG log);
}