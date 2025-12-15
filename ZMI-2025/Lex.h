#pragma once
#include "stdafx.h"
#include "Log.h"
#include "In.h"
#include "LT.h" 
#include "IT.h" 

namespace Lex {

	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
	};

	LEX lexAnaliz(Log::LOG log, In::IN in);
}