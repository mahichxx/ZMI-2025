#include "stdafx.h"
#include "Generation.h"
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <stack>
#include <vector>
#include <string>

using namespace std;

namespace Gener {

	// Структура для отслеживания состояния активного switch
	struct SwitchContext {
		int id;             // ID свитча
		int lastCaseVal;    // Значение предыдущего кейса
		bool hasDefault;
		int braceBalance;   // Баланс скобок при входе
	};

	bool IsLib(const char* id) {
		return (strcmp(id, "strtoint") == 0 || strcmp(id, "stcmp") == 0);
	}

	string GetID(IT::Entry& e) {
		string s((char*)e.id);
		return s;
	}

	void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log) {
		ofstream out("ASM\\Asm.asm");
		if (!out.is_open()) return;

		// Путь к твоей библиотеке
		string libPath = "\"D:\\Программирование\\3_сем\\КПО\\ZMI-2025\\Debug\\InLib.lib\"";

		out << ".586\n.model flat, stdcall\n"
			<< "includelib kernel32.lib\n"

			// !!! НОВЫЕ СТРОКИ: Подключаем стандартные библиотеки C++ !!!
			<< "includelib msvcrtd.lib\n"      // C Runtime (Debug)
			<< "includelib ucrtd.lib\n"        // Universal CRT (Debug)
			<< "includelib vcruntimed.lib\n"   // VC Runtime (Debug)
			// Эта библиотека нужна для работы printf/cout в VS 2015+
			<< "includelib legacy_stdio_definitions.lib\n"

			<< "includelib " << libPath << "\n\n"
			<< "ExitProcess PROTO :DWORD\n"
			<< "outnum PROTO :SDWORD\n"
			<< "outstr PROTO :DWORD\n"
			<< "newline PROTO\n"
			<< "strtoint PROTO :DWORD\n"
			<< "stcmp PROTO :DWORD, :DWORD\n"
			<< "\n.stack 4096\n.const\n";

		// Константы
		for (int i = 0; i < tables.idtable.size; i++) {
			IT::Entry& e = tables.idtable.table[i];
			if (e.idtype == IT::L) {
				out << "\tL" << i << "\t";
				if (e.iddatatype == IT::STR) out << "db '" << e.value.vstr.str << "', 0" << endl;
				else if (e.iddatatype == IT::INT) out << "sdword " << e.value.vint << endl;
				else out << "byte '" << e.value.vchar << "'" << endl;
			}
		}

		// Переменные
		out << ".data\n";

		// !!! ИСПРАВЛЕНИЕ: Объявляем переменную для switch !!!
		out << "\tswitch_val dd 0" << endl;

		for (int i = 0; i < tables.idtable.size; i++) {
			IT::Entry& e = tables.idtable.table[i];
			if (e.idtype == IT::V) {
				out << "\t" << GetID(e) << "\t";
				if (e.iddatatype == IT::STR) out << "dd 0" << endl;
				else out << "sbyte 0" << endl;
			}
		}

		out << ".code\n";

		stack<SwitchContext> switches;
		int currentBalance = 0;
		bool inProc = false;
		string currentProcName = "";

		for (int i = 0; i < tables.lextable.size; i++) {
			LT::Entry& t = tables.lextable.table[i];

			// Баланс скобок
			if (t.lexema == LEX_LEFTBRACE) currentBalance++;
			if (t.lexema == LEX_BRACELET) {
				currentBalance--;

				// Конец свитча?
				if (!switches.empty()) {
					if (currentBalance == switches.top().braceBalance) {
						SwitchContext& ctx = switches.top();
						// Если был case, ставим метку next для него
						if (ctx.lastCaseVal != -1) {
							out << "sw_" << ctx.id << "_next_" << ctx.lastCaseVal << ":" << endl;
						}
						out << "sw_" << ctx.id << "_end:" << endl;
						switches.pop();
					}
				}
			}

			// 1. ФУНКЦИЯ
			if (t.lexema == LEX_FUNCTION) {
				i++;
				IT::Entry& func = tables.idtable.table[tables.lextable.table[i].idxTI];
				if (IsLib((char*)func.id)) {
					while (tables.lextable.table[i].lexema != LEX_LEFTBRACE) i++;
					int bal = 1; i++;
					while (i < tables.lextable.size && bal > 0) {
						if (tables.lextable.table[i].lexema == LEX_LEFTBRACE) bal++;
						if (tables.lextable.table[i].lexema == LEX_BRACELET) bal--;
						i++;
					}
					i--; continue;
				}

				if (inProc) out << currentProcName << " ENDP\n";
				currentProcName = GetID(func);
				out << currentProcName << " PROC";
				inProc = true;

				int p = i + 2; bool first = true;
				while (tables.lextable.table[p].lexema != LEX_RIGHTTHESIS) {
					if (tables.lextable.table[p].lexema == LEX_ID) {
						if (first) { out << ", "; first = false; }
						else out << ", ";
						out << GetID(tables.idtable.table[tables.lextable.table[p].idxTI]) << " :DWORD";
					}
					p++;
				}
				out << endl;
				while (tables.lextable.table[i].lexema != LEX_LEFTBRACE) i++;
				currentBalance = 0; currentBalance++;
				continue;
			}

			// 2. MAIN
			if (t.lexema == LEX_MAIN) {
				if (inProc) out << currentProcName << " ENDP\n";
				currentProcName = "main";
				out << "main PROC" << endl;
				inProc = true;
				while (tables.lextable.table[i].lexema != LEX_LEFTBRACE) i++;
				currentBalance = 1; continue;
			}

			// 3. RETURN
			if (t.lexema == LEX_RETURN) {
				out << "\tpop eax\n\tret" << endl;
				continue;
			}

			// 4. SWITCH
			if (t.lexema == LEX_SWITCH) {
				SwitchContext ctx;
				ctx.id = i;
				ctx.lastCaseVal = -1;
				ctx.hasDefault = false;
				ctx.braceBalance = currentBalance; // Баланс ПЕРЕД {
				switches.push(ctx);

				out << "\tpop eax" << endl;
				out << "\tmov switch_val, eax" << endl; // Теперь эта переменная существует!
				continue;
			}

			// CASE
			if (t.lexema == LEX_CASE) {
				if (switches.empty()) continue;
				SwitchContext& ctx = switches.top();

				if (ctx.lastCaseVal != -1) {
					out << "\tjmp sw_" << ctx.id << "_end" << endl;
					out << "sw_" << ctx.id << "_next_" << ctx.lastCaseVal << ":" << endl;
				}

				// Восстанавливаем значение
				out << "\tmov eax, switch_val" << endl;

				int val = tables.idtable.table[tables.lextable.table[i + 1].idxTI].value.vint;
				out << "\tcmp eax, " << val << endl;
				out << "\tjne sw_" << ctx.id << "_next_" << val << endl;

				ctx.lastCaseVal = val;
				i++;
				continue;
			}

			// DEFAULT
			if (t.lexema == LEX_DEFAULT) {
				if (switches.empty()) continue;
				SwitchContext& ctx = switches.top();
				if (ctx.lastCaseVal != -1) {
					out << "\tjmp sw_" << ctx.id << "_end" << endl;
					out << "sw_" << ctx.id << "_next_" << ctx.lastCaseVal << ":" << endl;
				}
				ctx.lastCaseVal = -1;
				ctx.hasDefault = true;
				continue;
			}

			// 5. ОПЕРАНДЫ
			if (t.lexema == LEX_LITERAL) {
				IT::Entry& l = tables.idtable.table[t.idxTI];
				if (l.iddatatype == IT::STR) out << "\tpush offset L" << t.idxTI << endl;
				else out << "\tpush " << l.value.vint << endl;
			}
			else if (t.lexema == LEX_ID) {
				IT::Entry& v = tables.idtable.table[t.idxTI];
				if (v.idtype == IT::V || v.idtype == IT::P) {
					if (v.iddatatype == IT::STR || v.idtype == IT::P) out << "\tpush " << GetID(v) << endl;
					else { out << "\tmovsx eax, " << GetID(v) << endl; out << "\tpush eax" << endl; }
				}
			}

			// 6. ВЫЗОВ
			if (t.lexema == '@') {
				IT::Entry& func = tables.idtable.table[t.idxTI];
				out << "\tcall " << GetID(func) << endl;
				out << "\tpush eax" << endl;
			}

			// 7. ОПЕРАТОРЫ
			// 7. ОПЕРАТОРЫ
			if (t.lexema == LEX_OPERATOR) {
				if (t.op == LT::OLESS) {
					// Проверяем тип того, что мы выводим (предыдущая лексема)
					bool isString = false;
					if (i > 0) {
						LT::Entry& prev = tables.lextable.table[i - 1];
						if (prev.idxTI != LT_TI_NULLIDX) {
							IT::Entry& e = tables.idtable.table[prev.idxTI];
							if (e.iddatatype == IT::STR) isString = true;
						}
					}

					out << "\tpop eax" << endl;
					if (isString) {
						out << "\tinvoke outstr, eax" << endl;
					}
					else {
						out << "\tinvoke outnum, eax" << endl;
					}
					out << "\tinvoke newline" << endl;
				}
				else {
					// ... (остальной код операторов без изменений)
					out << "\tpop ebx\n\tpop eax" << endl;
					if (t.op == LT::OPLUS) out << "\tadd eax, ebx" << endl;
					if (t.op == LT::OMINUS) out << "\tsub eax, ebx" << endl;
					if (t.op == LT::OMUL) out << "\timul eax, ebx" << endl;
					out << "\tpush eax" << endl;
				}
			}

			// 8. ПРИСВАИВАНИЕ
			if (t.lexema == LEX_EQUAL) {
				IT::Entry& l = tables.idtable.table[tables.lextable.table[i - 1].idxTI];
				out << "\tpop eax" << endl;
				if (l.iddatatype == IT::STR || l.idtype == IT::P) out << "\tmov " << GetID(l) << ", eax" << endl;
				else out << "\tmov " << GetID(l) << ", al" << endl;
			}
		}

		if (inProc) {
			if (currentProcName == "main") out << "\tinvoke ExitProcess, 0\n";
			out << currentProcName << " ENDP" << endl;
		}
		out << "end main" << endl;
		out.close();
		cout << "[GENERATION] SUCCESS: ASM\\Asm.asm" << endl;
	}
}