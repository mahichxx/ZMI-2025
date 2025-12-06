#include "stdafx.h"
#include "Generation.h"
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace Gener
{
    const string ASM_HEAD =
        ".586\n"
        ".model flat, stdcall\n"
        // --- Библиотеки (Debug версии) ---
        "includelib msvcrtd.lib\n"
        "includelib ucrtd.lib\n"
        "includelib vcruntimed.lib\n"
        "includelib legacy_stdio_definitions.lib\n"
        "includelib kernel32.lib\n"
        // --- ТВОЙ ПУТЬ К БИБЛИОТЕКЕ ---
        "includelib \"D:\\Программирование\\3_сем\\КПО\\ZMI-2025\\Debug\\InLib.lib\"\n"

        "ExitProcess PROTO :DWORD\n"
        "outnum PROTO :SDWORD\n"
        "outstr PROTO :DWORD\n"
        "newline PROTO\n"
        "stcmp PROTO :DWORD, :DWORD\n"
        "strtoint PROTO :DWORD\n"
        "\n.stack 4096\n"
        ".const\n";

    const string ASM_DATA = ".data\n";
    const string ASM_CODE = ".code\n";

    void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log)
    {
        string asmPath = "ASM\\Asm.asm";
        ofstream ofile(asmPath);

        cout << "--- [DEBUG] ЗАПУЩЕН НОВЫЙ ГЕНЕРАТОР ---" << endl;

        if (!ofile.is_open()) return;

        // 1. ЗАГОЛОВОК
        ofile << ASM_HEAD;

        // Литералы
        for (int i = 0; i < tables.idtable.size; i++)
        {
            IT::Entry& e = tables.idtable.table[i];
            if (e.idtype == IT::L)
            {
                ofile << "\tL" << i << "\t";
                if (e.iddatatype == IT::INT)
                    ofile << "sbyte " << e.value.vint << endl;
                else if (e.iddatatype == IT::CHR)
                    ofile << "sbyte " << (int)e.value.vchar << endl;
                else if (e.iddatatype == IT::STR)
                    ofile << "db '" << (e.value.vstr.str ? (char*)e.value.vstr.str : "") << "', 0" << endl;
            }
        }

        // 2. ДАННЫЕ
        ofile << ASM_DATA;
        ofile << "\tswitch_val sdword 0" << endl;

        for (int i = 0; i < tables.idtable.size; i++)
        {
            IT::Entry& e = tables.idtable.table[i];
            if (e.idtype == IT::V)
            {
                ofile << "\t" << e.id << "\t";
                if (e.iddatatype == IT::STR) ofile << "dd 0" << endl;
                else ofile << "sbyte 0" << endl;
            }
        }

        // 3. КОД
        ofile << ASM_CODE;
        ofile << "main PROC" << endl;

        bool inMain = false;
        int targetIdIndex = -1;
        bool isCout = false; // Флаг: мы внутри cout?

        // Переменные для Switch
        bool pendingSwitch = false;
        int switchLabelIndex = 0;
        vector<int> activeSwitchIDs;

        for (int i = 0; i < tables.lextable.size; i++)
        {
            LT::Entry& lex = tables.lextable.table[i];

            if (lex.lexema == LEX_MAIN) { inMain = true; continue; }
            if (!inMain) continue;

            // Обработка закрывающей скобки
            if (lex.lexema == '}')
            {
                // Если это конец кейса (перед скобкой была точка с запятой)
                if (!activeSwitchIDs.empty() && i > 0 && tables.lextable.table[i - 1].lexema == LEX_SEMICOLON) {
                    ofile << "\tjmp switch_end_" << activeSwitchIDs.back() << endl;
                }
                continue;
            }

            // --- ОТСЛЕЖИВАНИЕ COUT ---
            if (lex.lexema == LEX_COUT) {
                isCout = true;
                continue;
            }

            // --- ПРИСВАИВАНИЕ (ID =) ---
            if (lex.lexema == LEX_ID && (i + 1 < tables.lextable.size) && tables.lextable.table[i + 1].lexema == LEX_EQUAL)
            {
                targetIdIndex = lex.idxTI;
                i++;
                continue;
            }

            // --- PUSH (Операнды) ---
            if (lex.lexema == LEX_LITERAL)
            {
                IT::Entry& lit = tables.idtable.table[lex.idxTI];
                if (lit.iddatatype == IT::STR) ofile << "\tpush offset L" << lex.idxTI << endl;
                else {
                    ofile << "\tmov al, L" << lex.idxTI << endl;
                    ofile << "\tmovsx eax, al" << endl;
                    ofile << "\tpush eax" << endl;
                }
            }
            else if (lex.lexema == LEX_ID)
            {
                // Пропускаем объявления переменных (integer x;)
                if (i > 0) {
                    unsigned char prev = tables.lextable.table[i - 1].lexema;
                    if (prev == LEX_INTEGER || prev == LEX_STRING || prev == LEX_CHAR) continue;
                }

                IT::Entry& id = tables.idtable.table[lex.idxTI];
                if (id.idtype != IT::F) {
                    if (id.iddatatype == IT::INT || id.iddatatype == IT::CHR || id.iddatatype == IT::BOOL) {
                        ofile << "\tmov al, " << id.id << endl;
                        ofile << "\tmovsx eax, al" << endl;
                        ofile << "\tpush eax" << endl;
                    }
                    else ofile << "\tpush " << id.id << endl;
                }
                else {
                    if (string((char*)id.id) == "cmp") { ofile << "\tcall stcmp" << endl; ofile << "\tpush eax" << endl; }
                    else if (string((char*)id.id) == "atoi") { ofile << "\tcall strtoint" << endl; ofile << "\tpush eax" << endl; }
                }
            }

            // --- ОПЕРАТОРЫ ---
            else if (lex.lexema == LEX_OPERATOR)
            {
                // !!! ВАЖНО: Если мы внутри cout, пропускаем оператор '<' !!!
                if (isCout) continue;

                ofile << "\tpop ebx\n\tpop eax" << endl;
                if (lex.op == LT::OPLUS)       ofile << "\tadd eax, ebx" << endl;
                else if (lex.op == LT::OMINUS) ofile << "\tsub eax, ebx" << endl;
                else if (lex.op == LT::OMUL)   ofile << "\timul eax, ebx" << endl;
                else if (lex.op == LT::ODIV)   ofile << "\tcdq\n\tidiv ebx" << endl;
                else if (lex.op == LT::OMOD)   ofile << "\tcdq\n\tidiv ebx\n\tmov eax, edx" << endl;
                else {
                    ofile << "\tcmp eax, ebx\n\tmov eax, 0" << endl;
                    if (lex.op == LT::OEQ)        ofile << "\tsete al" << endl;
                    else if (lex.op == LT::ONE)   ofile << "\tsetne al" << endl;
                    else if (lex.op == LT::OLESS) ofile << "\tsetl al" << endl;
                    else if (lex.op == LT::OMORE) ofile << "\tsetg al" << endl;
                    else if (lex.op == LT::OLE)   ofile << "\tsetle al" << endl;
                    else if (lex.op == LT::OGE)   ofile << "\tsetge al" << endl;
                }
                ofile << "\tpush eax" << endl;
            }

            // --- SEMICOLON (Точка с запятой) ---
            else if (lex.lexema == LEX_SEMICOLON)
            {
                // 1. Присваивание
                if (targetIdIndex != -1) {
                    ofile << "\tpop eax" << endl;
                    IT::Entry& tgt = tables.idtable.table[targetIdIndex];
                    if (tgt.iddatatype == IT::INT || tgt.iddatatype == IT::CHR) ofile << "\tmov " << tgt.id << ", al" << endl;
                    else ofile << "\tmov " << tgt.id << ", eax" << endl;
                    targetIdIndex = -1;
                    // ЗДЕСЬ НЕТ ВЫВОДА! Просто сохранили значение.
                }
                // 2. Вывод (ТОЛЬКО если был cout)
                else if (isCout) {
                    ofile << "\tpop eax" << endl;

                    // Проверяем тип данных для правильного вывода
                    int opIdx = -1;
                    if (i > 0) opIdx = tables.lextable.table[i - 1].idxTI;

                    // Если строка -> outstr, иначе -> outnum
                    if (opIdx != -1 && tables.idtable.table[opIdx].iddatatype == IT::STR) {
                        ofile << "\tinvoke outstr, eax" << endl;
                    }
                    else {
                        ofile << "\tinvoke outnum, eax" << endl;
                    }

                    ofile << "\tinvoke newline" << endl;
                    isCout = false;
                }
            }

            // --- SWITCH ЛОГИКА ---
            else if (lex.lexema == LEX_SWITCH) {
                pendingSwitch = true;
            }
            else if (lex.lexema == LEX_LEFTBRACE && pendingSwitch) {
                pendingSwitch = false;
                switchLabelIndex++;
                activeSwitchIDs.push_back(switchLabelIndex);
                int currentID = switchLabelIndex;

                ofile << "\t; --- SWITCH START ID:" << currentID << " ---" << endl;
                ofile << "\tpop eax" << endl;
                ofile << "\tmov switch_val, eax" << endl;

                int braceBalance = 1;
                bool hasDefault = false;
                for (int k = i + 1; k < tables.lextable.size; k++) {
                    if (tables.lextable.table[k].lexema == LEX_LEFTBRACE) braceBalance++;
                    if (tables.lextable.table[k].lexema == '}') braceBalance--;
                    if (braceBalance == 0) break;

                    if (tables.lextable.table[k].lexema == LEX_CASE) {
                        int litIdx = tables.lextable.table[k + 1].idxTI;
                        int val = tables.idtable.table[litIdx].value.vint;
                        ofile << "\tmov eax, switch_val" << endl;
                        ofile << "\tcmp eax, " << val << endl;
                        ofile << "\tje switch_" << currentID << "_case_" << val << endl;
                    }
                    if (tables.lextable.table[k].lexema == LEX_DEFAULT) {
                        hasDefault = true;
                    }
                }

                if (hasDefault) ofile << "\tjmp switch_" << currentID << "_default" << endl;
                else ofile << "\tjmp switch_end_" << currentID << endl;
            }

            else if (lex.lexema == LEX_CASE) {
                int currentID = activeSwitchIDs.back();
                int litIdx = tables.lextable.table[i + 1].idxTI;
                int val = tables.idtable.table[litIdx].value.vint;
                ofile << "switch_" << currentID << "_case_" << val << ":" << endl;
                i += 2;
            }

            else if (lex.lexema == LEX_DEFAULT) {
                int currentID = activeSwitchIDs.back();
                ofile << "switch_" << currentID << "_default:" << endl;
                i += 1;
            }
        }

        for (int id : activeSwitchIDs) {
            ofile << "switch_end_" << id << ":" << endl;
        }

        ofile << "\tinvoke ExitProcess, 0" << endl;
        ofile << "main ENDP" << endl;
        ofile << "end main" << endl;

        ofile.close();
        cout << "\n[GENERATION] ASM-файл успешно сгенерирован: " << asmPath << endl;
    }
}