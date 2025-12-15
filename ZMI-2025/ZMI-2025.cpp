#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Lex.h"
#include "IT.h"
#include "LT.h"
#include "MFST.h"
#include "PN.h"
#include "Sem.h"
#include "Generation.h"
#include <fstream>
#include <iostream>

using namespace std;

int wmain(int argc, wchar_t* argv[]) {
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		log.errors_cout = 0; 

		Log::WriteLog(log.stream);
		Log::WriteParm(log.stream, parm);

		In::IN in = In::getin(parm.in);
		Log::WriteIn(log.stream, in);

		Lex::LEX lex = Lex::lexAnaliz(log, in);

		fstream fout;
		fout.open(parm.out, ios_base::out | ios_base::trunc);
		if (fout.is_open()) {
			LT::showTable(lex.lextable, &fout);
			IT::showITable(lex.idtable, log.stream);
		}

		cout << "- Лексический анализ выполнен без ошибок\n\n";
		MFST::Mfst mfst(lex.lextable, GRB::getGreibach(), log);
		if (parm.lenta) mfst.more = true;

		bool syntax_ok = mfst.start();
		mfst.savededucation();
		mfst.printrules();

		if (!syntax_ok) {
			cout << "\n- Синтаксический анализ обнаружил ошибку(и)\n";
			Log::WriteLine(log.stream, "\n- Синтаксический анализ обнаружил ошибку(и)\n", "");
			return 0;
		}
		cout << "\n- Синтаксический анализ выполнен без ошибок\n";

		if (!Sem::SemAnaliz(lex.lextable, lex.idtable, log)) {
			cout << "\n- Семантический анализ обнаружил ошибку(и)\n";
			Log::WriteLine(log.stream, "\n- Семантический анализ обнаружил ошибку(и)\n", "");
			return 0;
		}
		cout << "\n- Семантический анализ выполнен без ошибок\n";

		if (!Polish::StartPolish(lex)) {
			cout << "\n- Ошибка при построении Польской записи\n";
			return 0;
		}

		if (fout.is_open()) {
			LT::ShowPolishRaw(lex.lextable, lex.idtable, &fout);
			fout.close();
		}
		cout << "\n- Преобразование выражений (ПОЛИЗ) завершено\n\n";

		Gener::CodeGeneration(lex, parm, log);
		In::Delete(in);
		LT::Delete(lex.lextable);
		IT::Delete(lex.idtable);
		Log::Close(log);

		cout << "\nКомпиляция завершилась\n";
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		cout << "\n- Выполнение программы остановлено из-за ошибки\n";
	}
	return 0;
}