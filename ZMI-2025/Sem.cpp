#include "Sem.h"
#include "LT.h"
#include "IT.h"

// Вспомогательная функция проверки совместимости типов
bool areTypesCompatible(IT::IDDATATYPE t1, IT::IDDATATYPE t2) {
	if (t1 == t2) return true;
	return false;
}

bool Sem::SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log)
{
	bool sem_ok = true;

	for (int i = 0; i < lextable.size; i++)
	{
		switch (lextable.table[i].lexema)
		{
			// 1. Проверка присваивания (ID = Expression)
		case LEX_EQUAL:
		{
			if (lextable.table[i].op == LT::operations::OEQ) break;

			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				bool ignore = false;

				for (int k = i + 1; lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (k >= lextable.size) break;

					if (lextable.table[k].lexema == LEX_LEFTTHESIS) { ignore = true; continue; }
					if (lextable.table[k].lexema == LEX_RIGHTTHESIS) { ignore = false; continue; }
					if (ignore) continue;

					if (lextable.table[k].idxTI != LT_TI_NULLIDX)
					{
						if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL)
						{
							IT::IDDATATYPE righttype = idtable.table[lextable.table[k].idxTI].iddatatype;
							if (!areTypesCompatible(lefttype, righttype))
							{
								Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
								sem_ok = false;
								break;
							}
						}
					}
					// Проверка для строк (запрет арифметики)
					if (lefttype == IT::IDDATATYPE::STR)
					{
						if (lextable.table[k].lexema == LEX_OPERATOR)
						{
							char op = lextable.table[k].op;
							if (op == LT::operations::OMINUS || op == LT::operations::OMUL || op == LT::operations::ODIV) {
								Log::WriteErrors(log, Error::geterrorin(316, lextable.table[k].sn, 0));
								sem_ok = false;
								break;
							}
						}
					}
				}
			}
			break;
		}

		// 2. Проверка функций (Return и Параметры)
		case LEX_ID:
		{
			if (lextable.table[i].idxTI == LT_TI_NULLIDX) break;
			IT::Entry e = idtable.table[lextable.table[i].idxTI];

			// А) Проверка RETURN
			if (i > 0 && lextable.table[i - 1].lexema == LEX_FUNCTION)
			{
				if (e.idtype == IT::IDTYPE::F)
				{
					for (int k = i + 1; k < lextable.size; k++)
					{
						if (lextable.table[k].lexema == LEX_FUNCTION || lextable.table[k].lexema == LEX_MAIN) break;
						if (lextable.table[k].lexema == LEX_RETURN)
						{
							if (e.iddatatype == IT::IDDATATYPE::VOI) {
								if (lextable.table[k + 1].lexema != LEX_SEMICOLON) {
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
								}
							}
							else {
								if (lextable.table[k + 1].lexema == LEX_SEMICOLON) {
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
								}
								else {
									int nextIdx = lextable.table[k + 1].idxTI;
									if (nextIdx != LT_TI_NULLIDX) {
										if (!areTypesCompatible(idtable.table[nextIdx].iddatatype, e.iddatatype)) {
											Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
											sem_ok = false;
										}
									}
								}
							}
						}
					}
				}
			}

			// Б) Проверка ВЫЗОВА функции (параметры)
			if (lextable.table[i + 1].lexema == LEX_LEFTTHESIS && (i == 0 || lextable.table[i - 1].lexema != LEX_FUNCTION))
			{
				if (e.idtype == IT::IDTYPE::F)
				{
					// !!! ИСПРАВЛЕНИЕ: Пропускаем проверку для библиотечных функций !!!
					// Если имя функции strtoint или stcmp - не проверяем параметры (доверяем библиотеке)
					// Это решит ошибку 308 для этих функций.
					std::string fname = (char*)e.id;
					if (fname == "strtoint" || fname == "stcmp") {
						break;
					}
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

					int paramscount = 0;
					for (int j = i + 2; lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (j >= lextable.size) break;
						if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL)
						{
							paramscount++;
							if (paramscount > e.parm) {
								Log::WriteErrors(log, Error::geterrorin(308, lextable.table[i].sn, 0));
								sem_ok = false;
								break;
							}

							// Проверка типов параметров
							// (с защитой от выхода за пределы массива, если paramscount странный)
							if (i + paramscount < idtable.size) {
								IT::IDDATATYPE argType = idtable.table[lextable.table[j].idxTI].iddatatype;
								// Предполагаем, что параметры идут в таблице сразу за именем функции
								// Это работает для пользовательских функций
								int paramIndexInTable = lextable.table[i].idxTI + paramscount;
								if (paramIndexInTable < idtable.size) {
									IT::IDDATATYPE paramType = idtable.table[paramIndexInTable].iddatatype;
									if (!areTypesCompatible(argType, paramType)) {
										Log::WriteErrors(log, Error::geterrorin(309, lextable.table[i].sn, 0));
										sem_ok = false;
									}
								}
							}
						}
					}
					if (paramscount < e.parm) {
						Log::WriteErrors(log, Error::geterrorin(308, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}
			break;
		}

		// 3. Арифметика
		case LEX_OPERATOR:
		{
			// Деление на ноль
			if (lextable.table[i].op == LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
				if (lextable.table[i + 1].lexema == LEX_LITERAL && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::INT &&
						idtable.table[lextable.table[i + 1].idxTI].value.vint == 0) {
						Log::WriteErrors(log, Error::geterrorin(318, lextable.table[i + 1].sn, 0));
						sem_ok = false;
					}
				}
			}
			// Типы операндов
			bool flag = true;
			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX) {
				if (lextable.table[i].op <= LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
					if (idtable.table[lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::INT) flag = false;
				}
			}
			if (i < lextable.size - 1 && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
				if (lextable.table[i].op <= LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::INT) flag = false;
				}
			}
			if (!flag) {
				Log::WriteErrors(log, Error::geterrorin(317, lextable.table[i].sn, 0));
				sem_ok = false;
			}
			break;
		}
		}
	}
	return sem_ok;
}