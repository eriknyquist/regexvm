#ifndef RXVM_LFIX_H
#define RXVM_LFIX_H

void lfix_zero (void);
void lfix_start (char *orig, char *lp1);
void lfix_continue (char *orig, char *lp1);
unsigned int lfix_longest (void);
unsigned int lfix_current (void);
void lfix_set_longest (void);
void lfix_update_longest (void);
void lfix_stop (void);
void lfix_dec (void);
unsigned int get_lfix0 (void);
unsigned int get_lfixn (void);
void set_lfixn (unsigned int val);
void set_ltempn (unsigned int val);
unsigned int get_ltempn (void);

#endif
