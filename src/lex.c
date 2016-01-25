#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

#define PRINTABLE_LOW      ' '  /* ASCII 0x20 */
#define PRINTABLE_HIGH     '~'  /* ASCII 0x7E */

char *lp1;
char *lpn;

enum {STATE_START, STATE_LITERAL, STATE_RANGE, STATE_CLASS, STATE_DEREF,
      STATE_END};

static unsigned int literal;

static inline int isreserved (char x)
{
    return (x == CHARC_OPEN_SYM || x == CHARC_CLOSE_SYM) ? 1 : 0;
}

static inline int isprintable (char x)
{
    return (x >= PRINTABLE_LOW && x <= PRINTABLE_HIGH) ? 1 : 0;
}

char *get_token_text (void)
{
    char *ret;
    size_t size;
    unsigned int i;

    size = lpn - lp1;
    ret = malloc(size + 1);
    for (i = 0; i < size; i++)
        ret[i] = lp1[i];

    ret[size] = '\0';
    return ret;
}

int simple_transition (int literal, char **input, int tok, int *ret)
{
    int state;

    if (literal) {
        state = STATE_LITERAL;
    } else {
        state = STATE_END;
        *ret = tok;
        (*input)++;
    }

    return state;
}

int lex (char **input)
{
    int state = STATE_START;
    int ret = INVALIDSYM;

    if (!**input) return END;

    lp1 = *input;
    while (state != STATE_END && **input) {
        switch (state) {
            case STATE_START:
                if (**input == DEREF_SYM) {
                    state = STATE_DEREF;
                    (*input)++;

                } else if (**input == CHARC_OPEN_SYM) {
                    state = STATE_END;
                    ret = CHARC_OPEN;
                    literal = 1;
                    (*input)++;

                } else if (**input == CHARC_CLOSE_SYM) {
                    state = STATE_END;
                    ret = CHARC_CLOSE;
                    literal = 0;
                    (*input)++;

                } else if (**input == LPAREN_SYM) {
                    state = simple_transition(literal, input, LPAREN, &ret);
                } else if (**input == RPAREN_SYM) {
                    state = simple_transition(literal, input, RPAREN, &ret);
                } else if (**input == ONE_SYM) {
                    state = simple_transition(literal, input, ONE, &ret);
                } else if (**input == ONEZERO_SYM) {
                    state = simple_transition(literal, input, ONEZERO, &ret);
                } else if (**input == ZERO_SYM) {
                    state = simple_transition(literal, input, ZERO, &ret);
                } else if (**input == ANY_SYM) {
                    state = simple_transition(literal, input, ANY, &ret);
                } else if (**input == CONCAT_SYM) {
                    state = simple_transition(literal, input, CONCAT, &ret);
                } else if (isprintable(**input)) {
                    state = STATE_LITERAL;
                } else {
                    state = STATE_END;
                    ret = INVALIDSYM;
                    (*input)++;
                }

            break;

            case STATE_LITERAL:
                if (**input == RANGE_SEP_SYM && literal) {
                    state = STATE_END;
                    ret = INVALIDSYM;
                    (*input)++;
                } else if (*(*input + 1) == RANGE_SEP_SYM && literal) {
                    state = STATE_RANGE;
                    *input += 2;
                } else {
                    state = STATE_END;
                    ret = LITERAL;
                    (*input)++;
                }

            break;

            case STATE_DEREF:
                if (!**input) {
                    ret = INVALIDSYM;
                } else {
                    ret = LITERAL;
                    lp1++;
                }

                (*input)++;
                state = STATE_END;
            break;

            case STATE_RANGE:
                if (isprintable(**input) && !isreserved(**input)) {
                    state = STATE_END;
                    ret = CHAR_RANGE;
                    (*input)++;
                } else {
                    state = STATE_END;
                    ret = INVALIDSYM;
                }

            break;
        }
    }

    lpn = *input;
    return ret;
}
