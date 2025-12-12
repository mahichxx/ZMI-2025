#include "stdafx.h"
#include "PN.h"
#include "LT.h"
#include <stack>
#include <vector>

using namespace std;

namespace Polish {

	int GetPriority(LT::Entry entry) {
		if (entry.lexema == LEX_EQUAL) return 0;
		if (entry.lexema == LEX_OPERATOR || entry.lexema == LEX_LOGOPERATOR) {
			switch (entry.op) {
			case LT::OEQ: case LT::ONE: return 1;
			case LT::OMORE: case LT::OLESS: case LT::OGE: case LT::OLE: return 1;
			case LT::OPLUS: case LT::OMINUS: return 2;
			case LT::OMUL: case LT::ODIV: case LT::OMOD: return 3;
			default: return 0;
			}
		}
		return -1;
	}

	bool PolishNotation(int i, Lex::LEX& lex, char terminator) {
		std::stack<LT::Entry> stack;
		std::vector<LT::Entry> out;

		// Стек для вложенных вызовов функций: каждый элемент - это список аргументов
		std::stack<std::vector<std::vector<LT::Entry>>> funcArgsStack;
		std::stack<int> funcIdStack;

		int startPos = i;
		int itemsProcessed = 0;

		for (; i < lex.lextable.size; i++, itemsProcessed++) {
			LT::Entry t = lex.lextable.table[i];

			// Обработка терминатора (закрывающая скобка или ;)
			if (t.lexema == terminator) {
				if (terminator == LEX_RIGHTTHESIS) {
					// Если терминатор ')', выходим только если стек пуст или там нет парной '('
					if (stack.empty() || stack.top().lexema != LEX_LEFTTHESIS) break;
				}
				else {
					break;
				}
			}
			// Для switch(expr) { ... }
			if (t.lexema == LEX_LEFTBRACE) break;

			bool inFunction = !funcArgsStack.empty();

			switch (t.lexema) {
			case LEX_ID:
			case LEX_LITERAL:
				// Начало вызова функции: ID + (
				if (t.lexema == LEX_ID && i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					funcIdStack.push(t.idxTI);

					// Создаем новый уровень для аргументов
					std::vector<std::vector<LT::Entry>> args;
					args.push_back(std::vector<LT::Entry>()); // Первый аргумент
					funcArgsStack.push(args);
				}
				else {
					// Просто операнд
					if (inFunction) {
						// !!! БЕЗОПАСНАЯ ВСТАВКА !!!
						if (!funcArgsStack.empty() && !funcArgsStack.top().empty()) {
							funcArgsStack.top().back().push_back(t);
						}
					}
					else {
						out.push_back(t);
					}
				}
				break;

			case LEX_LEFTTHESIS:
				stack.push(t);
				break;

			case LEX_RIGHTTHESIS:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					LT::Entry top = stack.top(); stack.pop();
					if (inFunction && !funcArgsStack.empty() && !funcArgsStack.top().empty())
						funcArgsStack.top().back().push_back(top);
					else out.push_back(top);
				}
				if (!stack.empty()) stack.pop(); // Удаляем '('

				// Если это была скобка закрытия функции
				if (!funcIdStack.empty() && (stack.empty() || stack.top().lexema != LEX_LEFTTHESIS)) {

					if (funcArgsStack.empty()) break; // Защита

					auto args = funcArgsStack.top();
					funcArgsStack.pop();
					int idIdx = funcIdStack.top();
					funcIdStack.pop();

					// Вываливаем аргументы
					bool stillInFunc = !funcArgsStack.empty();

					// Аргументы выводим так, чтобы они шли подряд, а потом @
					// Но для стековой машины (ASM) порядок аргументов важен. 
					// В коде генератора мы их перебираем.
					// Здесь просто линеаризуем их в ПОЛИЗ.

					for (auto& arg : args) {
						for (auto& token : arg) {
							if (stillInFunc && !funcArgsStack.empty() && !funcArgsStack.top().empty())
								funcArgsStack.top().back().push_back(token);
							else
								out.push_back(token);
						}
					}

					// Добавляем оператор вызова @
					LT::Entry callOp;
					callOp.lexema = '@';
					callOp.sn = t.sn;
					callOp.idxTI = idIdx;

					if (stillInFunc && !funcArgsStack.empty() && !funcArgsStack.top().empty())
						funcArgsStack.top().back().push_back(callOp);
					else
						out.push_back(callOp);
				}
				break;

			case LEX_COMMA:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					LT::Entry top = stack.top(); stack.pop();
					if (inFunction && !funcArgsStack.empty() && !funcArgsStack.top().empty())
						funcArgsStack.top().back().push_back(top);
					else out.push_back(top);
				}
				// Новый аргумент
				if (inFunction && !funcArgsStack.empty()) {
					funcArgsStack.top().push_back(std::vector<LT::Entry>());
				}
				break;

			case LEX_OPERATOR:
			case LEX_LOGOPERATOR:
			case LEX_EQUAL:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS &&
					GetPriority(t) <= GetPriority(stack.top())) {
					LT::Entry top = stack.top(); stack.pop();
					if (inFunction && !funcArgsStack.empty() && !funcArgsStack.top().empty())
						funcArgsStack.top().back().push_back(top);
					else out.push_back(top);
				}
				stack.push(t);
				break;
			}
		}

		while (!stack.empty()) {
			out.push_back(stack.top());
			stack.pop();
		}

		// Записываем обратно
		for (size_t k = 0; k < out.size(); k++) {
			lex.lextable.table[startPos + k] = out[k];
		}
		// Забиваем остаток заглушками
		for (size_t k = out.size(); k < (size_t)itemsProcessed; k++) {
			lex.lextable.table[startPos + k] = { '#', -1, -1 };
		}
		return true;
	}

	bool StartPolish(Lex::LEX& lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			// Выражения
			if (lex.lextable.table[i].lexema == LEX_EQUAL ||
				lex.lextable.table[i].lexema == LEX_RETURN ||
				lex.lextable.table[i].lexema == LEX_COUT)
			{
				PolishNotation(i + 1, lex, LEX_SEMICOLON);
			}
			// Условие switch ( expr )
			else if (lex.lextable.table[i].lexema == LEX_SWITCH) {
				if (i + 2 < lex.lextable.size)
					PolishNotation(i + 2, lex, LEX_RIGHTTHESIS);
			}
			// Вызов процедуры как отдельной инструкции: ID(...);
			else if (lex.lextable.table[i].lexema == LEX_ID) {
				if (i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					// Проверяем, что это не объявление и не часть выражения
					bool isProc = false;
					if (i == 0) isProc = true;
					else {
						char prev = lex.lextable.table[i - 1].lexema;
						if (prev == LEX_SEMICOLON || prev == LEX_LEFTBRACE || prev == LEX_BRACELET || prev == LEX_TWOPOINT)
							isProc = true;
					}
					if (isProc) PolishNotation(i, lex, LEX_SEMICOLON);
				}
			}
		}
		return true;
	}
}