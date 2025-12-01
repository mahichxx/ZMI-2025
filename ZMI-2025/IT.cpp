#include "IT.h"
#include "Error.h"
#include "stdafx.h"

namespace IT {
	IdTable Create(int size) {
		if (size > TI_MAXSIZE) throw ERROR_THROW(105);
		IdTable Table;
		Table.max_size = size;
		Table.size = 0;
		Table.table = new Entry[size];
		return Table;
	}
	Entry Create(Entry ITEntry)
	{
		return ITEntry;
	}
	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size > idtable.max_size) throw ERROR_THROW(106);
		idtable.table[idtable.size++] = entry;
	}
	void Add(IdTable& idtable, Entry entry, int i) {
		if (idtable.size > idtable.max_size) throw ERROR_THROW(106);
		idtable.table[i] = entry;
	}
	Entry GetEntry(IdTable& idtable, int n) {
		return idtable.table[n];
	}
	int IsId(IdTable& idtable, unsigned char id[ID_MAXSIZE]) {
		bool fl = true;
		for (int i = 0; i < idtable.size; i++) {
			if (_mbscmp(idtable.table[i].id, id) == 0)
				return i;
		}
		return TI_NULLIDX;
	}
	void Delete(IdTable& idtable) {
		delete[] idtable.table;

	}

	void showITable(IdTable& table, ostream* log)
	{
		bool lit = false, var = false, param = false, func = false, op = false, logic = false;
		*log << "\n\n------------------Таблица индетификаторов------------------\n\n";
		*log << " Идентификатор | тип данных | первое вхождение | содержание \n";
		for (int i = 0; i < table.size; i++) {
			switch (table.table[i].idtype)
			{
			case L: { lit = true; break; }
			case V: { var = true; break; }
			case P: { param = true; break; }
			case F: { func = true; break; }
			case OP: { op = true; break; }
			case LO: { logic = true; break; }
			default:
				break;
			}
		}
		if (lit) {
			*log << "_____Литерал_______________________________________________\n";
			for (int i = 0; i < table.size; i++) {
				if (table.table[i].idtype == L) {
					TABLE;
				}
			}
		}
		if (var) {
			*log << "_____Переменная____________________________________________\n";
			for (int i = 0; i < table.size; i++) {
				if (table.table[i].idtype == V) {
					TABLE;
				}
			}
		}
		if (param) {
			*log << "_____Параметр_____________________________________________\n";
			for (int i = 0; i < table.size; i++) {
				if (table.table[i].idtype == P) {
					TABLE;
				}
			}
		}
		if (func) {
			*log << "_____Функция_______________________________________________\n";
			for (int i = 0; i < table.size; i++) {
				if (table.table[i].idtype == F) {
					TABLE;
				}
			}
		}
		if (op) {
			*log << "_____Оператор______________________________________________\n";
			for (int i = 0; i < table.size; i++) {
				if (table.table[i].idtype == OP) {
					TABLE;
				}
			}
		}
		if (logic) {
			*log << "_____Логический оператор___________________________________\n";
			for (int i = 0; i < table.size; i++) {
				if (table.table[i].idtype == LO) {
					TABLE;
				}
			}
		}
	}
}