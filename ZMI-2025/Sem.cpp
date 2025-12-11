#include "stdafx.h"
#include "Sem.h"
#include "LT.h"
#include "IT.h"

// Хелпер: числовой ли тип
bool isNumeric(IT::IDDATATYPE t) {
	return (t == IT::INT || t == IT::CHR);
}

// Хелпер: совместимы ли типы
bool areTypesCompatible(IT::IDDATATYPE t1, IT::IDDATATYPE t2) {
	if (t1 == t2) return true;
	if (isNumeric(t1) && isNumeric(t2)) return true;
	return false;
}

bool Sem::SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log)
{
	bool sem_ok = true;

	for (int i = 0; i < lextable.size; i++)
	{
		// ========================================================================
		// 1. ИГНОРИРОВАНИЕ COUT
		// Если видим cout, пропускаем всё до точки с запятой.
		// Это решает проблему ошибок на строках вида: cout < "text";
		// ========================================================================
		if (lextable.table[i].lexema == LEX_COUT) {
			while (i < lextable.size && lextable.table[i].lexema != LEX_SEMICOLON) {
				i++;
			}
			continue;
		}

		switch (lextable.table[i].lexema)
		{
			// 2. ПРИСВАИВАНИЕ (ID = ...)
		case LEX_EQUAL:
		{
			// Игнорируем оператор сравнения ==
			if (lextable.table[i].op == LT::operations::OEQ) break;

			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				IT::IDDATATYPE leftType = idtable.table[lextable.table[i - 1].idxTI].iddatatype;

				// Сканируем правую часть
				for (int k = i + 1; k < lextable.size && lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (lextable.table[k].idxTI != LT_TI_NULLIDX)
					{
						IT::Entry e = idtable.table[lextable.table[k].idxTI];

						// Если справа ФУНКЦИЯ (например, mixer)
						if (e.idtype == IT::F) {
							// Проверяем, совпадает ли возвращаемый тип функции с переменной
							if (!areTypesCompatible(leftType, e.iddatatype)) {
								Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
								sem_ok = false;
							}
							// !!! ГЛАВНОЕ ИСПРАВЛЕНИЕ !!!
							// Мы нашли функцию. Всё, что идет дальше (скобки, аргументы) — это её внутренности.
							// Мы ПРЕРЫВАЕМ проверку присваивания, чтобы не сравнивать аргументы '100', '28' с переменной 'result'.
							// Аргументы будут проверены отдельно в блоке case LEX_ID.
							break;
						}

						// Если справа ПЕРЕМЕННАЯ или ЛИТЕРАЛ
						if (e.idtype == IT::V || e.idtype == IT::L || e.idtype == IT::P)
						{
							// Проверяем тип
							if (!areTypesCompatible(leftType, e.iddatatype)) {
								Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
								sem_ok = false;
							}
						}
					}
				}
			}
			break;
		}

		// 3. ИДЕНТИФИКАТОРЫ (Проверка параметров функций)
		case LEX_ID:
		{
			if (lextable.table[i].idxTI == LT_TI_NULLIDX) break;
			IT::Entry e = idtable.table[lextable.table[i].idxTI];

			// Если это вызов функции: ID ( ... )
			// Проверяем, что это не объявление функции (слово function перед ID)
			if (e.idtype == IT::F && i + 1 < lextable.size && lextable.table[i + 1].lexema == LEX_LEFTTHESIS)
			{
				bool isDeclaration = (i > 0 && lextable.table[i - 1].lexema == LEX_FUNCTION);

				if (!isDeclaration) {
					// Считаем параметры
					int paramCount = 0;
					int k = i + 2;

					// Если внутри не пусто
					if (lextable.table[k].lexema != LEX_RIGHTTHESIS) {
						paramCount = 1; // Первый параметр есть
						int balance = 0;
						while (k < lextable.size) {
							if (lextable.table[k].lexema == LEX_LEFTTHESIS) balance++;
							if (lextable.table[k].lexema == LEX_RIGHTTHESIS) {
								if (balance == 0) break;
								balance--;
							}
							// Считаем запятые на верхнем уровне вложенности
							if (lextable.table[k].lexema == LEX_COMMA && balance == 0) paramCount++;
							k++;
						}
					}

					// Сравниваем количество
					if (paramCount != e.parm) {
						// Ошибка 308: Неверное количество параметров
						Log::WriteErrors(log, Error::geterrorin(308, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}
			break;
		}

		// 4. ОПЕРАТОРЫ (Арифметика)
		case LEX_OPERATOR:
		{
			// Игнорируем < (OLESS), так как он используется в cout. 
			// Игнорируем сравнения.
			if (lextable.table[i].op == LT::OLESS || lextable.table[i].op == LT::OEQ ||
				lextable.table[i].op == LT::ONE || lextable.table[i].op == LT::OMORE ||
				lextable.table[i].lexema == LEX_EQUAL) break;

			// Проверяем типы слева и справа только для + - * /
			if (i > 0 && i + 1 < lextable.size) {
				int lIdx = lextable.table[i - 1].idxTI;
				int rIdx = lextable.table[i + 1].idxTI;

				if (lIdx != LT_TI_NULLIDX && rIdx != LT_TI_NULLIDX) {
					IT::IDDATATYPE t1 = idtable.table[lIdx].iddatatype;
					IT::IDDATATYPE t2 = idtable.table[rIdx].iddatatype;

					// Только числа (int, char) можно складывать/умножать
					if (!isNumeric(t1) || !isNumeric(t2)) {
						Log::WriteErrors(log, Error::geterrorin(314, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}

			// Деление на ноль
			if (lextable.table[i].op == LT::ODIV || lextable.table[i].op == LT::OMOD) {
				if (lextable.table[i + 1].lexema == LEX_LITERAL && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i + 1].idxTI].value.vint == 0) {
						Log::WriteErrors(log, Error::geterrorin(318, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}
			break;
		}

		// 5. SWITCH (Проверка наличия default)
		case LEX_SWITCH: {
			bool hasDefault = false;
			int j = i + 1;
			// Ищем начало тела
			while (j < lextable.size && lextable.table[j].lexema != LEX_LEFTBRACE) j++;

			if (j < lextable.size) {
				int balance = 1; j++;
				while (j < lextable.size && balance > 0) {
					if (lextable.table[j].lexema == LEX_LEFTBRACE) balance++;
					else if (lextable.table[j].lexema == LEX_BRACELET) balance--;
					// Ищем default только на первом уровне вложенности
					else if (lextable.table[j].lexema == LEX_DEFAULT && balance == 1) hasDefault = true;
					j++;
				}
			}
			if (!hasDefault) {
				Log::WriteErrors(log, Error::geterrorin(319, lextable.table[i].sn, 0));
				sem_ok = false;
			}
			break;
		}
		}
	}
	return sem_ok;
}