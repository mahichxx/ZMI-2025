#pragma once
#include "stdafx.h"
#include "Lex.h"
#include "Parm.h"
#include "Log.h"

namespace Gener
{
	void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log);
};