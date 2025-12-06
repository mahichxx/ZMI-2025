#include "stdafx.h"
#include "Generation.h"
#include "LT.h"
#include "IT.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

namespace Gener
{
    // Шапка ASM-файла
    const string ASM_HEAD =
        ".586\n"
        ".model flat, stdcall\n"
        "includelib libucrt.lib\n"
        "includelib kernel32.lib\n"
        "includelib \"../Debug/InLib.lib\"\n"
        "ExitProcess PROTO :DWORD\n"
        "outnum PROTO :SDWORD\n"
        "outstr PROTO :DWORD\n"
        "newline PROTO\n"
        "\n.stack 4096\n"
        ".const\n";

    const string ASM_DATA = ".data\n";
    const string ASM_CODE = ".code\n";

    void CodeGeneration(Lex::LEX& tables, Parm::PARM& parm, Log::LOG& log)
    {
        string asmPath = "Asm.asm";
        ofstream ofile(asmPath);

        if (!ofile.is_open())
        {
            cout << "[GENERATION] Не удалось создать файл, пробую дефолтный путь..." << endl;
            ofile.open("Asm.asm");
        }

        // 1. ЗАГОЛОВОК
        ofile << ASM_HEAD;

        for (int i = 0; i < tables.idtable.size; i++)
        {
            IT::Entry& e = tables.idtable.table[i];
            if (e.idtype == IT::L) // Литералы
            {
                ofile << "\tL" << i << "\t";
                if (e.iddatatype == IT::INT)
                    ofile << "sdword " << e.value.vint << endl;
                else if (e.iddatatype == IT::CHR)
                    ofile << "sdword " << (int)e.value.vchar << endl;
                else if (e.iddatatype == IT::STR)
                    ofile << "db '" << e.value.vstr.str << "', 0" << endl;
            }
        }

        // 2. ДАННЫЕ
        ofile << ASM_DATA;
        ofile << "\ttemp sdword ?" << endl;
        ofile << "\tswitch_val sdword ?" << endl;

        for (int i = 0; i < tables.idtable.size; i++)
        {
            IT::Entry& e = tables.idtable.table[i];
            if (e.idtype == IT::V) // Переменные
            {
                ofile << "\t" << e.id << "\t";
                if (e.iddatatype == IT::STR)
                    ofile << "dd 0" << endl;
                else
                    ofile << "sdword 0" << endl;
            }
        }

        // 3. КОД
        ofile << ASM_CODE;
        ofile << "main PROC" << endl;

        bool inMain = false;
        int targetIdIndex = -1;

        for (int i = 0; i < tables.lextable.size; i++)
        {
            LT::Entry& lex = tables.lextable.table[i];

            if (lex.lexema == LEX_MAIN) { inMain = true; continue; }
            if (!inMain) continue;
            if (lex.lexema == '}') continue;

            // Присваивание
            if (lex.lexema == LEX_ID && (i + 1 < tables.lextable.size) && tables.lextable.table[i + 1].lexema == LEX_EQUAL)
            {
                targetIdIndex = lex.idxTI;
                i++;
                continue;
            }

            // PUSH
            if (lex.lexema == LEX_LITERAL)
            {
                IT::Entry& lit = tables.idtable.table[lex.idxTI];
                if (lit.iddatatype == IT::STR) ofile << "\tpush offset L" << lex.idxTI << endl;
                else                           ofile << "\tpush L" << lex.idxTI << endl;
            }
            else if (lex.lexema == LEX_ID)
            {
                IT::Entry& id = tables.idtable.table[lex.idxTI];
                if (id.idtype != IT::F) ofile << "\tpush " << id.id << endl;
            }

            // OPERATORS
            else if (lex.lexema == LEX_OPERATOR)
            {
                if (lex.op == LT::OLESS || lex.op == LT::OEQ)
                {
                    ofile << "\tpop ebx\n\tpop eax\n\tcmp eax, ebx\n\tmov eax, 0" << endl;
                    if (lex.op == LT::OLESS) ofile << "\tsetl al" << endl;
                    if (lex.op == LT::OEQ)   ofile << "\tsete al" << endl;
                    ofile << "\tpush eax" << endl;
                }
                else if (lex.op == LT::OPLUS) { ofile << "\tpop ebx\n\tpop eax\n\tadd eax, ebx\n\tpush eax" << endl; }
                else if (lex.op == LT::OMINUS) { ofile << "\tpop ebx\n\tpop eax\n\tsub eax, ebx\n\tpush eax" << endl; }
                else if (lex.op == LT::OMUL) { ofile << "\tpop ebx\n\tpop eax\n\timul eax, ebx\n\tpush eax" << endl; }
                else if (lex.op == LT::ODIV) { ofile << "\tpop ebx\n\tpop eax\n\tcdq\n\tidiv ebx\n\tpush eax" << endl; }
            }

            // SEMICOLON (Execution/Output)
            else if (lex.lexema == LEX_SEMICOLON)
            {
                if (targetIdIndex != -1) {
                    ofile << "\tpop eax" << endl;
                    ofile << "\tmov " << tables.idtable.table[targetIdIndex].id << ", eax" << endl;
                    targetIdIndex = -1;
                }
                else {
                    // Считаем, что это вывод (cout)
                    ofile << "\tpop eax" << endl;
                    ofile << "\tinvoke outnum, eax" << endl;
                    ofile << "\tinvoke newline" << endl;
                }
            }

            // SWITCH stub
            else if (lex.lexema == LEX_SWITCH) {
                ofile << "\t; Switch logic start (simplified)" << endl;
            }
        }

        ofile << "\tinvoke ExitProcess, 0" << endl;
        ofile << "main ENDP" << endl;
        ofile << "end main" << endl;
        ofile.close();
    }
}