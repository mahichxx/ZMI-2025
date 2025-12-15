#include "stdafx.h"
#include "Lex.h"
#include "FST.h"
#include "FST_def.h"
#include "Error.h"
#include <iostream>
#include <cstring>
#include <iomanip>

#define MAX_INTEGER 127
#define MIN_INTEGER -128
#define MAX_BYTE    255
#define MAX_STROKA  255
#define IN_CODE_DELIMETR '|'

using namespace std;

namespace Lex {

    bool checkBrace(unsigned char** word, int k) {
        while (word[k] != NULL && word[k][0] == IN_CODE_DELIMETR) { k++; }
        if (word[k] == NULL) return false;
        if (word[k][0] == '}') return true; 
        return false;
    }

    int BinToInt(unsigned char* word) {
        int sum = 0;
        for (int i = 2; word[i] != '\0'; i++) sum = (sum << 1) + (word[i] - '0');
        return sum;
    }

    int HexToInt(unsigned char* word) {
        int sum = 0;
        for (int i = 2; word[i] != '\0'; i++) {
            int digit = 0;
            unsigned char c = word[i];
            if (c >= '0' && c <= '9') digit = c - '0';
            else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
            else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
            sum = (sum << 4) + digit; 
        }
        return sum;
    }

    LEX lexAnaliz(Log::LOG log, In::IN in)
    {
        LEX lex;
        lex.lextable = LT::Create(LT_MAXSIZE);
        lex.idtable = IT::Create(TI_MAXSIZE);

        int line = 1;
        int indexID = 0;
        int countLit = 1;
        int position = 0;

        IT::Entry entryIT = {};
        IT::Entry bufentry = {}; 
        LT::Entry entryLT = {};
        
        char RegionPrefix[TI_STR_MAXSIZE] = { 0 };
        char buferRegionPrefix[TI_STR_MAXSIZE] = { 0 };
        char pastRegionPrefix[TI_STR_MAXSIZE] = { 0 };
        char bufL[TI_STR_MAXSIZE] = { 0 };
        char charCountLit[20] = { 0 };
        char tempBuf[TI_STR_MAXSIZE] = { 0 };

        bool findFunc = false;
        bool findParm = false;
        bool findReturn = false;
        bool endif = false;
        int Parm_count_IT = 0;
        int Idx_Func_IT = 0;
        int count_main = 0;

        IT::IDDATATYPE currentType = IT::INT; 

        unsigned char** word = in.word;

        try {
            for (int i = 0; word[i] != NULL && word[i][0] != '\0'; i++)
            {
                int wordLen = strlen((char*)word[i]);
                bool findSameID = false;

                if (word[i][0] == IN_CODE_DELIMETR) {
                    if (endif) {
                        LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_ENDIF, LT_TI_NULLIDX, line));
                        endif = false;
                    }
                    line++;
                    position = 0;
                    continue; 
                }

                // Сложные операторы
                bool isComplex = false;
                LT::operations complexOp = LT::operations::OEQ;
                int priority = 0;
                unsigned char firstChar = word[i][0];
                unsigned char secondChar = (word[i+1] != NULL) ? word[i+1][0] : 0;
                if (wordLen == 1 && word[i+1] != NULL && strlen((char*)word[i+1]) == 1 && secondChar == '=') {
                    if (firstChar == '<') { isComplex = true; complexOp = LT::operations::OLE; priority = 1; }
                    else if (firstChar == '>') { isComplex = true; complexOp = LT::operations::OGE; priority = 1; }
                    else if (firstChar == '=') { isComplex = true; complexOp = LT::operations::OEQ; priority = 1; }
                    else if (firstChar == '!') { isComplex = true; complexOp = LT::operations::ONE; priority = 1; }
                }
                if (isComplex) {
                    entryLT = LT::writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
                    entryLT.op = complexOp; entryLT.priority = priority;
                    LT::Add(lex.lextable, entryLT);
                    char opName[3] = { (char)firstChar, (char)secondChar, '\0' };
                    strcpy_s(entryIT.id, opName);
                    entryIT.idxfirstLE = lex.lextable.size - 1; entryIT.idtype = IT::OP;
                    IT::Add(lex.idtable, entryIT); entryIT = bufentry; i++; position += 2; continue;
                }

                // --- ТИПЫ ДАННЫХ ---
                { FST::FST fst(word[i], FST_INTEGER); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line)); currentType = IT::INT; position += wordLen; continue; }}
                { FST::FST fst(word[i], FST_TYPE_CHAR); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_CHAR, LT_TI_NULLIDX, line)); currentType = IT::CHR; position += wordLen; continue; }}
                { FST::FST fst(word[i], FST_STRING); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line)); currentType = IT::STR; position += wordLen; continue; }}
                { FST::FST fst(word[i], FST_VOID); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_VOID, LT_TI_NULLIDX, line)); currentType = IT::VOI; position += wordLen; continue; }}

                // --- КЛЮЧЕВЫЕ СЛОВА ---
                { FST::FST fst(word[i], FST_MAIN); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line)); count_main++; findReturn = false; strcpy_s(pastRegionPrefix, RegionPrefix); RegionPrefix[0] = '\0'; position += wordLen; continue; }}
                { FST::FST fst(word[i], FST_FUNCTION); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line)); entryIT.idtype = IT::F; findFunc = true; findParm = true; Parm_count_IT = 0; position += wordLen; continue; }}
                { FST::FST fst(word[i], FST_RETURN); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_RETURN, LT_TI_NULLIDX, line)); findReturn = true; position += wordLen; continue; } }
                { FST::FST fst(word[i], FST_COUT); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_COUT, LT_TI_NULLIDX, line)); position += wordLen; continue; } }
                { FST::FST fst(word[i], FST_SWITCH); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_SWITCH, LT_TI_NULLIDX, line)); position += wordLen; continue; } }
                { FST::FST fst(word[i], FST_CASE); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_CASE, LT_TI_NULLIDX, line)); position += wordLen; continue; } }
                { FST::FST fst(word[i], FST_DEFAULT); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_DEFAULT, LT_TI_NULLIDX, line)); position += wordLen; continue; } }
                { FST::FST fst(word[i], FST_IF); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_IF, LT_TI_NULLIDX, line)); endif = true; position += wordLen; continue; } }
                { FST::FST fst(word[i], FST_ELSE); if (FST::execute(fst)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_ELSE, LT_TI_NULLIDX, line)); position += wordLen; continue; } }

                // !!! ЛОГИЧЕСКИЕ ЛИТЕРАЛЫ КАК ОБЫЧНЫЕ ЧИСЛА (ЧТОБЫ ГРАММАТИКА НЕ ПАДАЛА) !!!
                {
                    FST::FST fst(word[i], FST_TRUE);
                    if (FST::execute(fst)) {
                        // Регистрируем как литерал 'l' со значением 1 (INT)
                        LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line));
                        entryIT.iddatatype = IT::INT; 
                        entryIT.idtype = IT::L;
                        entryIT.value.vint = 1;
                        strcpy_s(entryIT.id, "true_lit"); // Уникальное имя не важно, главное тип
                        IT::Add(lex.idtable, entryIT); 
                        entryIT = bufentry;
                        position += wordLen; continue;
                    }
                }
                {
                    FST::FST fst(word[i], FST_FALSE);
                    if (FST::execute(fst)) {
                        // Регистрируем как литерал 'l' со значением 0 (INT)
                        LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line));
                        entryIT.iddatatype = IT::INT;
                        entryIT.idtype = IT::L;
                        entryIT.value.vint = 0;
                        strcpy_s(entryIT.id, "false_lit");
                        IT::Add(lex.idtable, entryIT);
                        entryIT = bufentry;
                        position += wordLen; continue;
                    }
                }

                // --- ИДЕНТИФИКАТОРЫ ---
                FST::FST fstIdentif(word[i], FST_ID);
                if (FST::execute(fstIdentif)) {
                    if (wordLen > ID_MAXSIZE) throw ERROR_THROW_IN(202, line, position);
                    const char* forbidden[] = { "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "code", "data", "stack" };
                    for (const char* bad : forbidden) {
                        if (strcmp((char*)word[i], bad) == 0) throw ERROR_THROW(113);
                    }

                    int idx = TI_NULLIDX;
                    if (!findParm) {
                        idx = IT::IsId(lex.idtable, (char*)word[i]);
                        if (idx == TI_NULLIDX && !findFunc) {
                            strcpy_s(tempBuf, buferRegionPrefix); strcat_s(tempBuf, (char*)word[i]);
                            idx = IT::IsId(lex.idtable, tempBuf);
                        }
                    }

                    if (idx != TI_NULLIDX) {
                        LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_ID, idx, line));
                        findFunc = false; position += wordLen; continue;
                    }

                    LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_ID, indexID++, line));
                    entryIT.iddatatype = currentType;

                    if (findParm && !findFunc) {
                        entryIT.idtype = IT::P; 
                        Parm_count_IT++; 
                        strcpy_s(entryIT.id, (char*)word[i]);
                    } else if (!findFunc) {
                        entryIT.idtype = IT::V;
                        if (RegionPrefix[0] != '\0') {
                             strcpy_s(entryIT.id, RegionPrefix); strcat_s(entryIT.id, (char*)word[i]);
                        } else {
                             strcpy_s(entryIT.id, (char*)word[i]);
                        }
                    } else {
                        entryIT.idtype = IT::F;
                        strcpy_s(pastRegionPrefix, RegionPrefix);
                        strcpy_s(RegionPrefix, (char*)word[i]); 
                        strcpy_s(buferRegionPrefix, (char*)word[i]);
                        strcpy_s(entryIT.id, (char*)word[i]);
                    }

                    entryIT.idxfirstLE = lex.lextable.size - 1;
                    IT::Add(lex.idtable, entryIT);
                    if (findFunc) Idx_Func_IT = lex.idtable.size - 1;
                    
                    entryIT = bufentry; 
                    findFunc = false;
                    position += wordLen; continue;
                }

                // --- ЛИТЕРАЛЫ ---
                FST::FST fstInt(word[i], FST_INTLIT);
                if (FST::execute(fstInt)) {
                    int value = atoi((char*)word[i]);
                    bool isNegative = false;
                    if (lex.lextable.size > 0) {
                        LT::Entry prev = lex.lextable.table[lex.lextable.size - 1];
                        if (prev.lexema == LEX_OPERATOR && prev.op == LT::OMINUS) {
                            if (lex.lextable.size == 1) isNegative = true;
                            else {
                                unsigned char pp = lex.lextable.table[lex.lextable.size - 2].lexema;
                                if (pp == LEX_EQUAL || pp == LEX_LEFTTHESIS || pp == LEX_RETURN || pp == LEX_COMMA || pp == LEX_TWOPOINT) 
                                    isNegative = true;
                            }
                        }
                    }
                    if (isNegative) { value = -value; lex.lextable.size--; }
                    if (value > MAX_INTEGER || value < MIN_INTEGER) throw ERROR_THROW_IN(203, line, position);

                    for (int k = 0; k < lex.idtable.size; k++) {
                        if (lex.idtable.table[k].idtype == IT::L && lex.idtable.table[k].iddatatype == IT::INT && lex.idtable.table[k].value.vint == value && lex.idtable.table[k].numSys == 0) {
                            LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, k, line)); findSameID = true; break;
                        }
                    }
                    if (findSameID) { position += wordLen; continue; }
                    LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line));
                    entryIT.iddatatype = IT::INT; entryIT.idtype = IT::L; entryIT.value.vint = value; entryIT.numSys = 0;
                    _itoa_s(countLit++, charCountLit, 20, 10); strcpy_s(bufL, "L"); strcat_s(bufL, charCountLit); strcpy_s(entryIT.id, bufL);
                    IT::Add(lex.idtable, entryIT); entryIT = bufentry; position += wordLen; continue;
                }
                
                FST::FST fstHex(word[i], FST_INT16LIT); if (FST::execute(fstHex)) { int value = HexToInt(word[i]); LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line)); entryIT.iddatatype = IT::INT; entryIT.idtype = IT::L; entryIT.value.vint = value; entryIT.numSys = 1; _itoa_s(countLit++, charCountLit, 20, 10); strcpy_s(bufL, "L"); strcat_s(bufL, charCountLit); strcpy_s(entryIT.id, bufL); IT::Add(lex.idtable, entryIT); entryIT = bufentry; position += wordLen; continue; }
                FST::FST fstBin(word[i], FST_BINLIT); if (FST::execute(fstBin)) { int value = BinToInt(word[i]); LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line)); entryIT.iddatatype = IT::INT; entryIT.idtype = IT::L; entryIT.value.vint = value; entryIT.numSys = 2; _itoa_s(countLit++, charCountLit, 20, 10); strcpy_s(bufL, "L"); strcat_s(bufL, charCountLit); strcpy_s(entryIT.id, bufL); IT::Add(lex.idtable, entryIT); entryIT = bufentry; position += wordLen; continue; }
                
                // СТРОКИ С ПРОВЕРКОЙ ОШИБКИ 205
                FST::FST fstStr(word[i], FST_STRLIT); 
                if (FST::execute(fstStr)) { 
                    int len = strlen((char*)word[i]); 
                    for (int k = 0; k < len; k++) {
                        if (word[i][k] == '\'') throw ERROR_THROW_IN(205, line, position + k);
                    }
                    if (len == 2) {
                        throw ERROR_THROW_IN(310, line, position);
                    }
                    for(int k=0; k<len; k++) word[i][k] = word[i][k+1]; word[i][len-2] = '\0'; len -= 2; 
                    for(int k=0; k<lex.idtable.size; k++) if(lex.idtable.table[k].idtype == IT::L && lex.idtable.table[k].iddatatype == IT::STR && strcmp(lex.idtable.table[k].value.vstr.str, (char*)word[i]) == 0) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, k, line)); findSameID = true; break; } 
                    if(findSameID) { position += wordLen + 2; continue; } 
                    LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line)); 
                    entryIT.iddatatype = IT::STR; entryIT.idtype = IT::L; entryIT.value.vstr.len = len; strcpy_s(entryIT.value.vstr.str, (char*)word[i]); 
                    _itoa_s(countLit++, charCountLit, 20, 10); strcpy_s(bufL, "L"); strcat_s(bufL, charCountLit); strcpy_s(entryIT.id, bufL); 
                    IT::Add(lex.idtable, entryIT); entryIT = bufentry; position += wordLen + 2; continue; 
                }

                FST::FST fstCharLit(word[i], FST_CHARLIT); if (FST::execute(fstCharLit)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LITERAL, indexID++, line)); entryIT.iddatatype = IT::CHR; entryIT.idtype = IT::L; entryIT.value.vchar = word[i][1]; _itoa_s(countLit++, charCountLit, 20, 10); strcpy_s(bufL, "L"); strcat_s(bufL, charCountLit); strcpy_s(entryIT.id, bufL); IT::Add(lex.idtable, entryIT); entryIT = bufentry; position += wordLen; continue; }

                // --- ОПЕРАТОРЫ И РАЗДЕЛИТЕЛИ ---
                FST::FST fstOp(word[i], FST_OPERATOR);
                if (FST::execute(fstOp)) {
                    LT::Entry lt = LT::writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
                    unsigned char op = word[i][0];

                    switch (op) {
                    case '+': lt.op = LT::operations::OPLUS; lt.priority = 2; break;
                    case '-': lt.op = LT::operations::OMINUS; lt.priority = 2; break;
                    case '*': lt.op = LT::operations::OMUL; lt.priority = 3; break;
                    case '/': lt.op = LT::operations::ODIV; lt.priority = 3; break;
                    case '%': lt.op = LT::operations::OMOD; lt.priority = 3; break;

                        // !!! УМНАЯ ОБРАБОТКА < !!!
                    case '<':
                        // Если предыдущая лексема была 'show' (LEX_COUT), значит это ВЫВОД
                        if (lex.lextable.size > 0 && lex.lextable.table[lex.lextable.size - 1].lexema == LEX_COUT) {
                            lt.op = LT::operations::OLESS; // Операция вывода
                            lt.priority = 1;
                        }
                        else {
                            lt.op = LT::operations::OLESS_CMP; // Операция сравнения
                            lt.priority = 1;
                        }
                        break;

                    case '>': lt.op = LT::operations::OMORE; lt.priority = 1; break;
                    case '=': lt.lexema = LEX_EQUAL; break; // Присваивание
                    }

                    LT::Add(lex.lextable, lt);

                    // Добавляем в таблицу идентификаторов (как оператор)
                    strcpy_s(entryIT.id, (char*)word[i]);
                    entryIT.idxfirstLE = lex.lextable.size - 1;
                    entryIT.idtype = IT::OP;
                    IT::Add(lex.idtable, entryIT);

                    entryIT = bufentry; // Сброс буфера
                    position += strlen((char*)word[i]);
                    continue;
                }
                { FST::FST f(word[i], FST_SEMICOLON); if(FST::execute(f)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line)); position+=wordLen; continue;}}
                { FST::FST f(word[i], FST_COMMA); if(FST::execute(f)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line)); position+=wordLen; continue;}}
                { FST::FST f(word[i], FST_LEFTBRACE); if(FST::execute(f)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line)); position+=wordLen; continue;}}
                { FST::FST f(word[i], FST_BRACELET); if(FST::execute(f)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line)); position+=wordLen; continue;}}
                { FST::FST f(word[i], FST_LEFTTHESIS); if(FST::execute(f)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line)); position+=wordLen; continue;}}
                
                { FST::FST f(word[i], FST_RIGHTTHESIS); if(FST::execute(f)) { 
                    LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line));
                    if (findParm) { lex.idtable.table[Idx_Func_IT].parm = Parm_count_IT; findParm = false; Parm_count_IT = 0; Idx_Func_IT = 0; }
                    position+=wordLen; continue;
                }}
                
                { FST::FST f(word[i], FST_TWOPOINT); if (FST::execute(f)) { LT::Add(lex.lextable, LT::writeEntry(entryLT, LEX_TWOPOINT, LT_TI_NULLIDX, line)); position += wordLen; continue; } }

                // !!! ПРОВЕРКА НА КИРИЛЛИЦУ (Ошибка 206) !!!
                // Если слово не распознано и содержит символы с кодом < 0 (это русские буквы в char),
                // то выдаем специфичную ошибку.
                for (int k = 0; k < wordLen; k++) {
                    // (unsigned char)word[i][k] > 127 означает, что это не стандартный ASCII
                    if ((unsigned char)word[i][k] > 127) {
                        throw ERROR_THROW_IN(206, line, position);
                    }
                }
                if (word[i][0] == '\'') {
                    throw ERROR_THROW_IN(207, line, position);
                }
                // Иначе стандартная ошибка 201
                cout << "DEBUG: Ошибка 201. Слово: " << word[i] << endl;
                throw ERROR_THROW_IN(201, line, position);
            }
        }
        catch (Error::ERROR e) { IT::Delete(lex.idtable); LT::Delete(lex.lextable); throw e; }
        return lex;
    }
}