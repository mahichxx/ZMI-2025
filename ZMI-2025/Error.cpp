#include "stdafx.h"
#include "Error.h"
#include <iostream>

using namespace std;

namespace Error {
	ERROR errors[ERROR_MAX_ENTRY] = {
		ERROR_ENTRY(0,"Недопустимый код ошибки"),
		ERROR_ENTRY(1,"Системный сбой"),
		ERROR_ENTRY_NODEF(2),ERROR_ENTRY_NODEF(3),ERROR_ENTRY_NODEF(4),ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6),ERROR_ENTRY_NODEF(7),ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60),ERROR_ENTRY_NODEF10(70),ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),

		// --- 100: Ошибки параметров ---
		ERROR_ENTRY(100,"Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра"),
		ERROR_ENTRY(105, "Ошибка слишком большой размер таблицы лексем/идентификаторов"),
		ERROR_ENTRY(106, "Размер таблицы лексем/идентификаторов превысил максимальный возможный"),
		ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113, "Ошибка лексического анализа"),
		ERROR_ENTRY(114, "Ошибка при создании выходного файла (-out)"),
		ERROR_ENTRY(115, "Ошибка: Незакрытый комментарий (отсутствует ']')"), ERROR_ENTRY(116, "Ошибка: Незакрытый строковый литерал (ожидалась кавычка перед переходом на новую строку)"), 
		ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		// --- 200: Лексические ошибки ---
		ERROR_ENTRY(200, "Лексический анализатор: Неинициализированная переменная"),
		ERROR_ENTRY(201, "Лексический анализатор: Неверное имя идентификатора"),
		ERROR_ENTRY(202, "Лексический анализатор: Превышен допустимый размер литерала"),
		ERROR_ENTRY(203, "Лексический анализатор: Превышен диапазон числового литерала (-128...127)"),
		ERROR_ENTRY(204, "Лексический анализатор: Запрещенное имя идентификатора"),
		ERROR_ENTRY(205, "Лексический анализатор: Запрещенный символ (') в строке"),

		ERROR_ENTRY(206, "Лексический анализатор: Использование кириллицы в идентификаторах запрещено"),ERROR_ENTRY(207, "Лексический анализатор: Неверный символьный литерал (ожидался формат 'c')"),
		ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),

		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230),
		ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260),
		ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),

		// --- 300: Семантические ошибки ---
		ERROR_ENTRY_NODEF(300),
		ERROR_ENTRY(301, "Семантическая ошибка: Отсутствует точка входа box"),
		ERROR_ENTRY(302, "Семантическая ошибка: Обнаружено несколько точек входа box"),
		ERROR_ENTRY_NODEF(303),
		ERROR_ENTRY_NODEF(304),
		ERROR_ENTRY(305, "Семантическая ошибка: Попытка переопределения идентификатора"),
		ERROR_ENTRY_NODEF(306),
		ERROR_ENTRY(307, "Семантическая ошибка: Кол-во параметров превышает максимальное значение"),
		ERROR_ENTRY(308, "Семантическая ошибка: Кол-во ожидаемых и переданных параметров не совпадает"),
		ERROR_ENTRY(309, "Семантическая ошибка: Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(310, "Семантическая ошибка: Использование пустого строкового литерала недопустимо"),
		ERROR_ENTRY(311, "Семантическая ошибка: Не закрыт строковый литерал"),
		ERROR_ENTRY_NODEF(312),
		ERROR_ENTRY(313, "Семантическая ошибка: Недопустимый размер целочисленного литерала"),
		ERROR_ENTRY(314, "Семантическая ошибка: Типы данных в выражении не совпадают"),
		ERROR_ENTRY(315, "Семантическая ошибка: Тип функции и возвращаемого значения не совпадают"),
		ERROR_ENTRY(316, "Семантическая ошибка: Недопустимое строковое выражение"),
		ERROR_ENTRY(317, "Семантическая ошибка: Неверное условное выражение"),
		ERROR_ENTRY(318, "Семантическая ошибка: Попытка деления на ноль"),
		ERROR_ENTRY(319, "Семантическая ошибка: Оператор check обязан содержать ветку else"),

		ERROR_ENTRY_NODEF10(320),ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		// --- 600: Синтаксические ошибки ---
		ERROR_ENTRY(600, "Синтаксическая ошибка: Неверная структура программы"),
		ERROR_ENTRY(601, "Синтаксическая ошибка: Ошибка в теле функции"),
		ERROR_ENTRY(602, "Синтаксическая ошибка: Ошибка в выражении"),
		ERROR_ENTRY(603, "Синтаксическая ошибка: Ошибка в параметрах функции"),
		ERROR_ENTRY(604, "Синтаксическая ошибка: Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY_NODEF(605),
		ERROR_ENTRY(606, "Синтаксическая ошибка: Неверная структура условия"),
		ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) {
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			cout << "\nОшибка " << id << ": " << errors[id].message << endl;
			return errors[id];
		}
		else {
			cout << "\nОшибка " << 0 << ": " << errors[0].message << endl;
			return errors[0];
		}
	}

	ERROR geterrorin(int id, int line = -1, int col = -1) {
		ERROR e;
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			e = errors[id];
			e.inext.col = col;
			e.inext.line = line;
			cout << "\nОшибка " << e.id << ": " << e.message << " cтрока " << e.inext.line << " позиция " << e.inext.col << endl;
			return e;
		}
		else {
			cout << "\nОшибка " << 0 << ": " << errors[0].message << endl;
			return errors[0];
		}
	}
}