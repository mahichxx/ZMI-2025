#include "stdafx.h"
#include "Generation.h"
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <cstring>

using namespace std;

namespace Gener
{
    const string LIB_PATH = "\"D:\\Программирование\\3_сем\\КПО\\ZMI-2025\\Debug\\InLib.lib\"";

    const string ASM_HEAD =
        ".586\n"
        ".model flat, stdcall\n"
        "includelib msvcrtd.lib\n"
        "includelib ucrtd.lib\n"
        "includelib vcruntimed.lib\n"
        "includelib legacy_stdio_definitions.lib\n"
        "includelib kernel32.lib\n"
        "includelib " + LIB_PATH + "\n"
        "\n"
        "ExitProcess PROTO :DWORD\n"
        "outnum PROTO :SDWORD\n"
        "outstr PROTO :DWORD\n"
        "newline PROTO\n"
        "stcmp PROTO :DWORD, :DWORD\n"
        "strtoint PROTO :DWORD\n"
        "\n.stack 4096\n";

    const string ASM_CONST = ".const\n";
    const string ASM_DATA = ".data\n";
    const string ASM_CODE = ".code\n";

    bool IsLibraryFunc(const char* name) {
        return (strcmp(name, "strtoint") == 0 || strcmp(name, "stcmp") == 0);
    }

    void ProcessStatements(Lex::LEX& tables, ofstream& ofile, int startIdx, int endIdx)
    {
        bool isCout = false;
        bool isReturn = false;
        IT::IDDATATYPE lastType = IT::INT;
        int targetIdIndex = -1;
        stack<int> activeSwitches;
        stack<int> switchEndIndices;

        for (int i = startIdx; i < endIdx; i++)
        {
            LT::Entry& lex = tables.lextable.table[i];

            if (lex.lexema == LEX_SWITCH) {
                int uid = i;
                int balance = 0, closeBraceIdx = -1, openBraceIdx = -1, k = i + 1;
                while (k < endIdx && tables.lextable.table[k].lexema != LEX_LEFTBRACE) k++;
                openBraceIdx = k;
                for (; k < endIdx; k++) {
                    if (tables.lextable.table[k].lexema == LEX_LEFTBRACE) balance++;
                    if (tables.lextable.table[k].lexema == LEX_BRACELET) {
                        balance--;
                        if (balance == 0) { closeBraceIdx = k; break; }
                    }
                }
                if (closeBraceIdx != -1) {
                    activeSwitches.push(uid);
                    switchEndIndices.push(closeBraceIdx);
                    int idIndex = -1;
                    for (int s = i + 1; s < openBraceIdx; s++) {
                        if (tables.lextable.table[s].lexema == LEX_ID) { idIndex = tables.lextable.table[s].idxTI; break; }
                    }
                    if (idIndex != -1) {
                        ofile << "\n\t; --- SWITCH " << uid << " ---" << endl;
                        IT::Entry& var = tables.idtable.table[idIndex];
                        if (var.idtype == IT::P || var.idtype == IT::V) {
                            if (var.iddatatype == IT::INT || var.iddatatype == IT::BOOL || var.iddatatype == IT::CHR) {
                                ofile << "\tmov al, " << var.id << endl << "\tmovsx eax, al" << endl;
                            }
                            else ofile << "\tmov eax, " << var.id << endl;
                        }
                        ofile << "\tmov switch_val, eax" << endl;
                        int innerBalance = 0;
                        for (int s = openBraceIdx + 1; s < closeBraceIdx; s++) {
                            if (tables.lextable.table[s].lexema == LEX_LEFTBRACE) innerBalance++;
                            if (tables.lextable.table[s].lexema == LEX_BRACELET) innerBalance--;
                            if (innerBalance == 0 && tables.lextable.table[s].lexema == LEX_CASE) {
                                int litIdx = tables.lextable.table[s + 1].idxTI;
                                int val = tables.idtable.table[litIdx].value.vint;
                                ofile << "\tcmp eax, " << val << endl << "\tje switch_" << uid << "_case_" << val << endl;
                            }
                        }
                        ofile << "\tjmp switch_" << uid << "_default" << endl;
                    }
                }
                i = openBraceIdx; continue;
            }
            if (lex.lexema == LEX_CASE) {
                if (!activeSwitches.empty()) {
                    int litIdx = tables.lextable.table[i + 1].idxTI;
                    int val = tables.idtable.table[litIdx].value.vint;
                    ofile << "switch_" << activeSwitches.top() << "_case_" << val << ":" << endl;
                }
                i++; continue;
            }
            if (lex.lexema == LEX_DEFAULT) {
                if (!activeSwitches.empty()) ofile << "switch_" << activeSwitches.top() << "_default:" << endl;
                continue;
            }
            if (lex.lexema == LEX_BRACELET) {
                if (!switchEndIndices.empty() && i == switchEndIndices.top()) {
                    ofile << "switch_end_" << activeSwitches.top() << ":" << endl;
                    activeSwitches.pop(); switchEndIndices.pop();
                }
                else if (!activeSwitches.empty()) ofile << "\tjmp switch_end_" << activeSwitches.top() << endl;
                continue;
            }

            if (lex.lexema == LEX_COUT) { isCout = true; continue; }

            if (lex.lexema == LEX_ID && (i + 1 < tables.lextable.size) && tables.lextable.table[i + 1].lexema == LEX_EQUAL) {
                targetIdIndex = lex.idxTI; i++; continue;
            }

            if (lex.lexema == LEX_LITERAL) {
                IT::Entry& lit = tables.idtable.table[lex.idxTI];
                lastType = lit.iddatatype;
                if (lit.iddatatype == IT::STR) ofile << "\tpush offset L" << lex.idxTI << endl;
                else { ofile << "\tmov al, L" << lex.idxTI << "\n\tmovsx eax, al\n\tpush eax" << endl; }
            }

            else if (lex.lexema == LEX_ID) {
                if (i > 0) {
                    unsigned char prev = tables.lextable.table[i - 1].lexema;
                    if (prev == LEX_INTEGER || prev == LEX_STRING || prev == LEX_CHAR) continue;
                }
                IT::Entry& id = tables.idtable.table[lex.idxTI];

                // ВАРИАНТ 1: ПОЛИЗ (@) - просто пропускаем ID функции
                if (id.idtype == IT::F) continue;

                // ВАРИАНТ 2: ИНФИКС (резервный, если ПОЛИЗ не сработал)
                // Если видим func(, то генерируем вызов
                bool isCall = false;
                if (id.idtype == IT::F || IsLibraryFunc((char*)id.id)) {
                    if (i + 1 < tables.lextable.size && tables.lextable.table[i + 1].lexema == LEX_LEFTTHESIS) isCall = true;
                }

                if (isCall) {
                    int k = i + 1; vector<int> argsIdx;
                    if (tables.lextable.table[k].lexema == LEX_LEFTTHESIS) {
                        k++;
                        while (tables.lextable.table[k].lexema != LEX_RIGHTTHESIS) {
                            if (tables.lextable.table[k].lexema == LEX_ID || tables.lextable.table[k].lexema == LEX_LITERAL) argsIdx.push_back(k);
                            k++;
                        }
                    }
                    for (int j = argsIdx.size() - 1; j >= 0; j--) {
                        int argLex = argsIdx[j];
                        int ti = tables.lextable.table[argLex].idxTI;
                        IT::Entry& argIt = tables.idtable.table[ti];
                        if (argIt.idtype == IT::L) {
                            if (argIt.iddatatype == IT::STR) ofile << "\tpush offset L" << ti << endl;
                            else ofile << "\tmov al, L" << ti << "\n\tmovsx eax, al\n\tpush eax" << endl;
                        }
                        else {
                            if (argIt.iddatatype == IT::STR) ofile << "\tpush " << argIt.id << endl;
                            else ofile << "\tmov al, " << argIt.id << "\n\tmovsx eax, al\n\tpush eax" << endl;
                        }
                    }
                    ofile << "\tcall " << id.id << endl;
                    ofile << "\tpush eax" << endl;
                    lastType = id.iddatatype;
                    if (strcmp((char*)id.id, "getstatus") == 0 || strcmp((char*)id.id, "getmessage") == 0) lastType = IT::STR;
                    if (strcmp((char*)id.id, "strtoint") == 0 || strcmp((char*)id.id, "stcmp") == 0) lastType = IT::INT;
                    i = k;
                    continue;
                }

                if (id.idtype == IT::V || id.idtype == IT::P) {
                    lastType = id.iddatatype;
                    if (id.iddatatype == IT::STR) ofile << "\tpush " << id.id << endl;
                    else ofile << "\tmov al, " << id.id << "\n\tmovsx eax, al\n\tpush eax" << endl;
                }
            }

            else if (lex.lexema == LEX_OPERATOR) {
                if (isCout) continue;
                ofile << "\tpop ebx\n\tpop eax" << endl;
                if (lex.op == LT::OPLUS) ofile << "\tadd eax, ebx" << endl;
                else if (lex.op == LT::OMINUS) ofile << "\tsub eax, ebx" << endl;
                else if (lex.op == LT::OMUL) ofile << "\timul eax, ebx" << endl;
                else if (lex.op == LT::ODIV) ofile << "\tcdq\n\tidiv ebx" << endl;
                else {
                    ofile << "\tcmp eax, ebx\n\tmov eax, 0" << endl;
                    if (lex.op == LT::OEQ) ofile << "\tsete al" << endl;
                    else if (lex.op == LT::ONE) ofile << "\tsetne al" << endl;
                    else if (lex.op == LT::OLESS) ofile << "\tsetl al" << endl;
                    else if (lex.op == LT::OMORE) ofile << "\tsetg al" << endl;
                }
                ofile << "\tpush eax" << endl; lastType = IT::INT;
            }

            // --- ПОЛИЗ ВЫЗОВ (@) ---
            else if (lex.lexema == '@') {
                int funcLexIdx = i - 1;
                // Ищем ID функции, пропуская аргументы (в ПОЛИЗЕ они перед @)
                // Но проблема в том, что аргументы могут быть сложными выражениями...
                // В твоей реализации ПОЛИЗА имя функции хранится в idxTI самого оператора @!
                // Смотрим PN.cpp: callOp.idxTI = idx_func;

                int funcIDIndex = lex.idxTI;
                if (funcIDIndex != LT_TI_NULLIDX) {
                    IT::Entry& func = tables.idtable.table[funcIDIndex];
                    ofile << "\tcall " << func.id << endl;
                    ofile << "\tpush eax" << endl;

                    lastType = func.iddatatype;
                    if (strcmp((char*)func.id, "getstatus") == 0 || strcmp((char*)func.id, "getmessage") == 0) lastType = IT::STR;
                    if (strcmp((char*)func.id, "strtoint") == 0 || strcmp((char*)func.id, "stcmp") == 0) lastType = IT::INT;
                }
            }

            else if (lex.lexema == LEX_RETURN) isReturn = true;
            else if (lex.lexema == LEX_SEMICOLON) {
                if (targetIdIndex != -1) {
                    ofile << "\tpop eax" << endl;
                    IT::Entry& tgt = tables.idtable.table[targetIdIndex];
                    if (tgt.iddatatype == IT::STR) ofile << "\tmov " << tgt.id << ", eax" << endl;
                    else ofile << "\tmov " << tgt.id << ", al" << endl;
                    targetIdIndex = -1;
                }
                else if (isCout) {
                    ofile << "\tpop eax" << endl;
                    if (lastType == IT::STR) ofile << "\tinvoke outstr, eax" << endl;
                    else ofile << "\tinvoke outnum, eax" << endl;
                    ofile << "\tinvoke newline" << endl;
                    isCout = false;
                }
                else if (isReturn) {
                    ofile << "\tpop eax" << endl << "\tret" << endl;
                }
            }
        }
    }

    void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log)
    {
        string asmPath = "ASM\\Asm.asm";
        ofstream ofile(asmPath);
        if (!ofile.is_open()) return;
        ofile << ASM_HEAD << endl << ASM_CONST;
        for (int i = 0; i < tables.idtable.size; i++) {
            IT::Entry& e = tables.idtable.table[i];
            if (e.idtype == IT::L) {
                ofile << "\tL" << i << "\t";
                if (e.iddatatype == IT::INT) ofile << "sbyte " << e.value.vint << endl;
                else if (e.iddatatype == IT::CHR) ofile << "sbyte " << (int)e.value.vchar << endl;
                else if (e.iddatatype == IT::STR) ofile << "db '" << (e.value.vstr.str ? (char*)e.value.vstr.str : "") << "', 0" << endl;
            }
        }
        ofile << ASM_DATA << "\tswitch_val sdword 0" << endl;
        for (int i = 0; i < tables.idtable.size; i++) {
            IT::Entry& e = tables.idtable.table[i];
            if (e.idtype == IT::V) { ofile << "\t" << e.id << "\t"; if (e.iddatatype == IT::STR) ofile << "dd 0" << endl; else ofile << "sbyte 0" << endl; }
        }
        ofile << ASM_CODE;
        for (int i = 0; i < tables.lextable.size; i++) {
            if (tables.lextable.table[i].lexema == LEX_FUNCTION) {
                int idIdx = tables.lextable.table[i + 1].idxTI;
                IT::Entry& funcId = tables.idtable.table[idIdx];
                if (IsLibraryFunc((char*)funcId.id)) {
                    int k = i;
                    while (k < tables.lextable.size) {
                        if (tables.lextable.table[k].lexema == LEX_LEFTBRACE) {
                            int balance = 1; k++;
                            while (k < tables.lextable.size && balance > 0) {
                                if (tables.lextable.table[k].lexema == LEX_LEFTBRACE) balance++;
                                if (tables.lextable.table[k].lexema == LEX_BRACELET) balance--;
                                k++;
                            } break;
                        } k++;
                    } i = k - 1; continue;
                }
                ofile << funcId.id << " PROC";
                int k = i + 2; bool first = true;
                while (tables.lextable.table[k].lexema != LEX_RIGHTTHESIS) {
                    if (tables.lextable.table[k].lexema == LEX_ID) {
                        if (first) { ofile << ", "; first = false; }
                        else ofile << ", ";
                        int pIdx = tables.lextable.table[k].idxTI;
                        ofile << tables.idtable.table[pIdx].id << " :DWORD";
                    } k++;
                }
                ofile << endl;
                while (tables.lextable.table[k].lexema != LEX_LEFTBRACE) k++;
                int startBody = k + 1, endBody = startBody, balance = 1;
                for (; endBody < tables.lextable.size; endBody++) {
                    if (tables.lextable.table[endBody].lexema == LEX_LEFTBRACE) balance++;
                    if (tables.lextable.table[endBody].lexema == LEX_BRACELET) { balance--; if (balance == 0) break; }
                }
                ProcessStatements(tables, ofile, startBody, endBody);
                ofile << "\tret\n" << funcId.id << " ENDP" << endl; i = endBody;
            }
        }
        ofile << "main PROC" << endl;
        int mainIdx = 0;
        while (mainIdx < tables.lextable.size && tables.lextable.table[mainIdx].lexema != LEX_MAIN) mainIdx++;
        while (mainIdx < tables.lextable.size && tables.lextable.table[mainIdx].lexema != LEX_LEFTBRACE) mainIdx++;
        ProcessStatements(tables, ofile, mainIdx + 1, tables.lextable.size - 1);
        ofile << "\tinvoke ExitProcess, 0" << endl << "main ENDP" << endl << "end main" << endl;
        ofile.close();
        cout << "\n[GENERATION] SUCCESS: " << asmPath << endl;
    }
}