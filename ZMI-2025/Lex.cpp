#define _CRT_SECURE_NO_WARNINGS

#define IN_CODE_DELIMETR '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='
#define MAX_INTEGER 2147483647
#define MIN_INTEGER -2147483647
#define MAX_STROKA 256

#include "FST_def.h"
#include "stdafx.h"
#include <cmath> 
#include <iostream>
#include "Lex.h" // Убедись, что подключен этот заголовок

namespace Lex {

	// Предварительное объявление
	bool checkBrace(unsigned char** word, int k);

	int BinToInt(unsigned char* word) {
		int length = _mbslen(word);
		int sum = 0;
		int p = 0;
		for (int k = length - 1; k >= 2; k--) {
			if (word[k] == '1') sum += (int)pow(2, p);
			p++;
		}
		return sum;
	}

	int HexToInt(unsigned char* word) {
		int length = _mbslen(word);
		int sum = 0;
		int p = 0;
		for (int k = length - 1; k >= 2; k--) {
			int digit = 0;
			if (word[k] >= '0' && word[k] <= '9') digit = word[k] - '0';
			else if (word[k] >= 'A' && word[k] <= 'F') digit = word[k] - 'A' + 10;
			else if (word[k] >= 'a' && word[k] <= 'f') digit = word[k] - 'a' + 10;
			sum += digit * (int)pow(16, p);
			p++;
		}
		return sum;
	}

	LEX lexAnaliz(Log::LOG log, In::IN in)
	{
		LEX lex;
		// Создаем таблицы. Важно: при исключении нужно будет вызвать Delete!
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);

		int i = 0;
		int line = 1;
		int indexLex = 0;
		int indexID = 0;
		int countLit = 1;
		int position = 0;

		IT::Entry entryIT;
		IT::Entry bufentry;
		unsigned char emptystr[] = "";

		bool findFunc = false;
		bool findParm = false;
		bool findSameID = false;
		bool findReturn = false;
		bool endif = false;
		int Idx_Func_IT = 0;
		int Parm_count_IT = 0;
		int count_main = 0;
		bool newindf = false;
		bool errorssem = false;

		// --- БЕЗОПАСНЫЕ БУФЕРЫ НА СТЕКЕ (Без new/delete) ---
		unsigned char RegionPrefix[TI_STR_MAXSIZE] = { 0 };
		unsigned char buferRegionPrefix[TI_STR_MAXSIZE] = { 0 };
		unsigned char pastRegionPrefix[TI_STR_MAXSIZE] = { 0 };
		unsigned char L[] = "L";
		unsigned char bufL[TI_STR_MAXSIZE] = { 0 };
		char charCountLit[20] = { 0 };
		unsigned char nameLiteral[TI_STR_MAXSIZE] = { 0 };
		unsigned char tempBuf[TI_STR_MAXSIZE] = { 0 }; // Вынес из цикла

		unsigned char** word = in.word;

		try {
			for (i = 0; word[i] != NULL && word[i][0] != NULL; indexLex++, i++)
			{
				bool findSameID = false;

				// ============================================================
				// Сложные операторы (<<, ==, !=, ...)
				// ============================================================
				bool isComplexOp = false;
				LT::operations opType = LT::operations::OEQ;
				int priority = 2;

				unsigned char first = word[i][0];
				unsigned char second = (word[i + 1] != NULL && word[i + 1][0] != NULL) ? word[i + 1][0] : 0;

				if (word[i][1] == 0 && second != 0 && word[i + 1][1] == 0) {
					if (first == '<' && second == '<') { isComplexOp = true; opType = LT::operations::OLESS; priority = 1; }
					else if (first == '=' && second == '=') { isComplexOp = true; opType = LT::operations::OEQ; priority = 1; }
					else if (first == '!' && second == '=') { isComplexOp = true; opType = LT::operations::ONE; priority = 1; }
					else if (first == '>' && second == '=') { isComplexOp = true; opType = LT::operations::OGE; priority = 1; }
					else if (first == '<' && second == '=') { isComplexOp = true; opType = LT::operations::OLE; priority = 1; }
				}

				if (isComplexOp) {
					LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
					entryLT.op = opType;
					entryLT.priority = priority;

					unsigned char complexName[3];
					complexName[0] = first;
					complexName[1] = second;
					complexName[2] = '\0';
					_mbscpy(entryIT.id, complexName);

					entryIT.idxfirstLE = indexLex;
					entryIT.idtype = IT::OP;
					IT::Add(idtable, entryIT);
					LT::Add(lextable, entryLT);

					entryIT = bufentry;
					i++; // Пропускаем второй символ оператора
					continue;
				}

				// --- ТИПЫ ДАННЫХ ---
				{
					FST::FST fstTypeInteger(word[i], FST_INTEGER);
					if (FST::execute(fstTypeInteger)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						newindf = true;
						entryIT.iddatatype = IT::INT;
						continue;
					}
				}
				{
					FST::FST fstTypeChar(word[i], FST_TYPE_CHAR);
					if (FST::execute(fstTypeChar)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_CHAR, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						newindf = true;
						entryIT.iddatatype = IT::CHR;
						continue;
					}
				}
				{
					FST::FST fstTypeVoid(word[i], FST_VOID);
					if (FST::execute(fstTypeVoid)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_VOID, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						newindf = true;
						entryIT.iddatatype = IT::VOI;
						continue;
					}
				}
				{
					FST::FST fstTypeString(word[i], FST_STRING);
					if (FST::execute(fstTypeString)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						newindf = true;
						entryIT.iddatatype = IT::STR;
						_mbscpy(entryIT.value.vstr.str, emptystr);
						continue;
					}
				}

				// --- Ключевые слова ---
				// Используем блоки {}, чтобы fst объекты уничтожались сразу после проверки (освобождали память)
				{ FST::FST fstSwitch(word[i], FST_SWITCH); if (FST::execute(fstSwitch)) { LT::Entry entryLT = writeEntry(entryLT, LEX_SWITCH, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstCase(word[i], FST_CASE); if (FST::execute(fstCase)) { LT::Entry entryLT = writeEntry(entryLT, LEX_CASE, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstDefault(word[i], FST_DEFAULT); if (FST::execute(fstDefault)) { LT::Entry entryLT = writeEntry(entryLT, LEX_DEFAULT, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstCout(word[i], FST_COUT); if (FST::execute(fstCout)) { LT::Entry entryLT = writeEntry(entryLT, LEX_COUT, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstTrue(word[i], FST_TRUE); if (FST::execute(fstTrue)) { LT::Entry entryLT = writeEntry(entryLT, LEX_TRUE, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstFalse(word[i], FST_FALSE); if (FST::execute(fstFalse)) { LT::Entry entryLT = writeEntry(entryLT, LEX_FALSE, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }

				// --- Функции / Main ---
				{
					FST::FST fstFunction(word[i], FST_FUNCTION);
					if (FST::execute(fstFunction)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						entryIT.idtype = IT::F;
						findFunc = true; findParm = true; Parm_count_IT = 0; Idx_Func_IT = 0; findReturn = false;
						continue;
					}
				}
				{
					FST::FST fstReturn(word[i], FST_RETURN);
					if (FST::execute(fstReturn)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						findReturn = true;
						continue;
					}
				}
				{
					FST::FST fstIf(word[i], FST_IF);
					if (FST::execute(fstIf)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_IF, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						endif = true;
						continue;
					}
				}
				{
					FST::FST fstMain(word[i], FST_MAIN);
					if (FST::execute(fstMain)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						count_main++; findReturn = false;
						_mbscpy(pastRegionPrefix, RegionPrefix);
						_mbscpy(RegionPrefix, emptystr);
						continue;
					}
				}

				// --- ИДЕНТИФИКАТОРЫ (ID) ---
				FST::FST fstIdentif(word[i], FST_ID);
				if (FST::execute(fstIdentif)) {
					int length = _mbslen(word[i]);
					if (length > 10) Log::WriteError(log, Error::geterrorin(202, line, position));

					int idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) {
						LT::Entry entryLT = LT::writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
					if (!findFunc) {
						_mbscpy(buferRegionPrefix, RegionPrefix);
						// Используем статический буфер
						memset(tempBuf, 0, TI_STR_MAXSIZE);
						_mbscpy(tempBuf, buferRegionPrefix);
						_mbscat(tempBuf, word[i]);

						idx = IT::IsId(idtable, tempBuf);
						if (idx != TI_NULLIDX) {
							LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
							LT::Add(lextable, entryLT);
							continue;
						}
					}
					LT::Entry entryLT = LT::writeEntry(entryLT, LEX_ID, indexID++, line);
					LT::Add(lextable, entryLT);
					if (findParm && !findFunc) {
						entryIT.idtype = IT::P;
						Parm_count_IT++;
					}
					else if (!findFunc) {
						entryIT.idtype = IT::V;
						if (entryIT.iddatatype == IT::INT) entryIT.value.vint = 0;
						else if (entryIT.iddatatype == IT::STR) {
							entryIT.value.vstr.len = 0;
							memset(entryIT.value.vstr.str, 0, sizeof(char));
						}
					}
					else {
						_mbscpy(pastRegionPrefix, RegionPrefix);
						_mbscpy(RegionPrefix, word[i]);
					}
					entryIT.idxfirstLE = indexLex;
					_mbscpy(entryIT.id, word[i]);
					IT::Add(idtable, entryIT);
					entryIT = bufentry;
					if (findFunc) Idx_Func_IT = IT::IsId(idtable, word[i]);
					newindf = false; findFunc = false;
					continue;
				}

				// --- ЛИТЕРАЛЫ (ЧИСЛА) ---
				FST::FST fstLiteralInt(word[i], FST_INTLIT);
				if (FST::execute(fstLiteralInt)) {
					int value = atoi((char*)word[i]);
					if (value > 127 || value < -128) Log::WriteError(log, Error::geterrorin(203, line, position));

					// Логика отрицательных чисел
					bool isNegative = false;
					if (lextable.size > 0) {
						LT::Entry prev = lextable.table[lextable.size - 1];
						if (prev.lexema == LEX_OPERATOR && prev.op == LT::OMINUS) {
							bool isUnary = false;
							if (lextable.size == 1) isUnary = true;
							else {
								unsigned char prePrevLex = lextable.table[lextable.size - 2].lexema;
								if (prePrevLex == LEX_EQUAL || prePrevLex == LEX_LEFTTHESIS ||
									prePrevLex == LEX_COMMA || prePrevLex == LEX_RETURN || prePrevLex == LEX_TWOPOINT) {
									isUnary = true;
								}
							}

							if (isUnary) {
								isNegative = true;
								lextable.size--;
								idtable.size--; // Удаляем оператор минус из ID таблицы тоже
								indexID--;
							}
						}
					}

					if (isNegative) value = -value;

					for (int k = 0; k < idtable.size; k++) {
						if (idtable.table[k].iddatatype == IT::INT && idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L) {
							LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, k, line);
							LT::Add(lextable, entryLT);
							findSameID = true; break;
						}
					}
					if (findSameID) continue;
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
					entryIT.iddatatype = IT::INT;
					entryIT.idtype = IT::L;

					entryIT.value.vint = value;
					entryIT.idxfirstLE = indexLex;

					_itoa_s(countLit++, charCountLit, 20, 10);
					_mbscpy(bufL, L);
					_mbscat(bufL, (unsigned char*)charCountLit);
					_mbscpy(entryIT.id, bufL);

					IT::Add(idtable, entryIT);
					entryIT = bufentry;
					continue;
				}

				// Binary
				FST::FST fstLiteralBin(word[i], FST_BINLIT);
				if (FST::execute(fstLiteralBin)) {
					int value = BinToInt(word[i]);
					if (value > 255) Log::WriteError(log, Error::geterrorin(203, line, position));
					for (int k = 0; k < idtable.size; k++) {
						if (idtable.table[k].iddatatype == IT::INT && idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L) {
							LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, k, line);
							LT::Add(lextable, entryLT);
							findSameID = true; break;
						}
					}
					if (findSameID) continue;
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
					entryIT.iddatatype = IT::INT;
					entryIT.nums = 2;
					entryIT.idtype = IT::L;
					entryIT.value.vint = value;
					entryIT.idxfirstLE = indexLex;

					_itoa_s(countLit++, charCountLit, 20, 10);
					_mbscpy(bufL, L);
					_mbscat(bufL, (unsigned char*)charCountLit);
					_mbscpy(entryIT.id, bufL);

					IT::Add(idtable, entryIT);
					entryIT = bufentry;
					continue;
				}

				// Hex
				FST::FST fstLiteralHex(word[i], FST_INT16LIT);
				if (FST::execute(fstLiteralHex)) {
					int value = HexToInt(word[i]);
					if (value > 255) Log::WriteError(log, Error::geterrorin(203, line, position));
					for (int k = 0; k < idtable.size; k++) {
						if (idtable.table[k].iddatatype == IT::INT && idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L) {
							LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, k, line);
							LT::Add(lextable, entryLT);
							findSameID = true; break;
						}
					}
					if (findSameID) continue;
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
					entryIT.iddatatype = IT::INT;
					entryIT.nums = 16;
					entryIT.idtype = IT::L;
					entryIT.value.vint = value;
					entryIT.idxfirstLE = indexLex;

					_itoa_s(countLit++, charCountLit, 20, 10);
					_mbscpy(bufL, L);
					_mbscat(bufL, (unsigned char*)charCountLit);
					_mbscpy(entryIT.id, bufL);

					IT::Add(idtable, entryIT);
					entryIT = bufentry;
					continue;
				}

				// Char
				FST::FST fstLiteralChar(word[i], FST_CHARLIT);
				if (FST::execute(fstLiteralChar)) {
					char val = word[i][1];
					LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
					entryIT.iddatatype = IT::CHR;
					entryIT.idtype = IT::L;
					entryIT.value.vchar = val;
					entryIT.idxfirstLE = indexLex;

					_itoa_s(countLit++, charCountLit, 20, 10);
					_mbscpy(bufL, L);
					_mbscat(bufL, (unsigned char*)charCountLit);
					_mbscpy(entryIT.id, bufL);

					IT::Add(idtable, entryIT);
					entryIT = bufentry;
					continue;
				}

				// String
				FST::FST fstLiteralString(word[i], FST_STRLIT);
				if (FST::execute(fstLiteralString)) {
					int length = _mbslen(word[i]);
					if (length > MAX_STROKA) {
						Log::WriteError(log, Error::geterrorin(202, line, position));
						throw  ERROR_THROW_IN(202, line, position);
					}
					// Сдвиг кавычек (осторожно с word[i], мы меняем его in-place)
					for (int k = 0; k < length; k++) word[i][k] = word[i][k + 1];
					word[i][length - 2] = 0;

					for (int k = 0; k < idtable.size; k++) {
						if (idtable.table[k].iddatatype == IT::IDDATATYPE::STR && !(_mbscmp(idtable.table[k].value.vstr.str, word[i]))) {
							findSameID = true;
							LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
							LT::Add(lextable, entryLT);
							break;
						}
					}
					if (findSameID) continue;
					LT::Entry entryLT = LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line);
					LT::Add(lextable, entryLT);
					entryIT.iddatatype = IT::STR;
					entryIT.idtype = IT::L;
					entryIT.value.vstr.len = length - 2;
					_mbscpy(entryIT.value.vstr.str, word[i]);
					entryIT.idxfirstLE = indexLex;

					_itoa_s(countLit++, charCountLit, 20, 10);
					_mbscpy(bufL, L);
					_mbscat(bufL, (unsigned char*)charCountLit);
					_mbscpy(entryIT.id, bufL);

					IT::Add(idtable, entryIT);
					entryIT = bufentry;
					continue;
				}

				// --- ОПЕРАТОРЫ ---
				{
					FST::FST fstOperator(word[i], FST_OPERATOR);
					if (FST::execute(fstOperator))
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
						_mbscpy(entryIT.id, word[i]);
						entryIT.idxfirstLE = indexLex;
						entryIT.idtype = IT::OP;
						IT::Add(idtable, entryIT);
						entryIT = bufentry;
						unsigned char first = word[i][0];

						switch (first) {
						case '>': entryLT.priority = 1; entryLT.op = LT::operations::OMORE; break;
						case '<': entryLT.priority = 1; entryLT.op = LT::operations::OLESS; break;
						case '+': entryLT.priority = 2; entryLT.op = LT::operations::OPLUS; break;
						case '-': entryLT.priority = 2; entryLT.op = LT::operations::OMINUS; break;
						case '/': entryLT.priority = 3; entryLT.op = LT::operations::ODIV; break;
						case '*': entryLT.priority = 3; entryLT.op = LT::operations::OMUL; break;
						case '%': entryLT.priority = 3; entryLT.op = LT::operations::OMOD; break;
						case '=': entryLT.lexema = LEX_EQUAL; break;
						}
						LT::Add(lextable, entryLT);
						continue;
					}
				}

				// --- РАЗДЕЛИТЕЛИ ---
				// Также оборачиваем в блоки
				{ FST::FST fstSemicolon(word[i], FST_SEMICOLON); if (FST::execute(fstSemicolon)) { LT::Entry entryLT = writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstComma(word[i], FST_COMMA); if (FST::execute(fstComma)) { LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstLeftBrace(word[i], FST_LEFTBRACE); if (FST::execute(fstLeftBrace)) { LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstRightBrace(word[i], FST_BRACELET); if (FST::execute(fstRightBrace)) { LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS); if (FST::execute(fstLeftThesis)) { LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }

				{
					FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
					if (FST::execute(fstRightThesis)) {
						LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
						// Проверка на выход за границы в checkBrace уже внутри функции, но проверим тут наличие элементов
						if (findParm && word[i + 1] != NULL && word[i + 1][0] != LEX_LEFTBRACE &&
							word[i + 2] != NULL && word[i + 2][0] != LEX_LEFTBRACE && !checkBrace(word, i + 1)) {
							_mbscpy(RegionPrefix, pastRegionPrefix);
						}
						if (findParm) idtable.table[Idx_Func_IT].parm = Parm_count_IT;
						findParm = false; Parm_count_IT = 0; Idx_Func_IT = 0;
						LT::Add(lextable, entryLT);
						continue;
					}
				}

				{ FST::FST fstTwoPoint(word[i], FST_TWOPOINT); if (FST::execute(fstTwoPoint)) { LT::Entry entryLT = writeEntry(entryLT, LEX_TWOPOINT, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }
				{ FST::FST fstEqual(word[i], FST_EQUAL); if (FST::execute(fstEqual)) { LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line); LT::Add(lextable, entryLT); continue; } }

				// Проверка на незакрытую строку (или дублирующая проверка)
				{
					FST::FST fstLitStr_1(word[i], FST_LITERALSTRING_1);
					if (FST::execute(fstLitStr_1)) {
						// Если мы здесь, значит FST_STRLIT сработал бы раньше. 
						// Если код дошел сюда, значит это либо ошибка логики, либо специально для отлова ошибок.
						// Предположим, что это catch-all для кривых строк.
						LT::Entry entryLT = writeEntry(entryLT, word[i][0], LT_TI_NULLIDX, line);
						LT::Add(lextable, entryLT);
						errorssem = true;
						Log::WriteErrors(log, Error::geterrorin(311, line, position));
						continue;
					}
				}

				position += _mbslen(word[i]);
				if (word[i][0] == IN_CODE_DELIMETR && endif) {
					LT::Entry entryLT = writeEntry(entryLT, LEX_ENDIF, LT_TI_NULLIDX, line);
					LT::Add(lextable, entryLT);
					endif = false; continue;
				}
				if (word[i][0] == IN_CODE_DELIMETR) {
					line++; position = 0; indexLex--; continue;
				}

				// Если ничего не подошло
				Log::WriteError(log, Error::geterrorin(201, line, position));
			};
		}
		catch (...) {
			// Если произошла фатальная ошибка, чистим память перед выходом
			IT::Delete(idtable);
			LT::Delete(lextable);
			throw;
		}

		lex.idtable = idtable;
		lex.lextable = lextable;
		if (count_main > 1) { errorssem = true; Log::WriteErrors(log, Error::geterror(302)); }
		if (count_main == 0) { errorssem = true; Log::WriteErrors(log, Error::geterror(301)); }
		if (count_main == 1 && findReturn) {
			// Здесь тоже стоит почистить, если бросаем исключение
			IT::Delete(idtable); LT::Delete(lextable);
			throw Error::geterror(601);
		}
		if (errorssem) {
			IT::Delete(idtable); LT::Delete(lextable);
			throw Error::geterror(113);
		}
		return lex;
	}

	bool checkBrace(unsigned char** word, int k)
	{
		// Защита от выхода за пределы массива слов
		// Предполагаем, что word завершается NULL указателем
		while (word[k] != NULL && word[k][0] == IN_CODE_DELIMETR) { k++; }

		if (word[k] == NULL) return 0;
		if (word[k][0] == LEX_LEFTBRACE) return 1;
		else return 0;
	}

	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)
	{
		if (itTableIndex == TI_NULLIDX) return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI) return i;
		return TI_NULLIDX;
	}
}