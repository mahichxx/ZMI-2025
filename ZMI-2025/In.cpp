#include "stdafx.h"
#include "Parm.h"
#include "Error.h"
#include "In.h"
#include <locale>	
#include <cwchar>
#include <iostream>
#include <fstream>

using namespace std;

namespace In
{
	IN getin(wchar_t infile[])
	{
		IN in;
		in.size = 0; in.lines = 0; in.ignor = 0;
		int col = 0;

		in.word = new unsigned char* [IN_MAX_WORDS];
		for (int i = 0; i < IN_MAX_WORDS; i++)
			in.word[i] = new unsigned char[256] {0};

		int st = 0; // индекс слова
		int cl = 0; // индекс символа в слове

		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
		memset(text, 0, IN_MAX_LEN_TEXT);

		ifstream fin(infile);
		if (fin.fail()) throw ERROR_THROW(110);

		fin.seekg(0, std::ios::end);
		long long fileSize = fin.tellg();
		fin.seekg(0, std::ios::beg);
		if (fileSize > IN_MAX_LEN_TEXT) throw ERROR_THROW(113);

		unsigned char quoteChar = 0;
		bool inComment = false; // Флаг: находимся ли мы внутри комментария [ ... ]

		char c;
		while (in.size < IN_MAX_LEN_TEXT && fin.get(c))
		{
			unsigned char x = (unsigned char)c;

			// 1. Обработка начала комментария '['
			// Если мы не в строке и не в комментарии
			if (!inComment && quoteChar == 0 && x == '[') {
				inComment = true;
				continue; // Пропускаем сам символ '['
			}

			// 2. Обработка конца комментария ']'
			if (inComment && x == ']') {
				inComment = false;
				// Заменяем комментарий на пробел, чтобы слова не склеились
				// Например: word[comment]word -> word word
				if (in.word[st][0] != 0) {
					in.word[st][cl] = 0; st++; cl = 0;
				}
				continue; // Пропускаем символ ']'
			}

			// 3. Если мы внутри комментария — ИГНОРИРУЕМ ВСЁ
			if (inComment) {
				// Можно считать переносы строк внутри комментов, чтобы не сбивался счетчик строк
				if (x == IN_CODE_ENDL) in.lines++;
				continue;
			}

			// --- Дальше стандартная логика (если не в комментарии) ---

			if (x == IN_CODE_ENDL)
			{
				in.lines++;
				col = 0;
				quoteChar = 0;
				if (in.word[st][0] != 0) {
					in.word[st][cl] = 0; st++; cl = 0;
				}
				in.word[st][cl++] = '|';
				in.word[st][cl] = 0; st++; cl = 0;
				text[in.size++] = x;
				continue;
			}

			if (st >= IN_MAX_WORDS - 1) throw ERROR_THROW(113);

			int type = in.code[x];

			if (type == in.F) {
				throw ERROR_THROW_IN(111, in.lines, col);
			}
			else if (type == in.I) {
				in.ignor++;
			}
			else if (type == in.P) { // Кавычки
				if (quoteChar == 0) {
					quoteChar = x;
					if (in.word[st][0] != 0) {
						in.word[st][cl] = 0; st++; cl = 0;
					}
					in.word[st][cl++] = x;
				}
				else {
					if (quoteChar == x) {
						if (cl < 255) in.word[st][cl++] = x;
						in.word[st][cl] = 0; st++; cl = 0;
						quoteChar = 0;
					}
					else {
						if (cl < 255) in.word[st][cl++] = x;
					}
				}
				text[in.size++] = x; col++;
			}
			else if (type == in.S) { // Пробелы
				if (quoteChar != 0) {
					if (cl < 255) in.word[st][cl++] = x;
				}
				else {
					if (in.word[st][0] != 0) {
						in.word[st][cl] = 0; st++; cl = 0;
					}
				}
				text[in.size++] = x; col++;
			}
			else { // Обычные символы
				if (quoteChar != 0) {
					if (cl < 255) in.word[st][cl++] = x;
				}
				else {
					if (type == in.T) {
						if (cl < 255) in.word[st][cl++] = x;
					}
					else {
						if (in.word[st][0] != 0) {
							in.word[st][cl] = 0; st++; cl = 0;
						}
						in.word[st][cl++] = x;
						in.word[st][cl] = 0; st++; cl = 0;
					}
				}
				text[in.size++] = x; col++;
			}
		}

		text[in.size] = '\0';
		in.text = text;
		if (st < IN_MAX_WORDS) in.word[st + 1] = NULL;

		fin.close();
		return in;
	}

	void Delete(IN& in) {
		delete[] in.text;
		if (in.word) {
			for (int i = 0; i < IN_MAX_WORDS; i++) {
				if (in.word[i]) delete[] in.word[i];
			}
			delete[] in.word;
			in.word = nullptr;
		}
	}
}