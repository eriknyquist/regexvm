#ifndef RXVM_LEX_H
#define RXVM_LEX_H

#define PRINTABLE_LOW        ' '  /* ASCII 0x20 */
#define PRINTABLE_HIGH       '~'  /* ASCII 0x7E */
#define WS_LOW               '\a' /* ASCII 0x07 */
#define WS_HIGH              '\r' /* ASCII 0x0D */

#define CHARC_OPEN_SYM       '['
#define CHARC_CLOSE_SYM      ']'
#define LPAREN_SYM           '('
#define RPAREN_SYM           ')'
#define REP_OPEN_SYM         '{'
#define REP_CLOSE_SYM        '}'
#define RANGE_SEP_SYM        '-'
#define ONE_SYM              '+'
#define ZERO_SYM             '*'
#define ANY_SYM              '.'
#define ONEZERO_SYM          '?'
#define ALT_SYM              '|'
#define SOL_SYM              '^'
#define EOL_SYM              '$'
#define DEREF_SYM            '\\'

extern const unsigned int ws_diff;
extern const unsigned int printable_diff;

#define isprintable(x) (((unsigned) (x - PRINTABLE_LOW) <= printable_diff \
                          || (unsigned) (x - WS_LOW) <= ws_diff) ? 1 : 0)

/* lexer return tokens */
enum {CHARC_OPEN, CHARC_CLOSE, CHAR_RANGE, LPAREN, RPAREN, ONE,
      ZERO, ONEZERO, ALT, ANY, LITERAL, REP, SOL, EOL, INVALIDSYM, END};

void lex_init (void);
int lex (char **input);

#endif
