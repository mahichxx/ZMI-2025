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
		char		id[ID_MAXSIZE];
		IDDATATYPE	iddatatype = NUL;
		IDTYPE		idtype;
		int			parm = 0;   

		int			numSys = 0;

		union
		{
			int vint;     
			char vchar;
			bool vbool;
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE - 1]; 
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
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);
	void Delete(IdTable& idtable);
	void showITable(IdTable& table, std::ostream* log); 
};