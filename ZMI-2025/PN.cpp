#include "stdafx.h"
#include "PN.h"
#include "LT.h"
#include <stack>
#include <queue>
#include <vector>

using namespace std;

namespace Polish {

	int GetPriority(LT::operations op) {
		switch (op) {
		case LT::operations::OEQ: return 1;
		case LT::operations::ONE: return 1;
		case LT::operations::OMORE: return 1;
		case LT::operations::OLESS: return 1;
		case LT::operations::OGE: return 1;
		case LT::operations::OLE: return 1;
		case LT::operations::OPLUS: return 2;
		case LT::operations::OMINUS: return 2;
		case LT::operations::OMUL: return 3;
		case LT::operations::ODIV: return 3;
		case LT::operations::OMOD: return 3;
		default: return 0;
		}
	}

	bool PolishNotation(int i, Lex::LEX& lex, char terminator)
	{
		std::stack<LT::Entry> stack;
		// Используем вектор для финальной записи, чтобы было проще управлять индексами
		std::vector<LT::Entry> outBuffer;

		// Стек для аргументов функций.
		// Каждый уровень стека - это вектор очередей (одна очередь = один аргумент)
		std::stack<std::vector<std::vector<LT::Entry>>> funcArgsStack;
		// Стек индексов функций (чтобы знать, какой ID вызывать)
		std::stack<int> funcIdStack;

		int lexemStartPos = i;
		int processedCount = 0;

		for (; i < lex.lextable.size; i++, processedCount++)
		{
			LT::Entry entry = lex.lextable.table[i];

			// Обработка терминатора (; или })
			if (entry.lexema == terminator) {
				// Если это закрывающая скобка, проверяем, не вложенная ли она
				if (terminator == LEX_RIGHTTHESIS && !stack.empty() && stack.top().lexema == LEX_LEFTTHESIS) {
					// Это скобка приоритета (a+b), обрабатываем ниже
				}
				else {
					break; // Конец выражения
				}
			}

			// --- ЛОГИКА ФУНКЦИЙ ---
			// Если мы внутри функции, мы не пишем в outBuffer сразу, а копим в funcArgsStack
			bool inFunction = !funcArgsStack.empty();

			switch (entry.lexema)
			{
			case LEX_ID:
			case LEX_LITERAL:
				// Проверка: начало вызова функции ID + (
				if (entry.lexema == LEX_ID && i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					// Начинаем новую функцию
					funcIdStack.push(entry.idxTI); // Запоминаем ID

					// Создаем новый слой аргументов (вектор аргументов)
					funcArgsStack.push(std::vector<std::vector<LT::Entry>>());
					// Создаем первый аргумент (пустой вектор лексем)
					funcArgsStack.top().push_back(std::vector<LT::Entry>());
				}
				else {
					// Обычный операнд
					if (inFunction) funcArgsStack.top().back().push_back(entry);
					else outBuffer.push_back(entry);
				}
				continue;

			case LEX_LEFTTHESIS:
				stack.push(entry);
				continue;

			case LEX_RIGHTTHESIS:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					LT::Entry top = stack.top(); stack.pop();
					if (inFunction) funcArgsStack.top().back().push_back(top);
					else outBuffer.push_back(top);
				}
				if (!stack.empty()) stack.pop(); // Выкидываем '('

				// Если закрылась скобка ФУНКЦИИ (проверяем по наличию ID в стеке)
				// Условие: стек скобок пуст (для текущего уровня) и есть активная функция
				// Но здесь сложнее: funcIdStack хранит ID.
				// Простая эвристика: если мы только что закрыли скобку, которая открывала функцию.
				// Мы знаем это, так как funcArgsStack не пуст.
				// Если стек операторов пуст или там нет '(', значит закрылась функция.

				// Дополнительная проверка: если funcArgsStack не пуст, и мы закрыли скобку, соответствующую вызову
				// В данном простом парсере считаем: если inFunction и стек чист от '(', значит это конец аргументов.
				if (!funcIdStack.empty() && (stack.empty() || stack.top().lexema != LEX_LEFTTHESIS)) {

					// 1. Забираем аргументы
					auto args = funcArgsStack.top();
					funcArgsStack.pop();
					int idIdx = funcIdStack.top();
					funcIdStack.pop();

					// 2. Вываливаем аргументы в ОБРАТНОМ ПОРЯДКЕ
					// Если аргумент пустой (функция без параметров), args[0] будет пуст
					int argCount = 0;
					for (int k = args.size() - 1; k >= 0; k--) {
						if (args[k].empty()) continue; // Пустой аргумент (например func())
						argCount++;

						// Куда вываливать? В предыдущую функцию или в main?
						bool stillInFunc = !funcArgsStack.empty();
						for (const auto& token : args[k]) {
							if (stillInFunc) funcArgsStack.top().back().push_back(token);
							else outBuffer.push_back(token);
						}
					}

					// 3. Добавляем оператор @
					LT::Entry callOp;
					callOp.lexema = '@';
					callOp.sn = entry.sn;
					callOp.idxTI = idIdx;
					callOp.priority = argCount; // Сохраняем кол-во аргументов (опционально)

					if (!funcArgsStack.empty()) funcArgsStack.top().back().push_back(callOp);
					else outBuffer.push_back(callOp);
				}
				continue;

			case LEX_COMMA:
				// Запятая - разделитель аргументов
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					LT::Entry top = stack.top(); stack.pop();
					if (inFunction) funcArgsStack.top().back().push_back(top);
					else outBuffer.push_back(top);
				}
				// Начинаем новый аргумент
				if (inFunction) {
					funcArgsStack.top().push_back(std::vector<LT::Entry>());
				}
				continue;

			case LEX_OPERATOR:
			case LEX_LOGOPERATOR:
			case LEX_EQUAL:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS &&
					GetPriority(entry.op) <= GetPriority(stack.top().op)) {
					LT::Entry top = stack.top(); stack.pop();
					if (inFunction) funcArgsStack.top().back().push_back(top);
					else outBuffer.push_back(top);
				}
				stack.push(entry);
				continue;
			}
		}

		// Выталкиваем остатки стека
		while (!stack.empty()) {
			LT::Entry top = stack.top(); stack.pop();
			outBuffer.push_back(top);
		}

		// Записываем результат обратно в таблицу лексем
		int pos = lexemStartPos;
		for (const auto& token : outBuffer) {
			lex.lextable.table[pos++] = token;
		}
		// Заполняем хвост заглушками
		LT::Entry placeholder;
		placeholder.idxTI = LT_TI_NULLIDX; placeholder.lexema = '#';
		while (pos < lexemStartPos + processedCount) {
			lex.lextable.table[pos++] = placeholder;
		}

		return true;
	}

	bool StartPolish(Lex::LEX& lex) {
		for (int i = 0; i < lex.lextable.size; i++) {

			// Присваивание
			if (lex.lextable.table[i].lexema == LEX_EQUAL) {
				PolishNotation(i + 1, lex, LEX_SEMICOLON);
			}

			// Return / Cout
			else if (lex.lextable.table[i].lexema == LEX_RETURN || lex.lextable.table[i].lexema == LEX_COUT) {
				PolishNotation(i + 1, lex, LEX_SEMICOLON);
			}

			// Switch
			else if (lex.lextable.table[i].lexema == LEX_SWITCH) {
				if (i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS)
					PolishNotation(i + 2, lex, LEX_RIGHTTHESIS);
			}

			// Вызов процедуры: ID(...)
			else if (lex.lextable.table[i].lexema == LEX_ID) {
				// Это должно быть начало выражения.
				// Проверяем, что это вызов функции: за ID идет '('
				if (i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {

					// Важно не спутать с ID в выражении (x = id(..)).
					// Если перед ID стоит '=', 'return', 'cout', '(', ',' - это выражение, мы его обработаем в других ветках.
					// Вызов процедуры возможен только если перед ID стоит ';' или '{' или это начало файла.

					bool isProcedureCall = false;
					if (i == 0) isProcedureCall = true;
					else {
						char prev = lex.lextable.table[i - 1].lexema;
						if (prev == LEX_SEMICOLON || prev == LEX_LEFTBRACE || prev == LEX_BRACELET) {
							isProcedureCall = true;
						}
					}

					if (isProcedureCall) {
						PolishNotation(i, lex, LEX_SEMICOLON);
					}
				}
			}
		}
		return true;
	}
}