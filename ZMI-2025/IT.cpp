#include "IT.h"
#include "Error.h"
#include "stdafx.h"
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace std;

namespace IT {

	static void printRow(const Entry& entry, ostream* log) {
		// Ограничиваем длину ID для вывода, чтобы не ехала таблица
		char idBuf[16];
		strncpy_s(idBuf, entry.id, 15);

		*log << setw(15) << left << idBuf << " | ";

		if (entry.idtype == OP || entry.idtype == LO) *log << setw(10) << left << "-" << " | ";
		else if (entry.iddatatype == INT)  *log << setw(10) << left << "int(1)" << " | ";
		else if (entry.iddatatype == STR)  *log << setw(10) << left << "string" << " | ";
		else if (entry.iddatatype == CHR)  *log << setw(10) << left << "char" << " | ";
		else if (entry.iddatatype == BOOL) *log << setw(10) << left << "bool" << " | ";
		else if (entry.iddatatype == VOI)  *log << setw(10) << left << "void" << " | ";
		else *log << setw(10) << left << "unk" << " | "; // Вот тут мы увидим, исправился ли unk!

		*log << setw(16) << left << entry.idxfirstLE << " | ";

		if (entry.idtype == L) {
			if (entry.iddatatype == INT) { *log << entry.value.vint; }
			else if (entry.iddatatype == CHR) { *log << "'" << entry.value.vchar << "'"; }
			else if (entry.iddatatype == STR) {
				// Обрезаем длинные строки
				char strBuf[30];
				if (entry.value.vstr.len > 25) {
					strncpy_s(strBuf, (char*)entry.value.vstr.str, 25);
					strcat_s(strBuf, "...");
					*log << strBuf;
				}
				else {
					*log << "[" << entry.value.vstr.len << "]" << entry.value.vstr.str;
				}
			}
		}
		else { *log << "-"; }
		*log << endl;
	}

	IdTable Create(int size) {
		if (size > TI_MAXSIZE) throw ERROR_THROW(105);
		IdTable Table; Table.max_size = size; Table.size = 0;
		Table.table = new Entry[size]; memset(Table.table, 0, sizeof(Entry) * size);
		return Table;
	}
	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size >= idtable.max_size) throw ERROR_THROW(106);
		idtable.table[idtable.size++] = entry;
	}
	Entry GetEntry(IdTable& idtable, int n) { return idtable.table[n]; }
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) {
		for (int i = 0; i < idtable.size; i++) if (strcmp(idtable.table[i].id, id) == 0) return i;
		return TI_NULLIDX;
	}
	void Delete(IdTable& idtable) { delete[] idtable.table; idtable.table = nullptr; idtable.size = 0; }

	void showITable(IdTable& table, ostream* log)
	{
		if (!table.table) return;
		*log << "\n\n------------------Таблица индетификаторов------------------\n\n";
		*log << " Идентификатор   | тип данных | первое вхождение | содержание \n";
		*log << "-----------------|------------|------------------|------------\n";

		const char* headers[] = { "Литерал", "Переменная", "Параметр", "Функция" };
		IDTYPE types[] = { L, V, P, F };

		for (int k = 0; k < 4; k++) {
			bool found = false;
			for (int i = 0; i < table.size; i++) if (table.table[i].idtype == types[k]) found = true;
			if (found) {
				*log << "_____" << headers[k] << "___________________________________________\n";
				for (int i = 0; i < table.size; i++) if (table.table[i].idtype == types[k]) printRow(table.table[i], log);
			}
		}
	}
}