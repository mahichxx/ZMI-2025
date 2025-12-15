#include "stdafx.h"
#include "Sem.h"
#include "LT.h"
#include "IT.h"
#include <vector>     // !!! НУЖНО ДЛЯ ПОИСКА ДУБЛИКАТОВ
#include <algorithm>  // !!! НУЖНО ДЛЯ std::find

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

bool Sem::SemAnaliz(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG& log)
{
	bool sem_ok = true;

	for (int i = 0; i < lextable.size; i++)
	{
		// 1. ИГНОРИРОВАНИЕ COUT
		if (lextable.table[i].lexema == LEX_COUT) {
			while (i < lextable.size && lextable.table[i].lexema != LEX_SEMICOLON) {
				i++;
			}
			continue;
		}

		switch (lextable.table[i].lexema)
		{
		// 2. ПРИСВАИВАНИЕ
		case LEX_EQUAL:
		{
			if (lextable.table[i].op == LT::operations::OEQ) break;

			if (i > 0 && lextable.table[i - 1].idxTI != LT_TI_NULLIDX)
			{
				IT::IDDATATYPE leftType = idtable.table[lextable.table[i - 1].idxTI].iddatatype;

				for (int k = i + 1; k < lextable.size && lextable.table[k].lexema != LEX_SEMICOLON; k++)
				{
					if (lextable.table[k].lexema == LEX_LEFTTHESIS) {
						int balance = 1; k++;
						while (k < lextable.size && balance > 0) {
							if (lextable.table[k].lexema == LEX_LEFTTHESIS) balance++;
							if (lextable.table[k].lexema == LEX_RIGHTTHESIS) balance--;
							k++;
						}
						k--; continue;
					}

					if (lextable.table[k].idxTI != LT_TI_NULLIDX)
					{
						if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL)
						{
							IT::IDDATATYPE rightType = idtable.table[lextable.table[k].idxTI].iddatatype;
							if (rightType == IT::NUL) rightType = IT::INT;

							if (!areTypesCompatible(leftType, rightType)) {
								Log::WriteErrors(log, Error::geterrorin(314, lextable.table[k].sn, 0));
								sem_ok = false;
							}
						}
					}
				}
			}
			break;
		}

		// 3. ИДЕНТИФИКАТОРЫ
		case LEX_ID:
		{
			if (lextable.table[i].idxTI == LT_TI_NULLIDX) break;
			IT::Entry e = idtable.table[lextable.table[i].idxTI];

			if (e.idtype == IT::F && i + 1 < lextable.size && lextable.table[i + 1].lexema == LEX_LEFTTHESIS)
			{
				bool isDeclaration = (i > 0 && lextable.table[i - 1].lexema == LEX_FUNCTION);
				
				if (!isDeclaration) {
					int paramCount = 0;
					int k = i + 2; 
					if (lextable.table[k].lexema != LEX_RIGHTTHESIS) {
						paramCount = 1; 
						int balance = 0;
						while (k < lextable.size) {
							if (lextable.table[k].lexema == LEX_LEFTTHESIS) balance++;
							if (lextable.table[k].lexema == LEX_RIGHTTHESIS) {
								if (balance == 0) break;
								balance--;
							}
							if (lextable.table[k].lexema == LEX_COMMA && balance == 0) paramCount++;
							k++;
						}
					}
					if (paramCount != e.parm) {
						Log::WriteErrors(log, Error::geterrorin(308, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}
			break;
		}

		// 4. ОПЕРАТОРЫ
		case LEX_OPERATOR:
		{
			if (lextable.table[i].op == LT::OLESS || lextable.table[i].op == LT::OEQ || 
				lextable.table[i].op == LT::ONE || lextable.table[i].op == LT::OMORE || 
				lextable.table[i].lexema == LEX_EQUAL) break;

			if (i > 0 && i + 1 < lextable.size) {
				int lIdx = lextable.table[i - 1].idxTI;
				int rIdx = lextable.table[i + 1].idxTI;

				if (lIdx != LT_TI_NULLIDX && rIdx != LT_TI_NULLIDX) {
					IT::IDDATATYPE t1 = idtable.table[lIdx].iddatatype;
					IT::IDDATATYPE t2 = idtable.table[rIdx].iddatatype;

					if (!isNumeric(t1) || !isNumeric(t2)) {
						Log::WriteErrors(log, Error::geterrorin(314, lextable.table[i].sn, 0));
						sem_ok = false;
					}
				}
			}
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
		
		// 5. SWITCH (CHECK) - РАСШИРЕННАЯ ПРОВЕРКА
		case LEX_SWITCH: {
			bool hasDefault = false;
			int defaultCount = 0;
			std::vector<int> caseValues; // Список значений case для проверки дубликатов

			int j = i + 1;
			while (j < lextable.size && lextable.table[j].lexema != LEX_LEFTBRACE) j++;
			
			if (j < lextable.size) {
				int balance = 1; j++;
				while (j < lextable.size && balance > 0) {
					if (lextable.table[j].lexema == LEX_LEFTBRACE) balance++;
					else if (lextable.table[j].lexema == LEX_BRACELET) balance--;
					
					// Проверяем только верхний уровень switch (balance == 1)
					if (balance == 1) {
						// Проверка на несколько DEFAULT (ELSE)
						if (lextable.table[j].lexema == LEX_DEFAULT) {
							hasDefault = true;
							defaultCount++;
							if (defaultCount > 1) {
								// Ошибка 321: Множественный default
								Log::WriteErrors(log, Error::geterrorin(321, lextable.table[j].sn, 0));
								sem_ok = false;
							}
						}
						// Проверка на дубликаты CASE (IS)
						else if (lextable.table[j].lexema == LEX_CASE) {
							// Значение кейса идет следующим (j+1)
							if (j + 1 < lextable.size && lextable.table[j + 1].lexema == LEX_LITERAL) {
								int val = idtable.table[lextable.table[j + 1].idxTI].value.vint;
								
								// Ищем, было ли такое значение
								if (std::find(caseValues.begin(), caseValues.end(), val) != caseValues.end()) {
									// Ошибка 320: Дубликат case
									Log::WriteErrors(log, Error::geterrorin(320, lextable.table[j].sn, 0));
									sem_ok = false;
								} else {
									caseValues.push_back(val);
								}
							}
						}
					}
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