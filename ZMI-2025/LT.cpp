#include "stdafx.h"
#include "LT.h"
#include "Error.h"
#include "IT.h" // Здесь подключаем IT.h, чтобы видеть внутренности таблицы

using namespace std;

namespace LT
{
	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE) throw ERROR_THROW(105);
		LexTable Table;
		Table.max_size = size;
		Table.size = 0;
		Table.table = new Entry[size];
		return Table;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size > lextable.max_size) throw ERROR_THROW(106);
		lextable.table[lextable.size++] = entry;
	}

	void Add(LexTable& lextable, Entry entry, int i)
	{
		if (lextable.size > lextable.max_size) throw ERROR_THROW(106);
		lextable.table[i] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}

	Entry writeEntry(Entry& entry, unsigned char lexema, int indx, int line) {
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		return entry;
	}

	void writeLexTable(std::ostream* stream, LT::LexTable& lextable)
	{
		*stream << "\n------------------------------ Таблица лексем (DEBUG) ------------------------\n" << endl;
		*stream << " Лексема | Строка | Индекс |" << endl;
		for (int i = 0; i < lextable.size; i++)
		{
			*stream << setw(5) << lextable.table[i].lexema << "    |  " << setw(3)
				<< lextable.table[i].sn << "   |";
			if (lextable.table[i].idxTI == LT_TI_NULLIDX)
				*stream << "        |" << endl;
			else
				*stream << setw(5) << lextable.table[i].idxTI << "   |" << endl;
		}
	}

	void showTable(LexTable lextable, ostream* fout) {
		*fout << "\n----------------- Поток лексем ---------------------";
		*fout << "\n01 ";

		int number = 1;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == '#') continue;

			if (lextable.table[i].sn != number && lextable.table[i].sn != -1)
			{
				if (lextable.table[i].sn < 10)
					*fout << endl << "0" << lextable.table[i].sn << " ";
				else
					*fout << endl << lextable.table[i].sn << " ";
				number = lextable.table[i].sn;
			}
			*fout << lextable.table[i].lexema;
		}
		*fout << endl;
	}

	
	void ShowPolishRaw(LexTable lextable, const IT::IdTable& idtable, std::ostream* fout)
	{
		*fout << "\n\n----------------- Декодированная Польская запись (ПОЛИЗ) ---------------------\n";
		*fout << "Номер строки | Выражение в ПОЛИЗ\n";
		*fout << "------------------------------------------------------------------------------\n";

		int currentLine = -1;
		bool expressionStarted = false;

		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == '#') continue;

			if (lextable.table[i].sn != currentLine) {
				if (currentLine != -1 && expressionStarted) *fout << "\n";

				currentLine = lextable.table[i].sn;
				*fout << std::setw(2) << std::setfill('0') << currentLine << "           | ";
				expressionStarted = false;
			}

			unsigned char lex = lextable.table[i].lexema;
			int idx = lextable.table[i].idxTI;

			switch (lex)
			{
			case LEX_ID:
				if (idx != LT_TI_NULLIDX) {
					const IT::Entry& entry = idtable.table[idx];
					// !!! ИСПРАВЛЕНИЕ: (char*) перед entry.id
					*fout << (char*)entry.id << " ";
				}
				else *fout << "ID??? ";
				expressionStarted = true;
				break;

			case LEX_LITERAL:
				if (idx != LT_TI_NULLIDX) {
					const IT::Entry& entry = idtable.table[idx];
					if (entry.iddatatype == IT::INT) *fout << entry.value.vint << " ";
					// !!! ИСПРАВЛЕНИЕ: (char*) перед строкой
					else if (entry.iddatatype == IT::STR) *fout << "\"" << (char*)entry.value.vstr.str << "\" ";
					else if (entry.iddatatype == IT::CHR) *fout << "'" << entry.value.vchar << "' ";
					else *fout << "LIT ";
				}
				expressionStarted = true;
				break;

			case LEX_OPERATOR:
			case LEX_LOGOPERATOR:
				if (idx != LT_TI_NULLIDX) {
					const IT::Entry& entry = idtable.table[idx];
					// !!! ИСПРАВЛЕНИЕ: (char*) перед entry.id
					*fout << (char*)entry.id << " ";
				}
				else *fout << "OP ";
				expressionStarted = true;
				break;

			case LEX_EQUAL: *fout << "= "; expressionStarted = true; break;
			case LEX_SEMICOLON: *fout << ";"; break;
			case '@': *fout << "CALL" << lextable.table[i].priority << " "; expressionStarted = true; break;
			case LEX_RETURN: *fout << "return "; expressionStarted = true; break;
			case LEX_COUT: *fout << "cout "; expressionStarted = true; break;

				// Добавил обработку IF/ELSE для красоты, если они попадут в таблицу (хотя в ПОЛИЗ их нет обычно)
			case LEX_IF: *fout << "if "; expressionStarted = true; break;
			case LEX_ELSE: *fout << "else "; expressionStarted = true; break;

			default: break;
			}
		}
		*fout << "\n------------------------------------------------------------------------------\n";
	}
}