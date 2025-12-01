#include "stdafx.h"
#include "PN.h" // Если есть заголовочный файл, иначе проверь инклуды

namespace Polish {

	// Получение приоритета операций
	// Чем больше число, тем выше приоритет
	int GetPriority(LT::operations op) {
		switch (op) {
		case LT::operations::OEQ:       // ==
		case LT::operations::ONE:       // !=
		case LT::operations::OMORE:     // >
		case LT::operations::OLESS:     // < (также используется для << в cout)
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

	// Основная функция преобразования в ПОЛИЗ
	// terminator - символ, на котором останавливаемся (для switch это ')', для остальных ';')
	bool PolishNotation(int i, Lex::LEX& lex, char terminator = LEX_SEMICOLON)
	{
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry placeholder_symbol; // Заполнитель (чтобы стереть лишние лексемы из таблицы)
		placeholder_symbol.idxTI = LT_TI_NULLIDX;
		placeholder_symbol.lexema = '#'; // Используем спецсимвол для пустоты
		placeholder_symbol.sn = lex.lextable.table[i].sn;

		LT::Entry function_symbol;
		function_symbol.idxTI = LT_TI_NULLIDX;
		function_symbol.lexema = '@'; // Собачка для вызова функций
		function_symbol.sn = lex.lextable.table[i].sn;

		int idx = 0;
		int lexem_counter = 0;
		int parm_counter = 0;
		int lexem_position = i;
		char* buf = new char[10];

		bool findFunc = false;

		// Цикл пока не встретим терминатор (точка с запятой или закрывающая скобка для switch)
		for (i; lex.lextable.table[i].lexema != terminator; i++, lexem_counter++)
		{
			// Защита от выхода за границы
			if (i >= lex.lextable.size) return false;

			switch (lex.lextable.table[i].lexema)
			{
				// Операнды: Идентификаторы и Литералы
			case LEX_ID:
			case LEX_LITERAL:
				// Если это функция (вызов)
				if (lex.lextable.table[i].idxTI != LT_TI_NULLIDX &&
					lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F)
				{
					findFunc = true;
					idx = lex.lextable.table[i].idxTI;
				}
				else
				{
					if (findFunc)
						parm_counter++; // Считаем параметры функции
					queue.push(lex.lextable.table[i]);
				}
				continue;

			case LEX_LEFTTHESIS: // (
				stack.push(lex.lextable.table[i]);
				continue;

			case LEX_RIGHTTHESIS: // )
				// Если мы встретили ), но terminator тоже ), значит мы дошли до конца switch(expr)
				if (terminator == LEX_RIGHTTHESIS && stack.empty()) {
					// Это конец выражения switch(..), выходим из цикла, чтобы не съесть лишнее
					goto end_loop;
				}

				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
				}

				if (stack.empty()) return false; // Ошибка скобок

				if (!findFunc) {
					stack.pop(); // Выкидываем '('
				}
				else {
					// Обработка закрытия вызова функции
					function_symbol.idxTI = idx;
					idx = LT_TI_NULLIDX;

					// Генерируем лексему вызова @param_count
					lex.lextable.table[i] = function_symbol;
					// Можно использовать priority или idxTI для хранения кол-ва параметров
					// В оригинале использовался трюк с записью числа в lexema char, это опасно
					// Лучше сохранить в op или priority, если генератор кода это поймет.
					// Сохраним оригинальный подход, но аккуратно:
					_itoa_s(parm_counter, buf, 10, 10);

					// Создаем специальную лексему для вызова
					LT::Entry callOp = function_symbol;
					callOp.lexema = '@';
					callOp.priority = parm_counter; // Сохраним кол-во параметров в приоритет (лайфхак)

					queue.push(callOp);

					stack.pop(); // Выкидываем '('
					parm_counter = 0;
					findFunc = false;
				}
				continue;

			case LEX_OPERATOR:
			case LEX_LOGOPERATOR: // Если есть логические
			case LEX_EQUAL:       // Если вдруг '=' попало сюда (хотя StartPolish фильтрует)
				while (!stack.empty() &&
					stack.top().lexema != LEX_LEFTTHESIS &&
					GetPriority(lex.lextable.table[i].op) <= GetPriority(stack.top().op))
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;

			case LEX_COMMA: // Запятая - разделитель аргументов
				while (!stack.empty() && stack.top().lexema != LEX_LEFTTHESIS) {
					queue.push(stack.top());
					stack.pop();
				}
				if (findFunc) parm_counter++;
				continue;
			}
		}

	end_loop:
		while (!stack.empty())
		{
			if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
				return false; // Непарные скобки

			queue.push(stack.top());
			stack.pop();
		}

		// Записываем результат обратно в LexTable
		while (lexem_counter != 0)
		{
			if (!queue.empty())
			{
				lex.lextable.table[lexem_position++] = queue.front();
				queue.pop();
			}
			else
				// Забиваем пустотой освободившееся место
				lex.lextable.table[lexem_position++] = placeholder_symbol;

			lexem_counter--;
		}

		// Маркировка строк для отладки
		for (int k = 0; k < lexem_position; k++)
		{
			if (lex.lextable.table[k].lexema == LEX_OPERATOR || lex.lextable.table[k].lexema == LEX_LITERAL)
				if (lex.lextable.table[k].idxTI != LT_TI_NULLIDX)
					lex.idtable.table[lex.lextable.table[k].idxTI].idxfirstLE = k;
		}

		return true;
	}

	bool StartPolish(Lex::LEX& lex)
	{
		bool flag = false;
		for (int i = 0; i < lex.lextable.size; i++)
		{
			// 1. Присваивание: id = expr;
			// Преобразуем только expr (то, что после =)
			if (lex.lextable.table[i].lexema == LEX_EQUAL)
			{
				flag = PolishNotation(i + 1, lex, LEX_SEMICOLON);
				if (!flag) return false;
			}

			// 2. Return: return expr;
			else if (lex.lextable.table[i].lexema == LEX_RETURN) {
				flag = PolishNotation(i + 1, lex, LEX_SEMICOLON);
				if (!flag) return false;
			}

			// 3. Cout: cout << expr;
			// Лексема LEX_COUT ('o'), за ней идет оператор <<, потом выражение
			else if (lex.lextable.table[i].lexema == LEX_COUT) {
				// Начинаем преобразование сразу после cout. 
				// Оператор << попадет в ПОЛИЗ как операция вывода.
				flag = PolishNotation(i + 1, lex, LEX_SEMICOLON);
				if (!flag) return false;
			}

			// 4. Switch: switch ( expr )
			// Преобразуем выражение внутри скобок
			else if (lex.lextable.table[i].lexema == LEX_SWITCH) {
				// Структура: switch ( expr )
				// i = switch, i+1 = (
				// Начинаем с i+2
				if (lex.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) {
					flag = PolishNotation(i + 2, lex, LEX_RIGHTTHESIS); // Останавливаемся на )
					if (!flag) return false;
				}
			}
		}
		return true;
	}
}