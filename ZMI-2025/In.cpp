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

		// Выделяем память под слова ЛОКАЛЬНО для структуры
		in.word = new unsigned char* [IN_MAX_WORDS];
		for (int i = 0; i < IN_MAX_WORDS; i++)
			in.word[i] = new unsigned char[256] {0}; // Макс длина слова 255 + null

		int st = 0; // индекс текущего слова
		int cl = 0; // индекс символа в текущем слове

		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
		memset(text, 0, IN_MAX_LEN_TEXT);

		ifstream fin(infile);
		if (fin.fail()) throw ERROR_THROW(110);

		// ПРАВИЛЬНАЯ проверка размера файла
		fin.seekg(0, std::ios::end);
		long long fileSize = fin.tellg();
		fin.seekg(0, std::ios::beg);
		if (fileSize > IN_MAX_LEN_TEXT) throw ERROR_THROW(113);

		bool fkov = false;

		// Читаем посимвольно (noskipws чтобы читать пробелы)
		char c;
		while (in.size < IN_MAX_LEN_TEXT && fin.get(c))
		{
			unsigned char x = (unsigned char)c;

			if (x == IN_CODE_ENDL)
			{
				in.lines++;
				col = 0;
			}

			// Проверка на переполнение массива слов
			if (st >= IN_MAX_WORDS - 1) {
				// Можно выбросить ошибку или просто остановиться
				throw ERROR_THROW(113); // Код ошибки: слишком большой файл/много лексем
			}

			if (in.code[x] == in.T)
			{
				text[in.size] = x;
				if (cl < 255) in.word[st][cl++] = x;
				in.size++;
				col++;
			}
			else if (in.code[x] == in.I)
			{
				in.ignor++;
			}
			else if (in.code[x] == in.F)
			{
				throw ERROR_THROW_IN(111, in.lines, col);
			}
			else if (in.code[x] == in.P) {
				fkov = !fkov;
				text[in.size] = x;
				in.size++;

				if (fkov == true) { // Открывающая скобка/кавычка
					if (in.word[st][0] != 0) {
						st++; cl = 0;
					}
					in.word[st][cl++] = x;
				}
				else { // Закрывающая
					if (cl < 255) in.word[st][cl++] = x;
					st++; cl = 0;
				}
			}
			else if (in.code[x] == in.S) {
				if (fkov) {
					text[in.size] = x;
					if (cl < 255) in.word[st][cl++] = x;
					in.size++;
					col++;
					continue;
				}

				// Логика пропуска пробелов
				if (((in.size == 0) || in.code[text[in.size - 1]] == in.S) && (x == ' '))
				{
					continue;
				}
				if (in.size > 0 && text[in.size - 1] == ' ' && x == ' ') {
					// Если предыдущий был пробел и этот пробел - откатываемся?
					// Оставим твою логику, но она странная
					in.size = in.size - 1;
				}

				if (x != ' ') {
					if (in.word[st][0] != 0) {
						st++; cl = 0;
					}
					if (cl < 255) in.word[st][cl++] = x;
					st++; cl = 0;
				}
				else {
					if (in.word[st][0] != 0) {
						st++; cl = 0;
					}
				}
				text[in.size] = x;
				in.size++;
				col++;
			}
			else // Символы, не попавшие в T, I, F, P, S (обычно разделители, которые сами по себе токены)
			{
				if (x == '\n') fkov = false;
				if (in.word[st][0] != 0) {
					st++; cl = 0;
				}
				if (st < IN_MAX_WORDS) {
					in.word[st][cl++] = in.code[x]; // Возможно, тут ты хотел x, а не code[x]?
					// В оригинале было code[x], но code[x] возвращает тип (int 2048, 4096...), 
					// а записываем мы в char. Скорее всего тут ошибка логики, но оставлю как было, 
					// если у тебя в таблице на пересечении стоит сам символ.
					// СУДЯ ПО ТАБЛИЦЕ: для '|' стоит '|'. Ок.
				}

				st++; cl = 0;
				text[in.size] = in.code[x]; // Тоже вопрос, надо ли code[x] или x
				in.size++;
				col++;
			}
		}

		// Завершающий ноль для текста
		text[in.size] = '\0';
		in.text = text;

		// Завершающий NULL-ptr для массива слов, чтобы лексер знал где конец
		if (st < IN_MAX_WORDS) {
			delete[] in.word[st + 1]; // удалим лишнее выделение, если хотим, но проще занулить указатель
			in.word[st + 1] = NULL;
		}

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