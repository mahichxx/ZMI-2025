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
		// 1. Параметры
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		log.errors_cout = 0; // Сбрасываем счетчик ошибок вывода

		Log::WriteLog(log.stream);
		Log::WriteParm(log.stream, parm);

		// 2. Ввод (In)
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log.stream, in);

		// 3. Лексический анализ (Lex)
		// Если здесь будет ошибка (например, нет box), вылетит исключение и мы уйдем в catch
		Lex::LEX lex = Lex::lexAnaliz(log, in);

		// Вывод таблиц (для отладки)
		fstream fout;
		fout.open(parm.out, ios_base::out | ios_base::trunc);
		if (fout.is_open()) {
			LT::showTable(lex.lextable, &fout);
			IT::showITable(lex.idtable, log.stream);
		}

		cout << "- Лексический анализ выполнен без ошибок\n\n";

		// 4. Синтаксический анализ (MFST)
		MFST::Mfst mfst(lex.lextable, GRB::getGreibach(), log);
		if (parm.lenta) mfst.more = true;

		bool syntax_ok = mfst.start();
		mfst.savededucation();
		mfst.printrules();

		if (!syntax_ok) {
			cout << "\n- Синтаксический анализ обнаружил ошибку(и)\n";
			Log::WriteLine(log.stream, "\n- Синтаксический анализ обнаружил ошибку(и)\n", "");
			// Останавливаемся, не идем в семантику
			return 0;
		}
		cout << "\n- Синтаксический анализ выполнен без ошибок\n";

		// 5. Семантический анализ (Sem)
		if (!Sem::SemAnaliz(lex.lextable, lex.idtable, log)) {
			cout << "\n- Семантический анализ обнаружил ошибку(и)\n";
			Log::WriteLine(log.stream, "\n- Семантический анализ обнаружил ошибку(и)\n", "");
			// Останавливаемся
			return 0;
		}
		cout << "\n- Семантический анализ выполнен без ошибок\n";

		// 6. Польская запись (PN)
		if (!Polish::StartPolish(lex)) {
			cout << "\n- Ошибка при построении Польской записи\n";
			return 0;
		}

		// Вывод ПОЛИЗ в файл .out
		if (fout.is_open()) {
			LT::ShowPolishRaw(lex.lextable, lex.idtable, &fout);
			fout.close();
		}
		cout << "\n- Преобразование выражений (ПОЛИЗ) завершено\n\n";

		// 7. Генерация кода (Generation)
		Gener::CodeGeneration(lex, parm, log);

		// Чистка памяти
		In::Delete(in);
		LT::Delete(lex.lextable);
		IT::Delete(lex.idtable);
		Log::Close(log);

		cout << "\nКомпиляция завершилась\n";
	}
	catch (Error::ERROR e)
	{
		// Если вылетело исключение (например, Ошибка 301 "Нет box" из Лексера)
		Log::WriteError(log, e);
		cout << "\n- Выполнение программы остановлено из-за ошибки\n";
	}
	return 0;
}