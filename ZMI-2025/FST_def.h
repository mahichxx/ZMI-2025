#pragma once
#include <stddef.h>

// =========================================================
//                 “»œ€ ƒ¿ÕÕ€’
// =========================================================

// integer -> byte
#define FST_INTEGER 5,	\
	FST::NODE(1, FST::RELATION('b', 1)),\
	FST::NODE(1, FST::RELATION('y', 2)),\
	FST::NODE(1, FST::RELATION('t', 3)),\
	FST::NODE(1, FST::RELATION('e', 4)),\
	FST::NODE()

// string -> text
#define FST_STRING 5,	\
	FST::NODE(1, FST::RELATION('t', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('x', 3)),\
	FST::NODE(1, FST::RELATION('t', 4)),\
	FST::NODE()

// char (ÏÓÊÌÓ ËÒÔÓÎ¸ÁÓ‚‡Ú¸ Í‡Í sym ËÎË char)
#define FST_TYPE_CHAR 5, \
	FST::NODE(1, FST::RELATION('c', 1)),\
	FST::NODE(1, FST::RELATION('h', 2)),\
	FST::NODE(1, FST::RELATION('a', 3)),\
	FST::NODE(1, FST::RELATION('r', 4)),\
	FST::NODE()

// void
#define FST_VOID 5,	\
	FST::NODE(1, FST::RELATION('v', 1)),\
	FST::NODE(1, FST::RELATION('o', 2)),\
	FST::NODE(1, FST::RELATION('i', 3)),\
	FST::NODE(1, FST::RELATION('d', 4)),\
	FST::NODE()

// =========================================================
//                  Àﬁ◊≈¬€≈ —ÀŒ¬¿
// =========================================================

// main -> box
#define FST_MAIN 4,	\
	FST::NODE(1, FST::RELATION('b', 1)),\
	FST::NODE(1, FST::RELATION('o', 2)),\
	FST::NODE(1, FST::RELATION('x', 3)),\
	FST::NODE()

// function -> proc
#define FST_FUNCTION 5,	\
	FST::NODE(1, FST::RELATION('p', 1)),\
	FST::NODE(1, FST::RELATION('r', 2)),\
	FST::NODE(1, FST::RELATION('o', 3)),\
	FST::NODE(1, FST::RELATION('c', 4)),\
	FST::NODE()

// return -> ret
#define FST_RETURN 4,	\
	FST::NODE(1, FST::RELATION('r', 1)),\
	FST::NODE(1, FST::RELATION('e', 2)),\
	FST::NODE(1, FST::RELATION('t', 3)),\
	FST::NODE()

// switch -> check
#define FST_SWITCH 6, \
    FST::NODE(1, FST::RELATION('c', 1)), \
    FST::NODE(1, FST::RELATION('h', 2)), \
    FST::NODE(1, FST::RELATION('e', 3)), \
    FST::NODE(1, FST::RELATION('c', 4)), \
    FST::NODE(1, FST::RELATION('k', 5)), \
    FST::NODE()

// case -> is
#define FST_CASE 3, \
    FST::NODE(1, FST::RELATION('i', 1)), \
    FST::NODE(1, FST::RELATION('s', 2)), \
    FST::NODE()

// default -> else
#define FST_DEFAULT 5, \
    FST::NODE(1, FST::RELATION('e', 1)), \
    FST::NODE(1, FST::RELATION('l', 2)), \
    FST::NODE(1, FST::RELATION('s', 3)), \
    FST::NODE(1, FST::RELATION('e', 4)), \
    FST::NODE()

// cout -> show
#define FST_COUT 5, \
    FST::NODE(1, FST::RELATION('s', 1)), \
    FST::NODE(1, FST::RELATION('h', 2)), \
    FST::NODE(1, FST::RELATION('o', 3)), \
    FST::NODE(1, FST::RELATION('w', 4)), \
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

// if (ÓÒÚ‡‚ËÏ ‰Îˇ ÒÓ‚ÏÂÒÚËÏÓÒÚË)
#define FST_IF 3, \
	FST::NODE(1, FST::RELATION('i', 1)), \
	FST::NODE(1, FST::RELATION('f', 2)), \
	FST::NODE()

// else (‰Îˇ if, ÒÓ‚Ô‡‰‡ÂÚ Ò default)
#define FST_ELSE 5, \
	FST::NODE(1, FST::RELATION('e', 1)), \
	FST::NODE(1, FST::RELATION('l', 2)), \
	FST::NODE(1, FST::RELATION('s', 3)), \
	FST::NODE(1, FST::RELATION('e', 4)), \
	FST::NODE()

// =========================================================
//                 À»“≈–¿À€
// =========================================================

#define FST_INTLIT 2,	\
	FST::NODE(20,	\
	FST::RELATION('1', 0), FST::RELATION('2', 0), FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0), FST::RELATION('6', 0),\
	FST::RELATION('7', 0), FST::RELATION('8', 0), FST::RELATION('9', 0), FST::RELATION('0', 0),\
	\
	FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1),\
	FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1)),\
	FST::NODE()

#define FST_BINLIT 4, \
    FST::NODE(1, FST::RELATION('0', 1)), \
    FST::NODE(1, FST::RELATION('b', 2)), \
    FST::NODE(4, FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('0', 3), FST::RELATION('1', 3)), \
    FST::NODE()

#define FST_INT16LIT 4,	\
	FST::NODE(1, FST::RELATION('0', 1)),\
	FST::NODE(1, FST::RELATION('x', 2)),\
	FST::NODE(44,	\
	FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2),\
	FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2),\
	FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2),\
	FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),\
    FST::RELATION('A', 3), FST::RELATION('B', 3), FST::RELATION('C', 3), FST::RELATION('D', 3), FST::RELATION('E', 3), FST::RELATION('F', 3),\
	FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3),\
	FST::RELATION('7', 3), FST::RELATION('8', 3), FST::RELATION('9', 3), FST::RELATION('0', 3),\
	FST::RELATION('a', 3), FST::RELATION('b', 3), FST::RELATION('c', 3), FST::RELATION('d', 3), FST::RELATION('e', 3), FST::RELATION('f', 3)),\
	FST::NODE()

// !!! —»Ã¬ŒÀ€ (CHAR) — œŒƒƒ≈–∆ Œ… –”—— Œ√Œ !!!
#define FST_CHARLIT 4, \
    FST::NODE(1, FST::RELATION('\'', 1)), \
    FST::NODE(322, \
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
        FST::RELATION('9', 2), FST::RELATION(' ', 2), FST::RELATION('!', 2), FST::RELATION('?', 2), FST::RELATION('.', 2), \
        /* ---  »–»ÀÀ»÷¿ (¿-ﬂ, ‡-ˇ, ®, ∏) --- */ \
        FST::RELATION('¿', 2), FST::RELATION('¡', 2), FST::RELATION('¬', 2), FST::RELATION('√', 2), FST::RELATION('ƒ', 2), \
        FST::RELATION('≈', 2), FST::RELATION('®', 2), FST::RELATION('∆', 2), FST::RELATION('«', 2), FST::RELATION('»', 2), \
        FST::RELATION('…', 2), FST::RELATION(' ', 2), FST::RELATION('À', 2), FST::RELATION('Ã', 2), FST::RELATION('Õ', 2), \
        FST::RELATION('Œ', 2), FST::RELATION('œ', 2), FST::RELATION('–', 2), FST::RELATION('—', 2), FST::RELATION('“', 2), \
        FST::RELATION('”', 2), FST::RELATION('‘', 2), FST::RELATION('’', 2), FST::RELATION('÷', 2), FST::RELATION('◊', 2), \
        FST::RELATION('ÿ', 2), FST::RELATION('Ÿ', 2), FST::RELATION('⁄', 2), FST::RELATION('€', 2), FST::RELATION('‹', 2), \
        FST::RELATION('›', 2), FST::RELATION('ﬁ', 2), FST::RELATION('ﬂ', 2), \
        FST::RELATION('‡', 2), FST::RELATION('·', 2), FST::RELATION('‚', 2), FST::RELATION('„', 2), FST::RELATION('‰', 2), \
        FST::RELATION('Â', 2), FST::RELATION('∏', 2), FST::RELATION('Ê', 2), FST::RELATION('Á', 2), FST::RELATION('Ë', 2), \
        FST::RELATION('È', 2), FST::RELATION('Í', 2), FST::RELATION('Î', 2), FST::RELATION('Ï', 2), FST::RELATION('Ì', 2), \
        FST::RELATION('Ó', 2), FST::RELATION('Ô', 2), FST::RELATION('', 2), FST::RELATION('Ò', 2), FST::RELATION('Ú', 2), \
        FST::RELATION('Û', 2), FST::RELATION('Ù', 2), FST::RELATION('ı', 2), FST::RELATION('ˆ', 2), FST::RELATION('˜', 2), \
        FST::RELATION('¯', 2), FST::RELATION('˘', 2), FST::RELATION('˙', 2), FST::RELATION('˚', 2), FST::RELATION('¸', 2), \
        FST::RELATION('˝', 2), FST::RELATION('˛', 2), FST::RELATION('ˇ', 2) \
        ), \
    FST::NODE(1, FST::RELATION('\'', 3)), \
    FST::NODE()

// !!! —“–Œ » (STRING) — œŒƒƒ≈–∆ Œ… –”—— Œ√Œ !!!
#define FST_STRLIT 3,	\
	FST::NODE(1, FST::RELATION('"', 1)),\
	FST::NODE(162,	\
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
    FST::RELATION('\'', 1), FST::RELATION('\\', 1), FST::RELATION('`', 1), \
    /* ---  »–»ÀÀ»÷¿ ¬ —“–Œ ¿’ --- */ \
    FST::RELATION('¿', 1), FST::RELATION('¡', 1), FST::RELATION('¬', 1), FST::RELATION('√', 1), FST::RELATION('ƒ', 1), \
    FST::RELATION('≈', 1), FST::RELATION('®', 1), FST::RELATION('∆', 1), FST::RELATION('«', 1), FST::RELATION('»', 1), \
    FST::RELATION('…', 1), FST::RELATION(' ', 1), FST::RELATION('À', 1), FST::RELATION('Ã', 1), FST::RELATION('Õ', 1), \
    FST::RELATION('Œ', 1), FST::RELATION('œ', 1), FST::RELATION('–', 1), FST::RELATION('—', 1), FST::RELATION('“', 1), \
    FST::RELATION('”', 1), FST::RELATION('‘', 1), FST::RELATION('’', 1), FST::RELATION('÷', 1), FST::RELATION('◊', 1), \
    FST::RELATION('ÿ', 1), FST::RELATION('Ÿ', 1), FST::RELATION('⁄', 1), FST::RELATION('€', 1), FST::RELATION('‹', 1), \
    FST::RELATION('›', 1), FST::RELATION('ﬁ', 1), FST::RELATION('ﬂ', 1), \
    FST::RELATION('‡', 1), FST::RELATION('·', 1), FST::RELATION('‚', 1), FST::RELATION('„', 1), FST::RELATION('‰', 1), \
    FST::RELATION('Â', 1), FST::RELATION('∏', 1), FST::RELATION('Ê', 1), FST::RELATION('Á', 1), FST::RELATION('Ë', 1), \
    FST::RELATION('È', 1), FST::RELATION('Í', 1), FST::RELATION('Î', 1), FST::RELATION('Ï', 1), FST::RELATION('Ì', 1), \
    FST::RELATION('Ó', 1), FST::RELATION('Ô', 1), FST::RELATION('', 1), FST::RELATION('Ò', 1), FST::RELATION('Ú', 1), \
    FST::RELATION('Û', 1), FST::RELATION('Ù', 1), FST::RELATION('ı', 1), FST::RELATION('ˆ', 1), FST::RELATION('˜', 1), \
    FST::RELATION('¯', 1), FST::RELATION('˘', 1), FST::RELATION('˙', 1), FST::RELATION('˚', 1), FST::RELATION('¸', 1), \
    FST::RELATION('˝', 1), FST::RELATION('˛', 1), FST::RELATION('ˇ', 1) \
    ), \
	FST::NODE()

// =========================================================
//                 »ƒ≈Õ“»‘» ¿“Œ–€
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
//                 Œœ≈–¿“Œ–€ » –¿«ƒ≈À»“≈À»
// =========================================================

#define FST_OPERATOR 2,	\
	FST::NODE(9, FST::RELATION('+', 1), FST::RELATION('-', 1), FST::RELATION('*', 1), FST::RELATION('/', 1), \
                 FST::RELATION('%', 1), FST::RELATION('<', 1), FST::RELATION('>', 1), FST::RELATION('=', 1), FST::RELATION('!', 1)), \
	FST::NODE()

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

// «‡„ÎÛ¯ÍË
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