#pragma once
#include "stdafx.h"
#include "Greibach.h"
#include "LT.h"
#include "Log.h"
#include <stack>
#include <iomanip>
#include <iostream>

using namespace std;

class my_stack_SHORT : public std::stack<short> {
public:
	using std::stack<short>::c;
};

typedef my_stack_SHORT MFSTSTSTACK;

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

// --- МАКРОСЫ ДЛЯ КОНСОЛИ (БЕЗОПАСНЫЕ ЗАГЛУШКИ) ---
// ((void)0) - это "пустая команда", которая не ломает if/else
#define MFST_TRACE_START_0_M	((void)0)
#define MFST_TRACE_START_M		((void)0)
#define MFST_TRACE1_M			((void)0)
#define MFST_TRACE2_M			((void)0)
#define MFST_TRACE3_M			((void)0)
#define MFST_TRACE4_M(c)		((void)0)
#define MFST_TRACE5_M(c)		((void)0)
#define MFST_TRACE6_M(c, k)		((void)0)
#define MFST_TRACE7_M			((void)0)

// --- МАКРОСЫ ДЛЯ ЛОГА (ОСТАЮТСЯ КАК ЕСТЬ) ---
#define MFST_TRACE_START_0	*log.stream << "\n------------------Синтаксический анализ------------------\n\n";
#define MFST_TRACE_START	*log.stream << setw(5) << setfill(' ') << left << "Шаг" << ":"\
							<< setw(30) << left << "Правило"\
							<< setw(30) << left << "Входная лента"\
							<< setw(20) << left << "Стек"\
							<< endl;

#define MFST_TRACE1			*log.stream << setw(4) << left << ++FST_TRACE_n << ": " \
							<< setw(30) << left << rule.getCRule(rbuf, nrulechain)\
							<< setw(30) << left << getCLenta(lbuf, lenta_position)\
							<< setw(20) << left << getCSt(sbuf)\
							<< endl;

#define MFST_TRACE2			*log.stream << setw(4) << left << FST_TRACE_n << ": "\
							<< setw(30) << left << " "\
							<< setw(30) << left << getCLenta(lbuf, lenta_position)\
							<< setw(20) << left << getCSt(sbuf)\
							<< endl;

#define MFST_TRACE3			*log.stream << setw(4) << left << ++FST_TRACE_n << ": "\
							<< setw(30) << left << " "\
							<< setw(30) << left << getCLenta(lbuf, lenta_position)\
							<< setw(20) << left << getCSt(sbuf)\
							<< endl;

#define MFST_TRACE4(c)		*log.stream << setw(4) << left << ++FST_TRACE_n << ": " << setw(20) << left << c << endl;
#define MFST_TRACE5(c)		*log.stream << setw(4) << left << FST_TRACE_n << ": " << setw(20) << left << c << endl;
#define MFST_TRACE6(c, k)	*log.stream << setw(4) << left << FST_TRACE_n << ": " << setw(20) << left << c << k << endl;
#define MFST_TRACE7			*log.stream << setw(4) << left << state.lenta_position << ": "\
							<< setw(20) << left << rule.getCRule(rbuf, state.nrulechain)\
							<< endl;

namespace MFST {
	struct MfstState {
		short lenta_position;
		short nrule;
		short nrulechain;
		MFSTSTSTACK st;
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
	};

	struct Mfst {
		enum RC_STEP {
			NS_OK, NS_NORULE, NS_NORULECHAIN, NS_ERROR, TS_OK, TS_NOK, LENTA_END, SURPRISE,
		};

		struct MfstDiagnosis {
			short lenta_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];

		class my_stack_MfstState : public std::stack<MfstState> {
		public:
			using std::stack<MfstState>::c;
		};

		GRBALPHABET* lenta;
		short lenta_position;
		short nrule;
		short nrulechain;
		short lenta_size;
		GRB::Greibach greibach;
		LT::LexTable lex;
		Log::LOG log;
		bool more;
		MFSTSTSTACK st;
		my_stack_MfstState storestate;

		Mfst();
		Mfst(LT::LexTable plex, GRB::Greibach pgreibach, Log::LOG plog);

		char* getCSt(char* buf);
		char* getCLenta(char* buf, short pos, short n = 25);
		char* getDiagnosis(short n, char* buf);
		bool savestate();
		bool resetstate();
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step();
		bool start();
		bool saveddiagnosis(RC_STEP pprc_step);
		void printrules();
		bool savededucation();

		struct Deducation {
			short size;
			short* nrules;
			short* nrulechains;
			Deducation() { size = 0; nrules = 0; nrulechains = 0; }
		} deducation;
	};
}