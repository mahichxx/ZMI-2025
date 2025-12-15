#pragma once
#include "stdafx.h"
#include <iostream>
#include <iomanip>

#define LEXEMA_FIXSIZE	1			
#define LT_MAXSIZE		4096		
#define LT_TI_NULLIDX	0xffffffff	

#define LEX_INTEGER		't'	
#define LEX_STRING		's'	
#define LEX_CHAR        'c' 
#define LEX_LITERAL		'l'	
#define LEX_TRUE        'T' 
#define LEX_FALSE       'F' 

#define LEX_ID			'i'	
#define LEX_FUNCTION	'f'	
#define LEX_RETURN		'r'	
#define LEX_MAIN		'm'	
#define LEX_COUT		'o'	
#define LEX_SWITCH      'h' 
#define LEX_CASE        'a' 
#define LEX_DEFAULT     'd' 

#define LEX_SEMICOLON	';'	
#define LEX_TWOPOINT	':'	
#define LEX_COMMA		','	
#define LEX_LEFTBRACE	'{'	
#define LEX_BRACELET	'}'	
#define LEX_LEFTTHESIS	'('	
#define LEX_RIGHTTHESIS	')'	

#define LEX_PLUS		'v'	
#define LEX_MINUS		'v'	
#define LEX_STAR		'v'	
#define LEX_DIRSLASH	'v'	
#define LEX_OPERATOR	'v'	

#define LEX_LOGOPERATOR 'q' 
#define LEX_EQUAL		'=' 

#define LEX_IF			'w' 
#define LEX_ELSE		'e' 
#define LEX_VOID		'n' 
#define LEX_ENDIF		'|' 

#define TI_NULLIDX		0xffffffff

namespace IT { struct IdTable; }

namespace LT
{
	enum operations {
		OPLUS = 1, OMINUS, OMUL, ODIV, OMOD,
		OEQ, ONE, OMORE, OLESS, OGE, OLE,
		OLESS_CMP, 
		OAND, OOR
	};

	struct Entry
	{
		unsigned char lexema;
		int sn;
		int idxTI = TI_NULLIDX;
		int priority;
		operations op;
	};

	struct LexTable
	{
		int max_size;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	void Add(LexTable& lextable, Entry entry, int i);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);

	Entry writeEntry(Entry& entry, unsigned char lexema, int indx, int line);
	void writeLexTable(std::ostream* stream, LT::LexTable& lextable);
	void showTable(LexTable lextable, std::ostream* fout);

	void ShowPolishRaw(LexTable lextable, const IT::IdTable& idtable, std::ostream* fout);
};