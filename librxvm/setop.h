#ifndef RXVM_SETOP_H
#define RXVM_SETOP_H

void set_op_class (inst_t *inst, char *ccs);
void set_op_nclass (inst_t *inst, char *ccs);
void set_op_branch (inst_t *inst, int x, int y);
void set_op_jmp (inst_t *inst, int x);
void set_op_match (inst_t *inst);
void set_op_char (inst_t *inst, char c);
void set_op_any (inst_t *inst);
void set_op_sol (inst_t *inst);
void set_op_eol (inst_t *inst);

#endif
