#pragma once
#define ID_MAXSIZE		32
#define TI_MAXSIZE		4096
#define TI_INT_DEFAULT	0
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255

#include "Log.h"
#include <iomanip>

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2, BOOL = 3, VOI = 4, CHR = 5, NUL = 6 };
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5, LO = 6 };

	struct Entry
	{
		int			idxfirstLE;
		char		id[ID_MAXSIZE]; // Исправлено на char
		IDDATATYPE	iddatatype = NUL;
		IDTYPE		idtype;
		int			parm = 0;   // Кол-во параметров (для функций)

		// 0 - DEC, 1 - HEX, 2 - BIN. Важно для твоего задания!
		// Нужно, чтобы в ASM записать 0FFh или 1010b, если хочешь сохранить стиль,
		// либо просто хранить значение и выводить dec.
		int			numSys = 0;

		union
		{
			int vint;      // Храним как int, но логически работаем как с int8
			char vchar;
			bool vbool;
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE - 1]; // Исправлено на char
			} vstr;
		} value;
	};

	struct IdTable
	{
		int max_size;
		int size;
		Entry* table;
	};

	IdTable Create(int size);
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]); // char
	void Delete(IdTable& idtable);
	void showITable(IdTable& table, std::ostream* log); // std::ostream для надежности
};