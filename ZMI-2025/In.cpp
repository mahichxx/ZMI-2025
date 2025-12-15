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

		int st = 0;
		int cl = 0;

		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
		memset(text, 0, IN_MAX_LEN_TEXT);

		ifstream fin(infile);
		if (fin.fail()) throw ERROR_THROW(110);

		fin.seekg(0, std::ios::end);
		long long fileSize = fin.tellg();
		fin.seekg(0, std::ios::beg);
		if (fileSize > IN_MAX_LEN_TEXT) throw ERROR_THROW(113);

		unsigned char quoteChar = 0;

		// !!! ИЗМЕНЕНИЕ: Вместо флага используем счетчик глубины !!!
		int commentDepth = 0;

		char c;
		while (in.size < IN_MAX_LEN_TEXT && fin.get(c))
		{
			unsigned char x = (unsigned char)c;

			// 1. Если встретили '[', увеличиваем глубину вложенности
			if (quoteChar == 0 && x == '[') {
				commentDepth++;
				continue; // Пропускаем сам символ '['
			}

			// 2. Если встретили ']', уменьшаем глубину
			if (quoteChar == 0 && x == ']') {
				if (commentDepth > 0) {
					commentDepth--;
					// Если мы вышли из самого верхнего комментария (глубина стала 0)
					if (commentDepth == 0) {
						// Разделяем слова пробелом, чтобы код не склеился
						if (in.word[st][0] != 0) {
							in.word[st][cl] = 0; st++; cl = 0;
						}
					}
					continue; // Пропускаем сам символ ']'
				}
				// Если встретили ']' без открывающей '[', это ошибка или разделитель?
				// В твоем языке это разделитель, пусть лексер или парсер разбирается, или игнорируем.
				// Но здесь мы считаем это просто символом, если глубина 0.
			}

			// 3. Если глубина > 0, значит мы внутри комментария — ИГНОРИРУЕМ ВСЁ
			if (commentDepth > 0) {
				if (x == IN_CODE_ENDL) in.lines++; // Но строки считаем!
				continue;
			}

			// --- ДАЛЬШЕ ОБЫЧНАЯ ОБРАБОТКА КОДА (depth == 0) ---

			if (in.code[x] == in.F) {
				throw ERROR_THROW_IN(111, in.lines, col);
			}

			if (x == IN_CODE_ENDL)
			{
				// Если мы дошли до конца строки, а кавычка открыта
				if (quoteChar != 0) {
					throw ERROR_THROW_IN(116, in.lines, col);
				}

				in.lines++; col = 0; quoteChar = 0;
				if (in.word[st][0] != 0) { in.word[st][cl] = 0; st++; cl = 0; }
				in.word[st][cl++] = '|'; in.word[st][cl] = 0; st++; cl = 0;
				text[in.size++] = x;
				continue;
			}

			if (st >= IN_MAX_WORDS - 1) throw ERROR_THROW(113);

			int type = in.code[x];

			if (type == in.I) {
				in.ignor++;
			}
			else if (type == in.P) {
				if (quoteChar == 0) {
					quoteChar = x;
					if (in.word[st][0] != 0) { in.word[st][cl] = 0; st++; cl = 0; }
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
			else if (type == in.S) {
				if (quoteChar != 0) {
					if (cl < 255) in.word[st][cl++] = x;
				}
				else {
					if (in.word[st][0] != 0) { in.word[st][cl] = 0; st++; cl = 0; }
				}
				text[in.size++] = x; col++;
			}
			else {
				if (quoteChar != 0) {
					if (cl < 255) in.word[st][cl++] = x;
				}
				else {
					if (type == in.T) {
						if (cl < 255) in.word[st][cl++] = x;
					}
					else {
						if (in.word[st][0] != 0) { in.word[st][cl] = 0; st++; cl = 0; }
						in.word[st][cl++] = x; in.word[st][cl] = 0; st++; cl = 0;
					}
				}
				text[in.size++] = x; col++;
			}
		}

		// !!! ПРОВЕРКА НА НЕЗАКРЫТЫЙ КОММЕНТАРИЙ !!!
		if (commentDepth > 0) {
			throw ERROR_THROW(115);
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