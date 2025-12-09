#include "stdafx.h"
#include "Sem.h"
#include "LT.h"
#include "IT.h"

// Вспомогательная функция проверки совместимости типов
bool areTypesCompatible(IT::IDDATATYPE t1, IT::IDDATATYPE t2) {
	// Для курсового часто int и char считаются совместимыми, но по твоему заданию int = 1 байт.
	// Оставим строгое равенство.
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
			// Если это оператор сравнения ==, пропускаем
			if (lextable.table[i].op == LT::operations::OEQ) break;

			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				bool ignore = false;

				// Бежим вправо до точки с запятой
				for (int k = i + 1; k < lextable.size && lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (lextable.table[k].lexema == LEX_LEFTTHESIS) { ignore = true; continue; }
					if (lextable.table[k].lexema == LEX_RIGHTTHESIS) { ignore = false; continue; }
					if (ignore) continue;

					if (lextable.table[k].idxTI != LT_TI_NULLIDX)
					{
						if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL)
						{
							IT::IDDATATYPE righttype = idtable.table[lextable.table[k].idxTI].iddatatype;

							// Разрешаем присваивать char в int и наоборот (так как int 1 байт)
							bool compatible = areTypesCompatible(lefttype, righttype);
							if (lefttype == IT::INT && righttype == IT::CHR) compatible = true;
							if (lefttype == IT::CHR && righttype == IT::INT) compatible = true;

							if (!compatible)
							{
								Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
								sem_ok = false;
							}
						}
					}

					// Проверка операций для строк
					if (lefttype == IT::IDDATATYPE::STR)
					{
						if (lextable.table[k].lexema == LEX_OPERATOR)
						{
							char op = lextable.table[k].op;
							// Для строк запрещены -, *, /
							if (op == LT::operations::OMINUS || op == LT::operations::OMUL || op == LT::operations::ODIV) {
								Log::WriteErrors(log, Error::geterrorin(316, lextable.table[k].sn, 0));
								sem_ok = false;
							}
						}
					}
				}
			}
			break;
		}

		// 2. Проверка функций
		case LEX_ID:
		{
			if (lextable.table[i].idxTI == LT_TI_NULLIDX) break;
			IT::Entry e = idtable.table[lextable.table[i].idxTI];

			// А) Проверка RETURN внутри функции
			if (i > 0 && lextable.table[i - 1].lexema == LEX_FUNCTION)
			{
				if (e.idtype == IT::IDTYPE::F)
				{
					for (int k = i + 1; k < lextable.size; k++)
					{
						if (lextable.table[k].lexema == LEX_FUNCTION || lextable.table[k].lexema == LEX_MAIN) break;

						if (lextable.table[k].lexema == LEX_RETURN)
						{
							// Если void
							if (e.iddatatype == IT::IDDATATYPE::VOI) {
								if (lextable.table[k + 1].lexema != LEX_SEMICOLON) {
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
								}
							}
							else { // Если не void
								if (lextable.table[k + 1].lexema == LEX_SEMICOLON) {
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
								}
								else {
									int nextIdx = lextable.table[k + 1].idxTI;
									if (nextIdx != LT_TI_NULLIDX) {
										IT::IDDATATYPE retType = idtable.table[nextIdx].iddatatype;
										bool compatible = areTypesCompatible(retType, e.iddatatype);
										// Доп. совместимость int <-> char
										if ((retType == IT::INT && e.iddatatype == IT::CHR) ||
											(retType == IT::CHR && e.iddatatype == IT::INT)) compatible = true;

										if (!compatible) {
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
			// Проверяем: ID + ( ... и ID не после FUNCTION
			if (i + 1 < lextable.size && lextable.table[i + 1].lexema == LEX_LEFTTHESIS &&
				(i == 0 || lextable.table[i - 1].lexema != LEX_FUNCTION))
			{
				if (e.idtype == IT::IDTYPE::F)
				{
					// Пропуск библиотечных функций
					std::string fname = (char*)e.id;
					if (fname == "strtoint" || fname == "stcmp") {
						break;
					}

					int paramscount = 0;
					// Сканируем аргументы до закрывающей скобки
					for (int j = i + 2; j < lextable.size && lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL)
						{
							paramscount++;
							if (paramscount > e.parm) {
								Log::WriteErrors(log, Error::geterrorin(308, lextable.table[i].sn, 0));
								sem_ok = false;
								break;
							}

							// Проверка типов
							int expectedParamIndex = lextable.table[i].idxTI + paramscount;
							if (expectedParamIndex < idtable.size) {
								IT::IDDATATYPE argType = idtable.table[lextable.table[j].idxTI].iddatatype;
								IT::IDDATATYPE paramType = idtable.table[expectedParamIndex].iddatatype;

								bool compatible = areTypesCompatible(argType, paramType);
								if ((argType == IT::INT && paramType == IT::CHR) ||
									(argType == IT::CHR && paramType == IT::INT)) compatible = true;

								if (!compatible) {
									Log::WriteErrors(log, Error::geterrorin(309, lextable.table[i].sn, 0));
									sem_ok = false;
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

		// 3. Арифметика и деление на 0
		case LEX_OPERATOR:
		{
			// Деление на ноль
			if (lextable.table[i].op == LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
				if (i + 1 < lextable.size && lextable.table[i + 1].lexema == LEX_LITERAL && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::INT &&
						idtable.table[lextable.table[i + 1].idxTI].value.vint == 0) {
						Log::WriteErrors(log, Error::geterrorin(318, lextable.table[i + 1].sn, 0));
						sem_ok = false;
					}
				}
			}

			// Типы операндов (только INT разрешен для арифметики в твоем коде)
			// Хотя для строк обычно разрешен '+'. Если нужно - добавь исключение для OPLUS
			bool flag = true;
			if (lextable.table[i].op <= LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
				if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::INT &&
						idtable.table[lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::CHR) flag = false;
				}
				if (i < lextable.size - 1 && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::INT &&
						idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::CHR) flag = false;
				}
			}
			if (!flag) {
				Log::WriteErrors(log, Error::geterrorin(317, lextable.table[i].sn, 0));
				sem_ok = false;
			}
			break;
		}
		// 4. Проверка наличия default в switch
		case LEX_SWITCH:
		{
			// Ищем открывающую скобку '{' после switch
			int j = i + 1;
			while (j < lextable.size && lextable.table[j].lexema != LEX_LEFTBRACE) {
				j++;
			}

			if (j >= lextable.size) break; // Если скобки нет, это поймает синтаксический анализатор

			// Сканируем тело switch
			bool hasDefault = false;
			int balance = 1; // Уровень вложенности (мы стоим на '{')
			j++; // Заходим внутрь

			while (j < lextable.size && balance > 0)
			{
				if (lextable.table[j].lexema == LEX_LEFTBRACE) balance++;
				else if (lextable.table[j].lexema == LEX_BRACELET) balance--;

				// Если встретили default И мы находимся на уровне нашего свитча (не вложенного)
				if (lextable.table[j].lexema == LEX_DEFAULT && balance == 1) {
					hasDefault = true;
				}
				j++;
			}

			// Если пробежали весь switch и не нашли default
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