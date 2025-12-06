#include "Sem.h"
#include "LT.h" // Обязательно подключаем, чтобы видеть поля таблиц
#include "IT.h" // Обязательно подключаем

// Вспомогательная функция. НЕ УДАЛЯТЬ.
// Она нужна для проверки, совпадают ли типы (например, int = int).
bool areTypesCompatible(IT::IDDATATYPE t1, IT::IDDATATYPE t2) {
	if (t1 == t2) return true;
	return false;
}

// ОБРАТИ ВНИМАНИЕ: Добавлены значки '&' перед названиями переменных таблиц,
// чтобы соответствовать Sem.h
bool Sem::SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log)
{
	bool sem_ok = true;

	for (int i = 0; i < lextable.size; i++)
	{
		switch (lextable.table[i].lexema)
		{
			// 1. Проверка типов при присваивании (ID = Expression)
		case LEX_EQUAL:
		{
			// Пропускаем, если это часть оператора сравнения == (OEQ)
			if (lextable.table[i].op == LT::operations::OEQ) break;

			// Смотрим, что стоит слева от равно (куда присваиваем)
			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				bool ignore = false;

				// Бежим вправо до точки с запятой
				for (int k = i + 1; lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (k >= lextable.size) break;

					// Пропускаем выражения внутри скобок (упрощение)
					if (lextable.table[k].lexema == LEX_LEFTTHESIS) { ignore = true; continue; }
					if (lextable.table[k].lexema == LEX_RIGHTTHESIS) { ignore = false; continue; }
					if (ignore) continue;

					if (lextable.table[k].idxTI != LT_TI_NULLIDX)
					{
						// Если встретили переменную или литерал
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

					// Спец. проверка для строк (нельзя вычитать/умножать строки)
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

		// 2. Проверка возвращаемого значения функций и параметров
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
						// Ищем return до конца функции или до начала следующей
						if (lextable.table[k].lexema == LEX_FUNCTION || lextable.table[k].lexema == LEX_MAIN) break;

						if (lextable.table[k].lexema == LEX_RETURN)
						{
							// Если функция VOID
							if (e.iddatatype == IT::IDDATATYPE::VOI) {
								if (lextable.table[k + 1].lexema != LEX_SEMICOLON) {
									Log::WriteErrors(log, Error::geterrorin(315, lextable.table[k].sn, 0));
									sem_ok = false;
								}
							}
							// Если функция НЕ VOID
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
					int paramscount = 0;
					// Перебираем аргументы внутри скобок
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
							// Тип переданного аргумента
							IT::IDDATATYPE argType = idtable.table[lextable.table[j].idxTI].iddatatype;
							// Тип ожидаемого параметра (он записан в таблице ID сразу после имени функции)
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

		// 3. Проверка арифметических операторов
		case LEX_OPERATOR:
		{
			// (А) ПРОВЕРКА ДЕЛЕНИЯ НА НОЛЬ (для литералов)
			if (lextable.table[i].op == LT::operations::ODIV || lextable.table[i].op == LT::operations::OMOD) {
				if (lextable.table[i + 1].lexema == LEX_LITERAL && lextable.table[i + 1].idxTI != LT_TI_NULLIDX) {
					if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::INT &&
						idtable.table[lextable.table[i + 1].idxTI].value.vint == 0) {
						Log::WriteErrors(log, Error::geterrorin(318, lextable.table[i + 1].sn, 0));
						sem_ok = false;
					}
				}
			}

			// (Б) ПРОВЕРКА ТИПОВ ОПЕРАНДОВ (только INT для арифметики)
			bool flag = true;
			// Левый операнд
			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX) {
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