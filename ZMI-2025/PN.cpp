#include "stdafx.h"
#include "PN.h"
#include <stack>
#include <queue>
#include <iostream>
#include <iomanip>

using namespace std;

namespace Polish {

	// Приоритеты операций (согласно лекции №13)
	int GetPriority(LT::operations op) {
		switch (op) {
		case LT::operations::OEQ:       // ==
		case LT::operations::ONE:       // !=
		case LT::operations::OMORE:     // >
		case LT::operations::OLESS:     // <
		case LT::operations::OGE:       // >=
		case LT::operations::OLE:       // <=
			return 1;
		case LT::operations::OPLUS:     // +
		case LT::operations::OMINUS:    // -
			return 2;
		case LT::operations::OMUL:      // *
		case LT::operations::ODIV:      // /
		case LT::operations::OMOD:      // %
			return 3;
		default:
			return 0;
		}
	}

	// Лабораторная 20: Функция PolishNotation
	// Изменяет таблицу лексем lextable, записывая туда ПОЛИЗ
	bool PolishNotation(int i, Lex::LEX& lex, char terminator = LEX_SEMICOLON)
	{
		std::stack<LT::Entry> stack;   // Стек для операторов
		std::queue<LT::Entry> queue;   // Очередь для выходной строки (ПОЛИЗ)

		// Лексема-заполнитель для "хвоста" (пустое место)
		LT::Entry placeholder_symbol;
		placeholder_symbol.idxTI = LT_TI_NULLIDX;
		placeholder_symbol.lexema = '#';
		placeholder_symbol.sn = lex.lextable.table[i].sn;

		// Лексема для вызова функции (@)
		LT::Entry function_symbol;
		function_symbol.idxTI = LT_TI_NULLIDX;
		function_symbol.lexema = '@';
		function_symbol.sn = lex.lextable.table[i].sn;

		int idx_func = LT_TI_NULLIDX; // Индекс ID функции в таблице идентификаторов
		int lexem_counter = 0;        // Сколько лексем обработали
		int parm_counter = 0;         // Счетчик параметров функции
		int lexem_position = i;       // Запоминаем позицию начала выражения
		bool findFunc = false;        // Флаг: сейчас обрабатываем аргументы функции

		// 1. Проход по выражению (Алгоритм Дейкстры)
		for (i; i < lex.lextable.size; i++, lexem_counter++)
		{
			// Если встретили терминатор (';' или ')' для switch) и стек пуст (или баланс скобок 0)
			if (lex.lextable.table[i].lexema == terminator) {
				if (terminator == LEX_RIGHTTHESIS && !stack.empty() && stack.top().lexema == LEX_LEFTTHESIS) {
					// Это внутренняя закрывающая скобка, идем дальше
				}
				else {
					break; // Конец выражения
				}
			}

			switch (lex.lextable.table[i].lexema)
			{
				// ОПЕРАНДЫ (Переменные и Литералы) -> Сразу в очередь
			case LEX_ID:
			case LEX_LITERAL:
				// Проверка: это просто переменная или вызов функции?
				// Если за ID идет '(', значит это функция
				if (lex.lextable.table[i].lexema == LEX_ID &&
					lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS)
				{
					// Это имя функции. Запоминаем его, но в ПОЛИЗ пока не пишем.
					// В ПОЛИЗ оно пойдет в конце как оператор '@'
					idx_func = lex.lextable.table[i].idxTI;
					findFunc = true;
					parm_counter = 0; // Начинаем считать параметры
					// ВАЖНО: Имя функции пропускаем, оно "превратится" в оператор @
				}
				else
				{
					// Обычная переменная или число
					queue.push(lex.lextable.table[i]);
					// Если мы внутри функции, то это аргумент (если только это не запятая)
					if (findFunc) {
						// Логика подсчета параметров сложнее (зависит от запятых),
						// поэтому лучше считать параметры по запятым + 1
					}
				}
				continue;

				// ОТКРЫВАЮЩАЯ СКОБКА
			case LEX_LEFTTHESIS:
				stack.push(lex.lextable.table[i]);
				// Если это начало вызова функции (предыдущий был ID функции), 
				// то мы уже взвели флаг findFunc.
				// Если внутри скобок функции нет аргументов "func()", счетчик останется 0
				if (findFunc && lex.lextable.table[i + 1].lexema != LEX_RIGHTTHESIS) {
					parm_counter = 1; // Есть как минимум 1 параметр
				}
				continue;

				// ЗАКРЫВАЮЩАЯ СКОБКА
			case LEX_RIGHTTHESIS:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
				}

				if (!stack.empty()) stack.pop(); // Выкидываем '(' из стека

				// Если это закрылась скобка вызова функции
				if (findFunc && stack.empty()) // (упрощенная логика вложенности)
				{
					// Генерируем оператор вызова @
					// В поле idxTI положим индекс имени функции (чтобы генератор знал, кого звать)
					// В поле priority положим количество параметров
					LT::Entry callOp = function_symbol;
					callOp.idxTI = idx_func;
					callOp.priority = parm_counter;

					queue.push(callOp); // Добавляем @ в ПОЛИЗ

					findFunc = false;
					idx_func = LT_TI_NULLIDX;
				}
				continue;

				// ОПЕРАТОРЫ (+, -, *, /...)
			case LEX_OPERATOR:
			case LEX_LOGOPERATOR:
			case LEX_EQUAL:
				while (!stack.empty() &&
					stack.top().lexema != LEX_LEFTTHESIS &&
					GetPriority(lex.lextable.table[i].op) <= GetPriority(stack.top().op))
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;

				// ЗАПЯТАЯ (Разделитель аргументов)
			case LEX_COMMA:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					queue.push(stack.top());
					stack.pop();
				}
				if (findFunc) parm_counter++;
				continue;
			}
		}

		// Выталкиваем оставшиеся операторы
		while (!stack.empty())
		{
			if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
				return false; // Ошибка скобок
			queue.push(stack.top());
			stack.pop();
		}

		// 2. Перезапись таблицы лексем (Требование методички п.4)
		// Заменяем исходное выражение на ПОЛИЗ
		while (lexem_counter != 0)
		{
			if (!queue.empty())
			{
				lex.lextable.table[lexem_position++] = queue.front();
				queue.pop();
			}
			else
			{
				// Заполняем "хвост" заглушками
				lex.lextable.table[lexem_position++] = placeholder_symbol;
			}
			lexem_counter--;
		}

		return true;
	}

	bool StartPolish(Lex::LEX& lex)
	{
		bool flag = false;
		for (int i = 0; i < lex.lextable.size; i++)
		{
			// Ищем места, где нужно применить ПОЛИЗ

			// 1. Присваивание: id = ...
			if (lex.lextable.table[i].lexema == LEX_EQUAL)
			{
				// Начинаем ПОЛИЗ сразу после '='
				flag = PolishNotation(i + 1, lex, LEX_SEMICOLON);
				if (!flag) return false;
			}

			// 2. Return ...
			else if (lex.lextable.table[i].lexema == LEX_RETURN) {
				flag = PolishNotation(i + 1, lex, LEX_SEMICOLON);
				if (!flag) return false;
			}

			// 3. Cout ...
			else if (lex.lextable.table[i].lexema == LEX_COUT) {
				// Пропускаем 'cout' (он сам не часть выражения)
				flag = PolishNotation(i + 1, lex, LEX_SEMICOLON);
				if (!flag) return false;
			}

			// 4. Switch ( ... )
			else if (lex.lextable.table[i].lexema == LEX_SWITCH) {
				// switch ( expr ) -> преобразуем expr
				// i=switch, i+1=(, i+2=expr
				if (lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					flag = PolishNotation(i + 2, lex, LEX_RIGHTTHESIS);
					if (!flag) return false;
				}
			}
		}
		return true;
	}
}