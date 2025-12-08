#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

extern "C"
{
	int __stdcall outnum(int value)
	{
		cout << value;
		return 0;
	}

	int __stdcall outstr(char* value)
	{
		if (value == nullptr) {
			cout << "NULL_PTR";
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

	// --- ÎÒËÀÄÎ×ÍÛÅ ÂÅÐÑÈÈ ÔÓÍÊÖÈÉ ---

	int __stdcall strtoint(char* buffer)
	{
		if (buffer == nullptr) return 0;
		int result = std::atoi(buffer);

		// ÎÒËÀÄÊÀ: ×òî âîçâðàùàåì
		return result;
	}

	int __stdcall stcmp(char* str1, char* str2)
	{
		if (str1 == nullptr || str2 == nullptr) return -1;

		int res = std::strcmp(str1, str2);
		return res;
	}
}