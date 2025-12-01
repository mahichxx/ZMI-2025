#include "Sem.h"

// Вспомогательная функция для проверки совместимости типов
bool areTypesCompatible(IT::IDDATATYPE t1, IT::IDDATATYPE t2) {
	if (t1 == t2) return true;
	// Строгая типизация: нельзя смешивать int и char
	return false;
}

bool Sem::SemAnaliz(LT::LexTable lextable, IT::IdTable idtable, Log::LOG log)
{
	bool sem_ok = true;

	for (int i = 0; i < lextable.size; i++)
	{
		switch (lextable.table[i].lexema)
		{
			// !!! МЫ УБРАЛИ CASE LEX_DIRSLASH, ЧТОБЫ НЕ БЫЛО ОШИБКИ !!!
			// Логика проверки деления теперь внутри LEX_OPERATOR

			// 1. Проверка типов при присваивании (ID = Expression)
		case LEX_EQUAL:
		{
			// Пропускаем, если это часть оператора сравнения ==
			if (lextable.table[i].op == LT::operations::OEQ) break;

			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				bool ignore = false;

				for (int k = i + 1; lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (k >= lextable.size) break;

					// Пропускаем вызовы функций внутри скобок
					if (lextable.table[k].lexema == LEX_LEFTTHESIS) { ignore = true; continue; }
					if (lextable.table[k].lexema == LEX_RIGHTTHESIS) { ignore = false; continue; }
					if (ignore) continue;

					if (lextable.table[k].idxTI != LT_TI_NULLIDX)
					{
						if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL)
						{
							IT::IDDATATYPE righttype = idtable.table[lextable.table[k].idxTI].iddatatype;

							// Если типы не совпадают
							if (!areTypesCompatible(lefttype, righttype))
							{
								Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
								sem_ok = false;
								break;
							}
						}
					}

					// Проверка операций со строками
					if (lefttype == IT::IDDATATYPE::STR)
					{
						if (lextable.table[k].lexema == LEX_OPERATOR)
						{
							// Если это минус, умножить, делить для строк - ошибка
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

		// 2. Проверка возвращаемого значения функций и параметров
		case LEX_ID:
		{
			if (lextable.table[i].idxTI == LT_TI_NULLIDX) break;
			IT::Entry e = idtable.table[lextable.table[i].idxTI];

			// Если это объявление функции
			if (i > 0 && lextable.table[i - 1].lexema == LEX_FUNCTION)
			{
				if (e.idtype == IT::IDTYPE::F)
				{
					for (int k = i + 1; k < lextable.size; k++)
					{
						if (lextable.table[k].lexema == LEX_FUNCTION || lextable.table[k].lexema == LEX_MAIN) break;

						if (lextable.table[k].lexema == LEX_RETURN)
						{
							// VOID
							if (e.iddatatype == IT::IDDATATYPE::VOI) {
								if (lextable.table[k + 1].lexema != LEX_SEMICOLON) {
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
								}
							}
							// НЕ VOID
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

			// Если это вызов функции
			if (lextable.table[i + 1].lexema == LEX_LEFTTHESIS && (i == 0 || lextable.table[i - 1].lexema != LEX_FUNCTION))
			{
				if (e.idtype == IT::IDTYPE::F)
				{
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
							IT::IDDATATYPE argType = idtable.table[lextable.table[j].idxTI].iddatatype;
							IT::IDDATATYPE paramType = idtable.table[lextable.table[i].idxTI + paramscount].iddatatype;

							if (!areTypesCompatible(argType, paramType)) {
								Log::WriteErrors(log, Error::geterrorin(309, lextable.table[i].sn, 0));
								sem_ok = false;
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

		// 3. Проверка операторов (+, -, /, *, >, <...)
		case LEX_OPERATOR:
		{
			// (А) ПРОВЕРКА ДЕЛЕНИЯ НА НОЛЬ
			if (lextable.table[i].op == LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
				if (lextable.table[i + 1].lexema == LEX_LITERAL && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::INT &&
						idtable.table[lextable.table[i + 1].idxTI].value.vint == 0) {
						Log::WriteErrors(log, Error::geterrorin(318, lextable.table[i + 1].sn, 0));
						sem_ok = false;
					}
				}
			}

			// (Б) ПРОВЕРКА ТИПОВ ОПЕРАНДОВ
			bool flag = true;
			// Левый операнд
			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX) {
				// Для арифметики только INT (так как char у нас отдельный тип, а string не поддерживает -, *, /)
				if (lextable.table[i].op <= LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
					if (idtable.table[lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
						flag = false;
				}
			}
			// Правый операнд
			if (i < lextable.size - 1 && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
				if (lextable.table[i].op <= LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
						flag = false;
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