#pragma once
#define IN_MAX_LEN_TEXT 1024 * 1024
#define IN_MAX_WORDS    10240
#define IN_CODE_ENDL '\n'

namespace In {
	struct IN;
}

// Таблица допустимых символов.
// 0 - Разделитель/Спецсимвол (прерывает слово)
// IN::T - Текст (накапливается в слово)
// IN::F - Запрещен (Ошибка 111)
// IN::P - Кавычки
// IN::S - Пробелы
#define IN_CODE_TABLE {\
/*         0      1      2       3     4    5   6      7      8      9      A      B      C     D      E      F*/\
/*0*/	IN::F,	IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F,	IN::S, '|',   IN::F, IN::F,	IN::F, IN::F, IN::F,\
/*1*/	IN::F,	IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F, IN::F,	IN::F, IN::F, IN::F,\
/*2*/	IN::S,	IN::T, IN::P, 0,     IN::F,	0,     0,     IN::P, 0,	    0,     0,     0,     0,	    0,     0,     0,\
/*3*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, 0,     0,     0,	    0,     0,     0,\
/*4*/	0,	    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,\
/*5*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, 0,     0,	    0,     0,     IN::T,\
/*6*/	0,      IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,\
/*7*/	IN::T,	IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, 0,     0,     0,     0,     IN::F,\
                                                                                 \
/*8*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*9*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*A*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*B*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*C*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*D*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*E*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T,\
/*F*/	IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T, IN::T,	IN::T, IN::T, IN::T\
}

namespace In
{
	struct IN
	{
		enum { T = 1024, F = 2048, I = 4096, S = 8192, P = 43532 };
		int size;
		int lines;
		int ignor;
		unsigned char* text;
		int code[256] = IN_CODE_TABLE;
		unsigned char** word;
	};

	IN getin(wchar_t infile[]);
	void Delete(IN& in);
}