#pragma once
#define ID_MAXSIZE		32		// максимальное количество символов в идентификаторе
#define TI_MAXSIZE		4096		// максимальное количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00     	// значение по умолчанию для типа integer
#define TI_STR_DEFAULT	0x00		// значение по умолчанию для типа string
#define TI_NULLIDX		0xffffffff	// нет элкмента таблицы идентификаторов
#define TI_STR_MAXSIZE	255			// 

#define TABLE *log << setw(15) << left << table.table[i].id << "| ";\
    if (table.table[i].idtype == OP || table.table[i].idtype == LO) *log << setw(10) << left << "-" << " | ";\
    else if (table.table[i].iddatatype == INT) {*log << setw(10) << left << "int(1)" << " | ";}\
    else if (table.table[i].iddatatype == STR)  *log << setw(10) << left << "string" << " | ";\
    else if (table.table[i].iddatatype == CHR)  *log << setw(10) << left << "char" << " | ";\
    else if (table.table[i].iddatatype == BOOL) *log << setw(10) << left << "bool" << " | ";\
    else if (table.table[i].iddatatype == VOI) *log << setw(10) << left << "void" << " | ";\
    *log << setw(16) << left << table.table[i].idxfirstLE << " | ";\
    if (table.table[i].idtype == L) {\
        if (table.table[i].iddatatype == INT) {*log << table.table[i].value.vint;}\
        else if (table.table[i].iddatatype == CHR) {*log << "'" << table.table[i].value.vchar << "'";}\
        else if (table.table[i].iddatatype == BOOL) {*log << (table.table[i].value.vbool ? "true" : "false");}\
        else if (table.table[i].iddatatype == STR) *log << '[' << table.table[i].value.vstr.len << ']' << table.table[i].value.vstr.str;\
    }\
    else *log << "-";\
    *log << endl;\

#include "Parm.h"
#include "Log.h"
#include <iomanip> // Нужно для setw

namespace IT			// таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2, BOOL = 3, VOI = 4, CHR = 5, NUL = 6 };	// типы данных идентификаторов: integer, string, int16bit, без типпа данных, процедура, символ
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5, LO = 6 };	// типы идентификаторов: переменная, функция, параметр, литерал, оператор

	struct Entry	// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		unsigned char	id[ID_MAXSIZE];		// индентификатор (автоматически усекается до ID_MAXSIZE)
		IDDATATYPE	iddatatype = NUL;			// тип данных
		IDTYPE		idtype;				// тип идентификатора
		int parm = 0;
		int nums = 0; //0 - 10 СС, 1 - 16 СС, 2 - 2 СС (!!! добавим для Bin)
		union
		{
			int vint;					// значение integer
			char vchar;
			bool vbool;
			struct
			{
				int len;						// количество символов в string
				unsigned char str[TI_STR_MAXSIZE - 1];	// символы string
			} vstr;				// значение string
		} value;		// значение идентификатора
	};

	struct IdTable				// экземпляр таблицы идентификаторов
	{
		int max_size;			// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;				// текущий размер таблицы идентификаторов < maxsize
		Entry* table;			// массив строк таблицы идентификаторов
	};

	IdTable Create(				// создать таблицу идентификаторов
		int size				// емкость таблицы идентификаторов < TI_MAXSIZE
	);
	void Add(				// добавить строку в таблицу идентификаторов
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		Entry entry			// строка таблицы идентификаторов
	);
	void Add(IdTable& idtable, Entry entry, int i);

	Entry GetEntry(			// получить строку таблицы идентификаторов
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		int n				// номер получаемой строки
	);

	int IsId(				// возврат: номер строки (если есть), TI_NULLIDX (если нет)
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		unsigned char id[ID_MAXSIZE]	// идентификатор
	);

	void Delete(IdTable& idtable);	// удалить таблицу лексем (освободить память)

	void showITable(IdTable& table, ostream* log);	// вывод таблицы лексем


};