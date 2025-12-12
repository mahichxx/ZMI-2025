#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime> // Для time()

using namespace std;

extern "C"
{
	// --- Базовые функции ---
	int __stdcall outnum(int value)
	{
		cout << value;
		return 0;
	}

	int __stdcall outstr(char* value)
	{
		if (value == nullptr) {
			cout << "NULL";
			return 0;
		}
		setlocale(LC_ALL, "Russian");
		cout << value;
		return 0;
	}

	int __stdcall newline()
	{
		cout << endl;
		return 0;
	}

	// --- Старые функции ---
	int __stdcall strtoint(char* buffer)
	{
		if (buffer == nullptr) return 0;
		return std::atoi(buffer);
	}

	int __stdcall stcmp(char* str1, char* str2)
	{
		if (str1 == nullptr || str2 == nullptr) return 0;
		return std::strcmp(str1, str2);
	}

	// --- НОВЫЕ ФУНКЦИИ ---

	// 1. Длина строки
	int __stdcall strle(char* str)
	{
		if (str == nullptr) return 0;
		int len = std::strlen(str);
		// Ограничиваем 127, чтобы влезло в byte
		if (len > 127) return 127;
		return len;
	}

	// 2. Модуль числа
	int __stdcall mabs(int value)
	{
		return std::abs(value);
	}

	// 3. Случайное число от 0 до (range-1)
	int __stdcall rnd(int range)
	{
		if (range <= 0) return 0;
		// Простая инициализация (можно убрать, если не нужна псевдослучайность)
		static bool seeded = false;
		if (!seeded) {
			std::srand(std::time(nullptr));
			seeded = true;
		}
		return std::rand() % range;
	}
}