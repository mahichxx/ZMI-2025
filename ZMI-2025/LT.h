#pragma once
#define LEXEMA_FIXSIZE	1			
#define LT_MAXSIZE		4096		
#define LT_TI_NULLIDX	0xfffffff	

// --- Типы данных и Литералы ---
#define LEX_INTEGER		't'	
#define LEX_STRING		's'	// !!! Поменял на 's' (было 't' - конфликт)
#define LEX_CHAR        'c' // !!! char
#define LEX_LITERAL		'l'	
#define LEX_TRUE        'T' // !!! true
#define LEX_FALSE       'F' // !!! false

// --- Ключевые слова ---
#define LEX_ID			'i'	
#define LEX_FUNCTION	'f'	
#define LEX_RETURN		'r'	
#define LEX_MAIN		'm'	
#define LEX_COUT		'o'	// !!! cout (output)
#define LEX_SWITCH      'h' // !!! switch (пусть будет h - cHoice)
#define LEX_CASE        'a' // !!! case (пусть будет a - cAse)
#define LEX_DEFAULT     'd' // !!! default

// --- Структурные символы ---
#define LEX_SEMICOLON	';'	
#define LEX_TWOPOINT	':'	
#define LEX_COMMA		','	
#define LEX_LEFTBRACE	'{'	
#define LEX_BRACELET	'}'	
#define LEX_LEFTTHESIS	'('	
#define LEX_RIGHTTHESIS	')'	

// --- Операторы ---
#define LEX_PLUS		'v'	
#define LEX_MINUS		'v'	
#define LEX_STAR		'v'	
#define LEX_DIRSLASH	'v'	
#define LEX_OPERATOR	'v'	

// !!! Логика и сравнения (Присвоим уникальные символы для упрощения разбора ПОЛИЗа)
// В оригинале все операторы сравнения были 'q', лучше их разделить для парсера,
// либо оставить 'q' если парсер смотрит поле op. Давай оставим стиль оригинала, 
// но добавим недостающие операции в enum.
#define LEX_LOGOPERATOR 'q' 

// Можно использовать конкретные буквы для отладки, но для парсера важен enum operations
#define LEX_EQUAL		'=' // Присваивание

// --- Условия ---
#define LEX_IF			'w' 
#define LEX_ELSE		'e' // !!! Если вдруг не было
#define LEX_VOID		'n' 
#define LEX_ENDIF		'|' 

#define TI_NULLIDX		0xffffffff

namespace LT		// таблица лексем
{
	enum operations {
		OPLUS = 1,
		OMINUS,
		OMUL,
		ODIV,
		OMOD,
		OEQ,
		ONE,
		OMORE,
		OLESS,
		OGE,
		OLE,
		OAND,
		OOR
	};
	struct Entry	// строка таблицы лексем
	{
		unsigned char lexema;	// лексема
		int sn;					// номер строки в исходном тексте
		int idxTI = TI_NULLIDX;				// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;
		operations op;
	};

	struct LexTable				// экземпляр таблицы лексем
	{
		int max_size;			// емкость таблицы лексем < LT_MAXSIZE
		int size;				// текущий размер таблицы лексем < max_size
		Entry* table;			// массив строк таблицы лексем
	};

	LexTable Create(		// создать таблицу лексем
		int size			// емкость таблицы лексем < LT_MAXSIZE
	);

	void Add(				// добавить строку в таблицу лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		Entry entry			// строка таблицы лексем
	);
	void Add(LexTable& lextable, Entry entry, int i);

	Entry GetEntry(			// получить строку таблицы лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		int n				// номер получаемой строки
	);

	void Delete(LexTable& lextable);	// удалить таблицу лексем (освободить память)

	Entry writeEntry(					// заполнить строку таблицы лексем
		Entry& entry,
		unsigned char lexema,
		int indx,
		int line
	);
	void writeLexTable(std::ostream* stream, LT::LexTable& lextable);
	void showTable(LexTable lextable, std::ostream* fout);	// вывод таблицы лексем

};