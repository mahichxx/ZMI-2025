#include <iostream>
#include <cstdlib> // Для atoi и strcmp
#include <cstring>

using namespace std;

extern "C"
{
	// --- Стандартные функции вывода (используются генератором) ---

	// Вывод числа (1 байт и 4 байта)
	int __stdcall outnum(int value)
	{
		cout << value;
		return 0;
	}

	// Вывод строки
	int __stdcall outstr(char* value)
	{
		if (value == nullptr) return 0;
		setlocale(LC_ALL, "Russian");
		cout << value;
		return 0;
	}

	// Перевод строки (endl)
	int __stdcall newline()
	{
		cout << endl;
		return 0;
	}

	// --- ТВОИ ФУНКЦИИ ПО ВАРИАНТУ ---

	// 1. Преобразование строки в число
	// В коде на твоем языке будет: t = strtoint(s);
	int __stdcall strtoint(char* buffer)
	{
		if (buffer == nullptr) return 0;
		return std::atoi(buffer);
	}

	// 2. Сравнение двух строк
	// Возвращает 0, если равны, и не 0, если разные (как strcmp)
	// В коде: if (stcmp(s1, s2) == 0) ...
	int __stdcall stcmp(char* str1, char* str2)
	{
		if (str1 == nullptr || str2 == nullptr) return -1;
		return std::strcmp(str1, str2);
	}
}