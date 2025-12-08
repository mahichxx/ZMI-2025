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

using namespace std;

int wmain(int argc, wchar_t* argv[]) {
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		// 1. Получение параметров
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		log.errors_cout = 0;

		if (parm.more) Log::WriteLine(&cout, "Тест: ", "без ошибок ", "");
		Log::WriteLog(log.stream);
		if (parm.more) Log::WriteLog(&cout);

		Log::WriteParm(log.stream, parm);
		if (parm.more) Log::WriteParm(&cout, parm);

		// 2. Ввод
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log.stream, in);
		if (parm.more) Log::WriteIn(&cout, in);

		// 3. Лексический анализ
		Lex::LEX lex = Lex::lexAnaliz(log, in);
		cout << "-----Лексический анализ завершился\n\n";

		// Вывод таблиц
		fstream fout;
		fout.open(parm.out, ios_base::out | ios_base::trunc);
		if (!fout.is_open()) throw ERROR_THROW(110);

		LT::showTable(lex.lextable, &fout);
		if (parm.more || parm.lt) LT::writeLexTable(&cout, lex.lextable);
		IT::showITable(lex.idtable, log.stream);
		if (parm.more || parm.it) IT::showITable(lex.idtable, &cout);

		// 4. Синтаксический анализ
		MFST::Mfst mfst(lex.lextable, GRB::getGreibach());
		mfst.log = log;
		if (parm.more || parm.lenta) mfst.more = true;

		// Запуск парсера (он пишет протокол шагов в лог)
		bool syntax_ok = mfst.start();

		// Сохраняем результат построения дерева
		mfst.savededucation();

		// !!! ВОТ ЗДЕСЬ ДОБАВЛЯЕМ КРАСИВУЮ ЛИНИЮ В ЛОГ ФАЙЛ !!!
		if (log.stream) {
			*log.stream << "\n------------------Дерево разбора------------------\n";
		}

		// Выводим само дерево (список правил)
		mfst.printrules();

		if (syntax_ok) {
			cout << "\n-----Синтаксический анализ выполнен без ошибок\n\n";
		}
		else {
			cout << "-----Синтаксический анализ завершен (с предупреждениями, продолжаем...)\n\n";
		}

		// 5. Семантический анализ
		if (Sem::SemAnaliz(lex.lextable, lex.idtable, log)) {
			cout << "-----Семантический анализ выполнен без ошибок\n\n";
		}
		else {
			Log::WriteLine(log.stream, "-----Семантический анализ обнаружил ошибку(и)\n", "");
			cout << "-----Семантический анализ обнаружил ошибку(и)\n\n";
			Log::Close(log);
			return 0;
		}

		// 6. Польская запись
		bool polish_ok = Polish::StartPolish(lex);
		if (!polish_ok)
		{
			Log::WriteLine(log.stream, "-----Ошибка при построении Польской записи\n", "");
			cout << "-----Ошибка при построении Польской записи\n\n";
			Log::Close(log);
			return 0;
		}
		else {
			cout << "-----Преобразование выражений (ПОЛИЗ) завершено без ошибок\n\n";
		}

		// Запись ПОЛИЗ
		fout << "\n\n-----Таблица лексем после преобразования в ПОЛИЗ (Сырая)\n";
		LT::showTable(lex.lextable, &fout);

		LT::ShowPolishRaw(lex.lextable, lex.idtable, &fout);
		if (parm.more || parm.Lout) {
			LT::ShowPolishRaw(lex.lextable, lex.idtable, &cout);
		}

		// 7. Генерация кода
		Gener::CodeGeneration(lex, parm, log);

		Log::Close(log);
		if (log.errors_cout > 0) throw Error::geterror(0);

		fout.close();
		return 0;
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		cout << "\n-----Выполнение программы остановлено из-за ошибки\n\n";
		return 0;
	}
}