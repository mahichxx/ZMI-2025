#include "stdafx.h"
#include "PN.h"
#include "LT.h"
#include <stack>
#include <vector>

using namespace std;

namespace Polish {

	int GetPriority(LT::Entry entry) {
		if (entry.lexema == LEX_EQUAL) return 0; // = самый низкий
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

	// Преобразование одного выражения (от i до terminator)
	bool PolishNotation(int i, Lex::LEX& lex, char terminator) {
		std::stack<LT::Entry> stack;
		std::vector<LT::Entry> out;
		int startPos = i;
		int itemsProcessed = 0;

		for (; i < lex.lextable.size; i++, itemsProcessed++) {
			LT::Entry t = lex.lextable.table[i];

			if (t.lexema == terminator) {
				// Если терминатор ')' и стек пуст или там нет '(', то это конец
				if (terminator == LEX_RIGHTTHESIS) {
					if (stack.empty() || stack.top().lexema != LEX_LEFTTHESIS) break;
				}
				else break;
			}
			// Для switch(expr) { ... }
			if (t.lexema == LEX_LEFTBRACE) break;

			switch (t.lexema) {
			case LEX_ID:
			case LEX_LITERAL:
				// Если это вызов функции: ID + (
				if (t.lexema == LEX_ID && i + 1 < lex.lextable.size && lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					// В простом варианте: не преобразуем вызовы функций в ПОЛИЗ внутри выражений
					// Оставляем как есть, генератор разберется.
					// Просто добавляем ID в выход
					out.push_back(t);
				}
				else {
					out.push_back(t);
				}
				break;

			case LEX_LEFTTHESIS:
				stack.push(t);
				break;

			case LEX_RIGHTTHESIS:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					out.push_back(stack.top());
					stack.pop();
				}
				if (!stack.empty()) stack.pop(); // Удаляем '('
				break;

			case LEX_OPERATOR:
			case LEX_LOGOPERATOR:
			case LEX_EQUAL:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS &&
					GetPriority(t) <= GetPriority(stack.top())) {
					out.push_back(stack.top());
					stack.pop();
				}
				stack.push(t);
				break;

				// Пропускаем запятые (для функций)
			case LEX_COMMA:
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					out.push_back(stack.top());
					stack.pop();
				}
				break;
			}
		}

		while (!stack.empty()) {
			out.push_back(stack.top());
			stack.pop();
		}

		// Записываем обратно
		for (int k = 0; k < out.size(); k++) {
			lex.lextable.table[startPos + k] = out[k];
		}
		// Забиваем остаток заглушками
		for (int k = out.size(); k < itemsProcessed; k++) {
			lex.lextable.table[startPos + k] = { '#', -1, -1 }; // Пустая лексема
		}
		return true;
	}

	bool StartPolish(Lex::LEX& lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			// Обрабатываем выражения после =, return, cout, switch(
			if (lex.lextable.table[i].lexema == LEX_EQUAL ||
				lex.lextable.table[i].lexema == LEX_RETURN ||
				lex.lextable.table[i].lexema == LEX_COUT)
			{
				PolishNotation(i + 1, lex, LEX_SEMICOLON);
			}
			else if (lex.lextable.table[i].lexema == LEX_SWITCH) {
				// switch ( expr )
				if (i + 2 < lex.lextable.size)
					PolishNotation(i + 2, lex, LEX_RIGHTTHESIS);
			}
		}
		return true;
	}
}