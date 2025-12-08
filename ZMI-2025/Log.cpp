#include "stdafx.h"
#include "Log.h"	
#include <time.h>
#include <cstdarg> // Обязательно

#pragma warning(disable:4996) // Для time(), если не хочется менять на _s

using namespace std;

#include <locale>	
#include <cwchar>
#include <iostream>
#include "Parm.h"
#include <fstream>

namespace Log {
	LOG getlog(wchar_t logfile[]) {
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail())
			throw ERROR_THROW(112);
		wcscpy_s(log.logfile, logfile);
		return log;
	}

	// ИСПРАВЛЕННЫЙ WriteLine (char)
	void WriteLine(ostream* log, const char* c, ...) {
		if (!log) return;
		*log << c; // Пишем первый аргумент

		va_list args;
		va_start(args, c);

		const char* param = va_arg(args, const char*);
		while (param != nullptr && param[0] != '\0') // Договоримся, что "" или NULL - конец
		{
			*log << param;
			param = va_arg(args, const char*);
		}

		va_end(args);
		*log << endl;
	}

	// ИСПРАВЛЕННЫЙ WriteLine (wchar_t)
	void WriteLine(ostream* log, const wchar_t* c, ...)
	{
		if (!log) return;

		char temp[1024]; // Увеличил буфер
		wcstombs(temp, c, sizeof(temp));
		*log << temp;

		va_list args;
		va_start(args, c);

		const wchar_t* param = va_arg(args, const wchar_t*);
		while (param != nullptr && param[0] != L'\0')
		{
			wcstombs(temp, param, sizeof(temp));
			*log << temp;
			param = va_arg(args, const wchar_t*);
		}
		va_end(args);
		*log << endl;
	}

	void WriteLog(ostream* log) {
		char temp[100];
		time_t tmr;
		time(&tmr);

		// Безопасная версия localtime
		tm tmf;
		localtime_s(&tmf, &tmr);

		strftime(temp, sizeof(temp), "\n---------------Протокол-----------------\n ------%d.%m.%y %H:%M:%S-----\n  ", &tmf);
		*log << temp;
	}

	void WriteIn(ostream* log, In::IN in) {
		*log
			<< "\n\nВсего символов: " << in.size
			<< "\n\nВсего строк: " << in.lines
			<< "\n\nПропущено: " << in.ignor << endl;
	}

	void WriteParm(ostream* log, Parm::PARM parm) {
		char in_text[PARM_MAX_SIZE];
		char out_text[PARM_MAX_SIZE];
		char log_text[PARM_MAX_SIZE];

		size_t converted;
		wcstombs_s(&converted, in_text, PARM_MAX_SIZE, parm.in, _TRUNCATE);
		wcstombs_s(&converted, out_text, PARM_MAX_SIZE, parm.out, _TRUNCATE);
		wcstombs_s(&converted, log_text, PARM_MAX_SIZE, parm.log, _TRUNCATE);

		*log << "\n ---- Параметры ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;
	}

	void WriteError(Log::LOG log, Error::ERROR error) {
		if (log.stream)
		{
			*log.stream << "\nОшибка " << error.id << ": " << error.message << " cтрока " << error.inext.line << " позиция " << error.inext.col << endl;
		}
		// throw error; // Убрал throw, логирование не должно прерывать, прерывает тот, кто вызвал
	}

	void WriteErrors(Log::LOG log, Error::ERROR error) {
		if (log.stream)
		{
			*log.stream << "\nОшибка " << error.id << ": " << error.message << " cтрока " << error.inext.line << " позиция " << error.inext.col << endl;
		}
		cout << "\nОшибка " << error.id << ": " << error.message << " cтрока " << error.inext.line << " позиция " << error.inext.col << endl;
	}

	void Close(LOG log) {
		if (log.stream) {
			log.stream->close();
			delete log.stream;
		}
	}
}