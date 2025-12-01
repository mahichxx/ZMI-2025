#include "stdafx.h"

int wmain(int argc, wchar_t* argv[]) {
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		log.errors_cout = 0;
		if (parm.more)
			Log::WriteLine(&cout, "Тест: ", "без ошибок ", "");
		Log::WriteLog(log.stream);
		if (parm.more)
			Log::WriteLog(&cout);

		Log::WriteParm(log.stream, parm);
		if (parm.more)
			Log::WriteParm(&cout, parm);

		In::IN in = In::getin(parm.in);
		Log::WriteIn(log.stream, in);
		if (parm.more)
			Log::WriteIn(&cout, in);

		Lex::LEX lex = Lex::lexAnaliz(log, in);
		cout << "-----Лексический анализ завершился\n\n";

		fstream fout;
		fout.open(parm.out, ios_base::app);
		if (!fout.is_open())
			throw ERROR_THROW(110);
		LT::showTable(lex.lextable, &fout);
		if (parm.more || parm.Lout)
			LT::showTable(lex.lextable, &cout);

		if (parm.more || parm.lt)
			LT::writeLexTable(&cout, lex.lextable);

		IT::showITable(lex.idtable, log.stream);
		if (parm.more || parm.it)
			IT::showITable(lex.idtable, &cout);

		MFST::Mfst mfst(lex.lextable, GRB::getGreibach());
		mfst.log = log;
		if (parm.more || parm.lenta) mfst.more = true;
		if (mfst.start()) {
			cout << "-----Синтаксический выполнен без ошибок\n\n";
		}
		else {
			cout << "-----Найдены ошибки в коде\n\n";
			return 0;
		}
		mfst.savededucation();
		mfst.printrules();
		if (Sem::SemAnaliz(lex.lextable, lex.idtable, log)) cout << "-----Семантичeский анализ выполнен без ошибок\n\n";
		else {
			Log::WriteLine(log.stream, "-----Семантический анализ обнаружил ошибку(и)\n", "");
			cout << "-----Семантический анализ обнаружил ошибку(и)\n\n-----Выполнение программы остановлено\n\n";
			return 0;
		}

		bool polish_ok = Polish::StartPolish(lex);
		if (!polish_ok)
		{
			Log::WriteLine(log.stream, "-----Ошибка при попытке преобразования выражения\n", "");
			cout << "-----Ошибка при попытке преобразования выражения\n\n-----Выполнение программы остановлено\n\n";
			return 0;
		}
		else cout << "-----Преобразование выражений завершено без ошибок\n\n";

		fout << "\n-----Преобразованная таблица лексем\n";
		LT::showTable(lex.lextable, &fout);
		if (parm.more || parm.Lout) {
			cout << "\n-----Преобразованная таблица лексем\n";
			LT::showTable(lex.lextable, &cout);
		}
		Gener::CodeGeneration(lex, parm, log);
		Log::Close(log);
		if (log.errors_cout > 0) throw Error::geterror(0);

		return 0;
	}
	catch (Error::ERROR e)
	{
		cout << "\n-----Выполнение программы остановлено\n\n";
		return 0;
	}
}