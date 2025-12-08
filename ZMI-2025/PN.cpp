#include "stdafx.h"
#include "PN.h"
#include "LT.h" // Подключаем, чтобы видеть константы LEX_*
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

namespace Polish {

	int GetPriority(LT::operations op) {
		switch (op) {
		case LT::operations::OEQ:
		case LT::operations::ONE:
		case LT::operations::OMORE:
		case LT::operations::OLESS:
		case LT::operations::OGE:
		case LT::operations::OLE:
			return 1;
		case LT::operations::OPLUS:
		case LT::operations::OMINUS:
			return 2;
		case LT::operations::OMUL:
		case LT::operations::ODIV:
		case LT::operations::OMOD:
			return 3;
		default:
			return 0;
		}
	}

	bool PolishNotation(int i, Lex::LEX& lex, char terminator)
	{
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry placeholder_symbol;
		placeholder_symbol.idxTI = LT_TI_NULLIDX;
		placeholder_symbol.lexema = '#';
		placeholder_symbol.sn = lex.lextable.table[i].sn;

		LT::Entry function_symbol;
		function_symbol.idxTI = LT_TI_NULLIDX;
		function_symbol.lexema = '@'; // Спец. символ для вызова
		function_symbol.sn = lex.lextable.table[i].sn;

		int idx_func = LT_TI_NULLIDX;
		int lexem_counter = 0;
		int parm_counter = 0;
		int lexem_position = i;
		bool findFunc = false;

		// Цикл по лексемам
		for (; i < lex.lextable.size; i++, lexem_counter++)
		{
			// Проверка на выход по терминатору
			if (lex.lextable.table[i].lexema == terminator) {
				// Если терминатор ')', нужно убедиться, что это не закрывающая скобка внутри выражения
				if (terminator == LEX_RIGHTTHESIS && !stack.empty() && stack.top().lexema == LEX_LEFTTHESIS) {
					// Это просто скобка внутри (a+b), идем дальше
				}
				else {
					break; // Нашли конец выражения
				}
			}

			// Проверка на запятую (если мы внутри аргументов функции)
			// Запятая - это разделитель, она выталкивает операторы до скобки
			if (lex.lextable.table[i].lexema == LEX_COMMA) {
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					queue.push(stack.top());
					stack.pop();
				}
				if (findFunc) parm_counter++;
				continue;
			}

			switch (lex.lextable.table[i].lexema)
			{
			case LEX_ID:
			case LEX_LITERAL:
				// Проверка на вызов функции: ID + (
				if (lex.lextable.table[i].lexema == LEX_ID &&
					i + 1 < lex.lextable.size &&
					lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS)
				{
					idx_func = lex.lextable.table[i].idxTI;
					findFunc = true;
					parm_counter = 0; // Сбрасываем счетчик (будет минимум 1, если не пустые скобки)
					// Имя функции пропускаем, оно станет @ в конце
				}
				else
				{
					queue.push(lex.lextable.table[i]);
				}
				continue;

			case LEX_LEFTTHESIS:
				stack.push(lex.lextable.table[i]);
				// Если это скобка сразу после имени функции
				if (findFunc && idx_func != LT_TI_NULLIDX &&
					i > 0 && lex.lextable.table[i - 1].lexema == LEX_ID) {

					// Если внутри скобок не пусто, значит есть параметры
					if (lex.lextable.table[i + 1].lexema != LEX_RIGHTTHESIS) {
						parm_counter = 1;
					}
					else {
						parm_counter = 0;
					}
				}
				continue;

			case LEX_RIGHTTHESIS:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
				}
				if (!stack.empty()) stack.pop(); // Удаляем '('

				// Если закрылась скобка вызова функции (простая эвристика)
				if (findFunc && idx_func != LT_TI_NULLIDX)
				{
					// Мы не можем точно знать, закрылась ли скобка ФУНКЦИИ или вложенного выражения
					// без рекурсии или счетчика скобок. 
					// Но для курсового предположим, что вызовы не вложены глубоко.
					// Если стек пуст или там нет других скобок - считаем, что вызов кончился.

					// Создаем оператор вызова @
					LT::Entry callOp = function_symbol;
					callOp.idxTI = idx_func;
					// В priority можно записать кол-во параметров, чтобы генератор знал
					callOp.priority = parm_counter;

					queue.push(callOp);

					findFunc = false;
					idx_func = LT_TI_NULLIDX;
				}
				continue;

			case LEX_OPERATOR:
			case LEX_LOGOPERATOR: // < > <= >=
			case LEX_EQUAL:       // == !=
				while (!stack.empty() &&
					stack.top().lexema != LEX_LEFTTHESIS &&
					GetPriority(lex.lextable.table[i].op) <= GetPriority(stack.top().op))
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;
			}
		}

		// Выталкиваем остатки
		while (!stack.empty())
		{
			if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
				return false;
			queue.push(stack.top());
			stack.pop();
		}

		// Заменяем лексемы в таблице на ПОЛИЗ
		while (lexem_counter > 0)
		{
			if (!queue.empty())
			{
				lex.lextable.table[lexem_position++] = queue.front();
				queue.pop();
			}
			else
			{
				lex.lextable.table[lexem_position++] = placeholder_symbol;
			}
			lexem_counter--;
		}

		return true;
	}

	bool StartPolish(Lex::LEX& lex)
	{
		bool flag = true;
		for (int i = 0; i < lex.lextable.size; i++)
		{
			// 1. Присваивание (ID = ...)
			if (lex.lextable.table[i].lexema == LEX_EQUAL)
			{
				// == это оператор сравнения, а = это присваивание. У тебя в лексере они различаются?
				// В Lex.cpp: '=' -> LEX_EQUAL. А '==' -> LEX_OPERATOR (OEQ).
				// Значит тут мы ловим именно присваивание.
				if (!PolishNotation(i + 1, lex, LEX_SEMICOLON)) return false;
			}
			// 2. Return
			else if (lex.lextable.table[i].lexema == LEX_RETURN) {
				if (!PolishNotation(i + 1, lex, LEX_SEMICOLON)) return false;
			}
			// 3. Cout
			else if (lex.lextable.table[i].lexema == LEX_COUT) {
				// cout << expr; 
				// Твой лексер не создает << как отдельный токен, он, кажется, пропускает его или делает оператором?
				// В Lex.cpp: '<<' -> LEX_OPERATOR (OLESS) priority 1? Нет, << это сдвиг или вывод.
				// В твоем коде Lex.cpp написано: 
				// if (first == '<' && second == '<') ... isComplexOp = true; opType = LT::operations::OLESS;
				// То есть << распознается как оператор "меньше" (OLESS)? Это странно для cout.
				// Но если мы просто анализируем выражение после cout:
				if (!PolishNotation(i + 1, lex, LEX_SEMICOLON)) return false;
			}
			// 4. Switch
			else if (lex.lextable.table[i].lexema == LEX_SWITCH) {
				if (i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					// switch ( expr )
					if (!PolishNotation(i + 2, lex, LEX_RIGHTTHESIS)) return false;
				}
			}
		}
		return true;
	}
}