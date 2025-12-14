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

	struct SwitchCtx {
		int id;
		int lastCaseVal;
		int braceBalance;
	};

	bool IsLib(const char* id) {
		return (strcmp(id, "strtoint") == 0 || strcmp(id, "stcmp") == 0 ||
			strcmp(id, "strle") == 0 || strcmp(id, "mabs") == 0 || strcmp(id, "rnd") == 0);
	}

	string GetID(IT::Entry& e) {
		string s((char*)e.id);
		return s;
	}

	void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log) {
		ofstream out("ASM\\Asm.asm");
		if (!out.is_open()) return;

		string libPath = "\"D:\\Программирование\\3_сем\\КПО\\ZMI-2025\\Debug\\InLib.lib\"";

		out << ".586\n.model flat, stdcall\n"
			<< "includelib kernel32.lib\n"
			<< "includelib msvcrtd.lib\n"
			<< "includelib ucrtd.lib\n"
			<< "includelib vcruntimed.lib\n"
			<< "includelib legacy_stdio_definitions.lib\n"
			<< "includelib " << libPath << "\n\n"
			<< "ExitProcess PROTO :DWORD\n"
			<< "outnum PROTO :SDWORD\n"
			<< "outstr PROTO :DWORD\n"
			<< "newline PROTO\n"
			<< "strtoint PROTO :DWORD\n"
			<< "stcmp PROTO :DWORD, :DWORD\n"
			<< "strle PROTO :DWORD\n"
			<< "mabs PROTO :SDWORD\n"
			<< "rnd PROTO :SDWORD\n"
			<< "\n.stack 4096\n.const\n";

		for (int i = 0; i < tables.idtable.size; i++) {
			IT::Entry& e = tables.idtable.table[i];
			if (e.idtype == IT::L) {
				out << "\tL" << i << "\t";
				if (e.iddatatype == IT::STR) out << "db '" << e.value.vstr.str << "', 0" << endl;
				else if (e.iddatatype == IT::INT) out << "sdword " << e.value.vint << endl;
				else out << "byte '" << e.value.vchar << "'" << endl;
			}
		}

		out << ".data\n";
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

		stack<SwitchCtx> switches;
		int currentBalance = 0;
		bool inProc = false;
		string currentProcName = "";
		int lastCaseValue = -1;
		int assignmentTargetIdx = -1;

		// !!! ФЛАГ ВОЗВРАТА !!!
		bool isReturn = false;

		for (int i = 0; i < tables.lextable.size; i++) {
			LT::Entry& t = tables.lextable.table[i];

			if (t.lexema == LEX_LEFTBRACE) currentBalance++;
			if (t.lexema == LEX_BRACELET) {
				currentBalance--;
				if (!switches.empty()) {
					if (currentBalance == switches.top().braceBalance) {
						SwitchCtx& ctx = switches.top();
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
					int balance = 1; i++;
					while (i < tables.lextable.size && balance > 0) {
						if (tables.lextable.table[i].lexema == LEX_LEFTBRACE) balance++;
						if (tables.lextable.table[i].lexema == LEX_BRACELET) balance--;
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
				currentBalance++; continue;
			}

			// 2. MAIN
			if (t.lexema == LEX_MAIN) {
				if (inProc) out << currentProcName << " ENDP\n";
				currentProcName = "main";
				out << "main PROC" << endl;
				inProc = true;
				while (tables.lextable.table[i].lexema != LEX_LEFTBRACE) i++;
				currentBalance++; continue;
			}

			// 3. RETURN (ТОЛЬКО СТАВИМ ФЛАГ)
			if (t.lexema == LEX_RETURN) {
				isReturn = true;
				continue;
			}

			// 4. SWITCH
			if (t.lexema == LEX_SWITCH) {
				SwitchCtx ctx; ctx.id = i; ctx.lastCaseVal = -1; ctx.braceBalance = currentBalance;
				switches.push(ctx);
				out << "\tpop eax\n\tmov switch_val, eax" << endl;
				continue;
			}
			if (t.lexema == LEX_CASE) {
				if (switches.empty()) continue;
				SwitchCtx& ctx = switches.top();
				int val = tables.idtable.table[tables.lextable.table[i + 1].idxTI].value.vint;
				if (ctx.lastCaseVal != -1) {
					out << "\tjmp sw_" << ctx.id << "_end" << endl;
					out << "sw_" << ctx.id << "_next_" << ctx.lastCaseVal << ":" << endl;
				}
				out << "\tmov eax, switch_val" << endl;
				out << "\tcmp eax, " << val << endl;
				out << "\tjne sw_" << ctx.id << "_next_" << val << endl;
				ctx.lastCaseVal = val;
				i++; continue;
			}
			if (t.lexema == LEX_DEFAULT) {
				if (switches.empty()) continue;
				SwitchCtx& ctx = switches.top();
				if (ctx.lastCaseVal != -1) {
					out << "\tjmp sw_" << ctx.id << "_end" << endl;
					out << "sw_" << ctx.id << "_next_" << ctx.lastCaseVal << ":" << endl;
				}
				ctx.lastCaseVal = -1;
				continue;
			}

			// 5. ОПЕРАНДЫ
			if (t.lexema == LEX_LITERAL) {
				IT::Entry& l = tables.idtable.table[t.idxTI];
				if (l.iddatatype == IT::STR) out << "\tpush offset L" << t.idxTI << endl;
				else out << "\tpush " << l.value.vint << endl;
			}
			else if (t.lexema == LEX_ID) {
				// Пропуск объявлений
				if (i > 0) {
					char prevLex = tables.lextable.table[i - 1].lexema;
					if (prevLex == LEX_INTEGER || prevLex == LEX_STRING || prevLex == LEX_CHAR || prevLex == LEX_VOID) {
						out << "\t; Decl: " << GetID(tables.idtable.table[t.idxTI]) << endl;
						continue;
					}
				}

				if (i + 1 < tables.lextable.size && tables.lextable.table[i + 1].lexema == LEX_EQUAL) {
					assignmentTargetIdx = t.idxTI;
				}
				else {
					IT::Entry& v = tables.idtable.table[t.idxTI];
					if (v.idtype == IT::V || v.idtype == IT::P) {
						if (v.iddatatype == IT::STR || v.idtype == IT::P) out << "\tpush " << GetID(v) << endl;
						else { out << "\tmovsx eax, " << GetID(v) << endl; out << "\tpush eax" << endl; }
					}
				}
			}

			// 6. ВЫЗОВ
			if (t.lexema == '@') {
				IT::Entry& func = tables.idtable.table[t.idxTI];
				out << "\tcall " << GetID(func) << endl;
				out << "\tpush eax" << endl;
			}

			// 7. ОПЕРАТОРЫ
			if (t.lexema == LEX_OPERATOR) {
				// Если это вывод (show <)
				if (t.op == LT::OLESS) {
					bool isString = false;
					if (i > 0) {
						LT::Entry& prev = tables.lextable.table[i - 1];
						if (prev.idxTI != LT_TI_NULLIDX) {
							IT::Entry& e = tables.idtable.table[prev.idxTI];
							if (e.iddatatype == IT::STR) isString = true;
						}
					}
					out << "\tpop eax" << endl;
					if (isString) out << "\tinvoke outstr, eax" << endl;
					else out << "\tinvoke outnum, eax" << endl;
					out << "\tinvoke newline" << endl;
				}
				else {
					// Арифметика и Сравнения
					out << "\tpop ebx\n\tpop eax" << endl;

					if (t.op == LT::OPLUS) out << "\tadd eax, ebx" << endl;
					else if (t.op == LT::OMINUS) out << "\tsub eax, ebx" << endl;
					else if (t.op == LT::OMUL) out << "\timul eax, ebx" << endl;
					else if (t.op == LT::ODIV) out << "\tcdq\n\tidiv ebx" << endl;
					else if (t.op == LT::OMOD) out << "\tcdq\n\tidiv ebx\n\tmov eax, edx" << endl;

					// Сравнения
					else {
						out << "\tcmp eax, ebx" << endl;
						out << "\tmov eax, 0" << endl; // Обнуляем для set

						if (t.op == LT::OEQ) out << "\tsete al" << endl;
						else if (t.op == LT::ONE) out << "\tsetne al" << endl;
						else if (t.op == LT::OMORE) out << "\tsetg al" << endl;
						else if (t.op == LT::OGE) out << "\tsetge al" << endl;
						else if (t.op == LT::OLE) out << "\tsetle al" << endl;

						// !!! НОВЫЙ ОПЕРАТОР СРАВНЕНИЯ !!!
						else if (t.op == LT::OLESS_CMP) out << "\tsetl al" << endl;
					}
					out << "\tpush eax" << endl;
				}
			}
			if (t.lexema == LEX_EQUAL) continue;

			// 9. ТОЧКА С ЗАПЯТОЙ (Обработка концов инструкций)
			if (t.lexema == LEX_SEMICOLON) {
				// Присваивание
				if (assignmentTargetIdx != -1) {
					IT::Entry& l = tables.idtable.table[assignmentTargetIdx];
					out << "\tpop eax" << endl;
					if (l.iddatatype == IT::STR || l.idtype == IT::P) out << "\tmov " << GetID(l) << ", eax" << endl;
					else out << "\tmov " << GetID(l) << ", al" << endl;
					assignmentTargetIdx = -1;
				}
				// Возврат из функции
				else if (isReturn) {
					out << "\tpop eax" << endl;
					out << "\tret" << endl;
					isReturn = false;
				}
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