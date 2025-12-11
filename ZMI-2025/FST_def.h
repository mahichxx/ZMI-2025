#pragma once
#include <stddef.h>
#pragma region auto

// =========================================================
//                 ТИПЫ ДАННЫХ
// =========================================================

#define FST_INTEGER 8,	\
	FST::NODE(1, FST::RELATION('i', 1)),\
	FST::NODE(1, FST::RELATION('n', 2)),\
	FST::NODE(1, FST::RELATION('t', 3)),\
	FST::NODE(1, FST::RELATION('e', 4)),\
	FST::NODE(1, FST::RELATION('g', 5)),\
	FST::NODE(1, FST::RELATION('e', 6)),\
	FST::NODE(1, FST::RELATION('r', 7)),\
	FST::NODE()

#define FST_STRING 7,	\
	FST::NODE(1, FST::RELATION('s', 1)),\
	FST::NODE(1, FST::RELATION('t', 2)),\
	FST::NODE(1, FST::RELATION('r', 3)),\
	FST::NODE(1, FST::RELATION('i', 4)),\
	FST::NODE(1, FST::RELATION('n', 5)),\
	FST::NODE(1, FST::RELATION('g', 6)),\
	FST::NODE()

#define FST_TYPE_CHAR 5, \
	FST::NODE(1, FST::RELATION('c', 1)),\
	FST::NODE(1, FST::RELATION('h', 2)),\
	FST::NODE(1, FST::RELATION('a', 3)),\
	FST::NODE(1, FST::RELATION('r', 4)),\
	FST::NODE()

#define FST_VOID 5,	\
	FST::NODE(1, FST::RELATION('v', 1)),\
	FST::NODE(1, FST::RELATION('o', 2)),\
	FST::NODE(1, FST::RELATION('i', 3)),\
	FST::NODE(1, FST::RELATION('d', 4)),\
	FST::NODE()

// =========================================================
//                 КЛЮЧЕВЫЕ СЛОВА
// =========================================================

#define FST_MAIN 5,	\
	FST::NODE(1, FST::RELATION('m', 1)),\
	FST::NODE(1, FST::RELATION('a', 2)),\
	FST::NODE(1, FST::RELATION('i', 3)),\
	FST::NODE(1, FST::RELATION('n', 4)),\
	FST::NODE()

#define FST_FUNCTION 9,	\
	FST::NODE(1, FST::RELATION('f', 1)),\
	FST::NODE(1, FST::RELATION('u', 2)),\
	FST::NODE(1, FST::RELATION('n', 3)),\
	FST::NODE(1, FST::RELATION('c', 4)),\
	FST::NODE(1, FST::RELATION('t', 5)),\
	FST::NODE(1, FST::RELATION('i', 6)),\
	FST::NODE(1, FST::RELATION('o', 7)),\
	FST::NODE(1, FST::RELATION('n', 8)),\
	FST::NODE()

#define FST_RETURN 7,	\
	FST::NODE(1, FST::RELATION('r', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('t', 3)),\
	FST::NODE(1, FST::RELATION('u', 4)),\
	FST::NODE(1, FST::RELATION('r', 5)),\
	FST::NODE(1, FST::RELATION('n', 6)),\
	FST::NODE()

#define FST_SWITCH 7, \
    FST::NODE(1, FST::RELATION('s', 1)), \
    FST::NODE(1, FST::RELATION('w', 2)), \
    FST::NODE(1, FST::RELATION('i', 3)), \
    FST::NODE(1, FST::RELATION('t', 4)), \
    FST::NODE(1, FST::RELATION('c', 5)), \
    FST::NODE(1, FST::RELATION('h', 6)), \
    FST::NODE()

#define FST_CASE 5, \
    FST::NODE(1, FST::RELATION('c', 1)), \
    FST::NODE(1, FST::RELATION('a', 2)), \
    FST::NODE(1, FST::RELATION('s', 3)), \
    FST::NODE(1, FST::RELATION('e', 4)), \
    FST::NODE()

#define FST_DEFAULT 8, \
    FST::NODE(1, FST::RELATION('d', 1)), \
    FST::NODE(1, FST::RELATION('e', 2)), \
    FST::NODE(1, FST::RELATION('f', 3)), \
    FST::NODE(1, FST::RELATION('a', 4)), \
    FST::NODE(1, FST::RELATION('u', 5)), \
    FST::NODE(1, FST::RELATION('l', 6)), \
    FST::NODE(1, FST::RELATION('t', 7)), \
    FST::NODE()

#define FST_COUT 5, \
    FST::NODE(1, FST::RELATION('c', 1)), \
    FST::NODE(1, FST::RELATION('o', 2)), \
    FST::NODE(1, FST::RELATION('u', 3)), \
    FST::NODE(1, FST::RELATION('t', 4)), \
    FST::NODE()

#define FST_TRUE 5, \
    FST::NODE(1, FST::RELATION('t', 1)), \
    FST::NODE(1, FST::RELATION('r', 2)), \
    FST::NODE(1, FST::RELATION('u', 3)), \
    FST::NODE(1, FST::RELATION('e', 4)), \
    FST::NODE()

#define FST_FALSE 6, \
    FST::NODE(1, FST::RELATION('f', 1)), \
    FST::NODE(1, FST::RELATION('a', 2)), \
    FST::NODE(1, FST::RELATION('l', 3)), \
    FST::NODE(1, FST::RELATION('s', 4)), \
    FST::NODE(1, FST::RELATION('e', 5)), \
    FST::NODE()

#define FST_IF 3, \
	FST::NODE(1, FST::RELATION('i', 1)), \
	FST::NODE(1, FST::RELATION('f', 2)), \
	FST::NODE()

#define FST_ELSE 5, \
	FST::NODE(1, FST::RELATION('e', 1)), \
	FST::NODE(1, FST::RELATION('l', 2)), \
	FST::NODE(1, FST::RELATION('s', 3)), \
	FST::NODE(1, FST::RELATION('e', 4)), \
	FST::NODE()

// =========================================================
//                 ЛИТЕРАЛЫ
// =========================================================

// Decimal (123)
#define FST_INTLIT 2,	\
	FST::NODE(20,	\
	FST::RELATION('1', 0), FST::RELATION('2', 0), FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0), FST::RELATION('6', 0),\
	FST::RELATION('7', 0), FST::RELATION('8', 0), FST::RELATION('9', 0), FST::RELATION('0', 0),\
	\
	FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1),\
	FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1)),\
	FST::NODE()

// Binary (0b101) - ИСПРАВЛЕНО
#define FST_BINLIT 4, \
    FST::NODE(1, FST::RELATION('0', 1)), \
    FST::NODE(1, FST::RELATION('b', 2)), \
    FST::NODE(4, FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('0', 3), FST::RELATION('1', 3)), \
    FST::NODE()

// Hex (0x1A)
#define FST_INT16LIT 4,	\
	FST::NODE(1, FST::RELATION('0', 1)),\
	FST::NODE(1, FST::RELATION('x', 2)),\
	FST::NODE(44,	\
	FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2),\
	FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2),\
	FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2),\
	FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),\
    /* Переход в финальное состояние (3) */ \
    FST::RELATION('A', 3), FST::RELATION('B', 3), FST::RELATION('C', 3), FST::RELATION('D', 3), FST::RELATION('E', 3), FST::RELATION('F', 3),\
	FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3),\
	FST::RELATION('7', 3), FST::RELATION('8', 3), FST::RELATION('9', 3), FST::RELATION('0', 3),\
	FST::RELATION('a', 3), FST::RELATION('b', 3), FST::RELATION('c', 3), FST::RELATION('d', 3), FST::RELATION('e', 3), FST::RELATION('f', 3)),\
	FST::NODE()

// Char literal ('a')
#define FST_CHARLIT 4, \
    FST::NODE(1, FST::RELATION('\'', 1)), \
    FST::NODE(256, \
        FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), \
        FST::RELATION('f', 2), FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), \
        FST::RELATION('k', 2), FST::RELATION('l', 2), FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), \
        FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2), FST::RELATION('s', 2), FST::RELATION('t', 2), \
        FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2), FST::RELATION('y', 2), \
        FST::RELATION('z', 2), FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), \
        FST::RELATION('E', 2), FST::RELATION('F', 2), FST::RELATION('G', 2), FST::RELATION('H', 2), FST::RELATION('I', 2), \
        FST::RELATION('J', 2), FST::RELATION('K', 2), FST::RELATION('L', 2), FST::RELATION('M', 2), FST::RELATION('N', 2), \
        FST::RELATION('O', 2), FST::RELATION('P', 2), FST::RELATION('Q', 2), FST::RELATION('R', 2), FST::RELATION('S', 2), \
        FST::RELATION('T', 2), FST::RELATION('U', 2), FST::RELATION('V', 2), FST::RELATION('W', 2), FST::RELATION('X', 2), \
        FST::RELATION('Y', 2), FST::RELATION('Z', 2), \
        FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), \
        FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), \
        FST::RELATION('9', 2), FST::RELATION(' ', 2), FST::RELATION('!', 2), FST::RELATION('?', 2), FST::RELATION('.', 2) \
        ), \
    FST::NODE(1, FST::RELATION('\'', 3)), \
    FST::NODE()

// String literal
#define FST_STRLIT 3,	\
	FST::NODE(1, FST::RELATION('"', 1)),\
	FST::NODE(96,	\
    FST::RELATION('"', 2), \
    FST::RELATION(' ', 1), \
	FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), \
    FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), \
    FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), \
    FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), \
    FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), \
    FST::RELATION('z', 1), \
    FST::RELATION('A', 1), FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1), FST::RELATION('E', 1), \
    FST::RELATION('F', 1), FST::RELATION('G', 1), FST::RELATION('H', 1), FST::RELATION('I', 1), FST::RELATION('J', 1), \
    FST::RELATION('K', 1), FST::RELATION('L', 1), FST::RELATION('M', 1), FST::RELATION('N', 1), FST::RELATION('O', 1), \
    FST::RELATION('P', 1), FST::RELATION('Q', 1), FST::RELATION('R', 1), FST::RELATION('S', 1), FST::RELATION('T', 1), \
    FST::RELATION('U', 1), FST::RELATION('U', 2), FST::RELATION('V', 1), FST::RELATION('V', 2), FST::RELATION('W', 1), FST::RELATION('X', 1), \
    FST::RELATION('Y', 1), FST::RELATION('Z', 1), \
    FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), \
    FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), \
    FST::RELATION('0', 1), \
    FST::RELATION('!', 1), FST::RELATION('?', 1), FST::RELATION(',', 1), \
    FST::RELATION('.', 1), FST::RELATION('-', 1), FST::RELATION('+', 1), FST::RELATION('=', 1), FST::RELATION('*', 1), \
    FST::RELATION('/', 1), FST::RELATION('>', 1), FST::RELATION('<', 1), FST::RELATION('_', 1), \
    FST::RELATION(':', 1), FST::RELATION(';', 1), FST::RELATION('(', 1), FST::RELATION(')', 1), \
    FST::RELATION('[', 1), FST::RELATION(']', 1), FST::RELATION('{', 1), FST::RELATION('}', 1), \
    FST::RELATION('%', 1), FST::RELATION('@', 1), FST::RELATION('#', 1), FST::RELATION('$', 1), \
    FST::RELATION('^', 1), FST::RELATION('&', 1), FST::RELATION('|', 1), FST::RELATION('~', 1), \
    FST::RELATION('\'', 1), FST::RELATION('\\', 1), FST::RELATION('`', 1) \
    ), \
	FST::NODE()

// =========================================================
//                 ИДЕНТИФИКАТОРЫ
// =========================================================

#define FST_ID 3,	\
	FST::NODE(104,	\
	FST::RELATION('a', 1), FST::RELATION('a', 2), FST::RELATION('b', 1), FST::RELATION('b', 2), \
	FST::RELATION('c', 1), FST::RELATION('c', 2), FST::RELATION('d', 1), FST::RELATION('d', 2), \
	FST::RELATION('e', 1), FST::RELATION('e', 2), FST::RELATION('f', 1), FST::RELATION('f', 2),\
	FST::RELATION('g', 1), FST::RELATION('g', 2), FST::RELATION('h', 1), FST::RELATION('h', 2), \
	FST::RELATION('i', 1), FST::RELATION('i', 2), FST::RELATION('j', 1), FST::RELATION('j', 2), \
	FST::RELATION('k', 1), FST::RELATION('k', 2), FST::RELATION('l', 1), FST::RELATION('l', 2),\
	FST::RELATION('m', 1), FST::RELATION('m', 2), FST::RELATION('n', 1), FST::RELATION('n', 2), \
	FST::RELATION('o', 1), FST::RELATION('o', 2), FST::RELATION('p', 1), FST::RELATION('p', 2), \
	FST::RELATION('q', 1), FST::RELATION('q', 2), FST::RELATION('r', 1), FST::RELATION('r', 2),\
	FST::RELATION('s', 1), FST::RELATION('s', 2), FST::RELATION('t', 1), FST::RELATION('t', 2), \
	FST::RELATION('u', 1), FST::RELATION('u', 2), FST::RELATION('v', 1), FST::RELATION('v', 2), \
	FST::RELATION('w', 1), FST::RELATION('w', 2), FST::RELATION('x', 1), FST::RELATION('x', 2),\
	FST::RELATION('y', 1), FST::RELATION('y', 2), FST::RELATION('z', 1), FST::RELATION('z', 2),\
	\
	FST::RELATION('A', 1), FST::RELATION('A', 2), FST::RELATION('B', 1), FST::RELATION('B', 2), \
	FST::RELATION('C', 1), FST::RELATION('C', 2), FST::RELATION('D', 1), FST::RELATION('D', 2), \
	FST::RELATION('E', 1), FST::RELATION('E', 2), FST::RELATION('F', 1), FST::RELATION('F', 2),\
	FST::RELATION('G', 1), FST::RELATION('G', 2), FST::RELATION('H', 1), FST::RELATION('H', 2), \
	FST::RELATION('I', 1), FST::RELATION('I', 2), FST::RELATION('J', 1), FST::RELATION('J', 2), \
	FST::RELATION('K', 1), FST::RELATION('K', 2), FST::RELATION('L', 1), FST::RELATION('L', 2),\
	FST::RELATION('M', 1), FST::RELATION('M', 2), FST::RELATION('N', 1), FST::RELATION('N', 2), \
	FST::RELATION('O', 1), FST::RELATION('O', 2), FST::RELATION('P', 1), FST::RELATION('P', 2), \
	FST::RELATION('Q', 1), FST::RELATION('Q', 2), FST::RELATION('R', 1), FST::RELATION('R', 2),\
	FST::RELATION('S', 1), FST::RELATION('S', 2), FST::RELATION('T', 1), FST::RELATION('T', 2), \
	FST::RELATION('U', 1), FST::RELATION('U', 2), FST::RELATION('V', 1), FST::RELATION('V', 2), \
	FST::RELATION('W', 1), FST::RELATION('W', 2), FST::RELATION('X', 1), FST::RELATION('X', 2),\
	FST::RELATION('Y', 1), FST::RELATION('Y', 2), FST::RELATION('Z', 1), FST::RELATION('Z', 2),\
	\
	FST::RELATION('1', 1), FST::RELATION('1', 2), FST::RELATION('2', 1), FST::RELATION('2', 2), \
	FST::RELATION('3', 1), FST::RELATION('3', 2), FST::RELATION('4', 1), FST::RELATION('4', 2), \
	FST::RELATION('5', 1), FST::RELATION('5', 2), FST::RELATION('6', 1), FST::RELATION('6', 2),\
	FST::RELATION('7', 1), FST::RELATION('7', 2), FST::RELATION('8', 1), FST::RELATION('8', 2), \
	FST::RELATION('9', 1), FST::RELATION('9', 2), FST::RELATION('0', 1), FST::RELATION('0', 2),\
	FST::RELATION('_', 1), FST::RELATION('_', 2)),\
	FST::NODE(126,	\
	FST::RELATION('a', 1), FST::RELATION('a', 2), FST::RELATION('b', 1), FST::RELATION('b', 2), \
	FST::RELATION('c', 1), FST::RELATION('c', 2), FST::RELATION('d', 1), FST::RELATION('d', 2), \
	FST::RELATION('e', 1), FST::RELATION('e', 2), FST::RELATION('f', 1), FST::RELATION('f', 2),\
	FST::RELATION('g', 1), FST::RELATION('g', 2), FST::RELATION('h', 1), FST::RELATION('h', 2), \
	FST::RELATION('i', 1), FST::RELATION('i', 2), FST::RELATION('j', 1), FST::RELATION('j', 2), \
	FST::RELATION('k', 1), FST::RELATION('k', 2), FST::RELATION('l', 1), FST::RELATION('l', 2),\
	FST::RELATION('m', 1), FST::RELATION('m', 2), FST::RELATION('n', 1), FST::RELATION('n', 2), \
	FST::RELATION('o', 1), FST::RELATION('o', 2), FST::RELATION('p', 1), FST::RELATION('p', 2), \
	FST::RELATION('q', 1), FST::RELATION('q', 2), FST::RELATION('r', 1), FST::RELATION('r', 2),\
	FST::RELATION('s', 1), FST::RELATION('s', 2), FST::RELATION('t', 1), FST::RELATION('t', 2), \
	FST::RELATION('u', 1), FST::RELATION('u', 2), FST::RELATION('v', 1), FST::RELATION('v', 2), \
	FST::RELATION('w', 1), FST::RELATION('w', 2), FST::RELATION('x', 1), FST::RELATION('x', 2),\
	FST::RELATION('y', 1), FST::RELATION('y', 2), FST::RELATION('z', 1), FST::RELATION('z', 2),\
	\
	FST::RELATION('A', 1), FST::RELATION('A', 2), FST::RELATION('B', 1), FST::RELATION('B', 2), \
	FST::RELATION('C', 1), FST::RELATION('C', 2), FST::RELATION('D', 1), FST::RELATION('D', 2), \
	FST::RELATION('E', 1), FST::RELATION('E', 2), FST::RELATION('F', 1), FST::RELATION('F', 2),\
	FST::RELATION('G', 1), FST::RELATION('G', 2), FST::RELATION('H', 1), FST::RELATION('H', 2), \
	FST::RELATION('I', 1), FST::RELATION('I', 2), FST::RELATION('J', 1), FST::RELATION('J', 2), \
	FST::RELATION('K', 1), FST::RELATION('K', 2), FST::RELATION('L', 1), FST::RELATION('L', 2),\
	FST::RELATION('M', 1), FST::RELATION('M', 2), FST::RELATION('N', 1), FST::RELATION('N', 2), \
	FST::RELATION('O', 1), FST::RELATION('O', 2), FST::RELATION('P', 1), FST::RELATION('P', 2), \
	FST::RELATION('Q', 1), FST::RELATION('Q', 2), FST::RELATION('R', 1), FST::RELATION('R', 2),\
	FST::RELATION('S', 1), FST::RELATION('S', 2), FST::RELATION('T', 1), FST::RELATION('T', 2), \
	FST::RELATION('U', 1), FST::RELATION('U', 2), FST::RELATION('V', 1), FST::RELATION('V', 2), \
	FST::RELATION('W', 1), FST::RELATION('W', 2), FST::RELATION('X', 1), FST::RELATION('X', 2),\
	FST::RELATION('Y', 1), FST::RELATION('Y', 2), FST::RELATION('Z', 1), FST::RELATION('Z', 2),\
	\
	FST::RELATION('1', 1), FST::RELATION('1', 2), FST::RELATION('2', 1), FST::RELATION('2', 2), \
	FST::RELATION('3', 1), FST::RELATION('3', 2), FST::RELATION('4', 1), FST::RELATION('4', 2), \
	FST::RELATION('5', 1), FST::RELATION('5', 2), FST::RELATION('6', 1), FST::RELATION('6', 2),\
	FST::RELATION('7', 1), FST::RELATION('7', 2), FST::RELATION('8', 1), FST::RELATION('8', 2), \
	FST::RELATION('9', 1), FST::RELATION('9', 2), FST::RELATION('0', 1), FST::RELATION('0', 2),\
	FST::RELATION('_', 1), FST::RELATION('_', 2)),\
	FST::NODE()

// =========================================================
//                 ОПЕРАТОРЫ И РАЗДЕЛИТЕЛИ
// =========================================================

#define FST_OPERATOR 2,	\
	FST::NODE(9, FST::RELATION('+', 1), FST::RELATION('-', 1), FST::RELATION('*', 1), FST::RELATION('/', 1), \
                 FST::RELATION('%', 1), FST::RELATION('<', 1), FST::RELATION('>', 1), FST::RELATION('=', 1), FST::RELATION('!', 1)), \
	FST::NODE()

// Убрали < и > отсюда, чтобы не было конфликтов
#define FST_LOGOPERATOR 2,  \
	FST::NODE(2, FST::RELATION('!', 1), FST::RELATION('~', 1)), \
	FST::NODE()

#define FST_SEMICOLON 2,	\
	FST::NODE(1, FST::RELATION(';', 1)),\
	FST::NODE()

#define FST_COMMA 2,	\
	FST::NODE(1, FST::RELATION(',', 1)),\
	FST::NODE()

#define FST_LEFTBRACE 2,	\
	FST::NODE(1, FST::RELATION('{', 1)),\
	FST::NODE()

#define FST_BRACELET 2,	\
	FST::NODE(1, FST::RELATION('}', 1)),\
	FST::NODE()

#define FST_LEFTTHESIS 2,	\
	FST::NODE(1, FST::RELATION('(', 1)),\
	FST::NODE()

#define FST_RIGHTTHESIS 2,	\
	FST::NODE(1, FST::RELATION(')', 1)),\
	FST::NODE()

#define FST_EQUAL 2,	\
	FST::NODE(1, FST::RELATION('=', 1)),\
	FST::NODE()

#define FST_TWOPOINT 2, \
	FST::NODE(1, FST::RELATION(':', 1)),\
	FST::NODE()

// Заглушки для старого кода
#define FST_LITERALSTRING_1 FST_STRLIT
#define FST_PLUS FST_OPERATOR
#define FST_MINUS FST_OPERATOR
#define FST_STAR FST_OPERATOR
#define FST_DIRSLASH FST_OPERATOR
#define FST_SMALL FST_OPERATOR
#define FST_BIG FST_OPERATOR
#define FST_AND FST_OPERATOR
#define FST_EXCL FST_OPERATOR
#define FST_TILDA FST_OPERATOR
#define FST_ROOF FST_OPERATOR

#pragma endregion