#ifndef RXVM_STAGE1_H
#define RXVM_STAGE1_H

struct context {
    strb_t strb;
    ir_stack_t *target;
    ir_stack_t *buf;
    ir_stack_t *prog;
    stackitem_t *operand;
    ir_stack_t *parens;
    char *simple;
    char *orig;

    int tok;
    int lasttok;
    unsigned int depth;
    uint8_t chained;
};

int stage1 (rxvm_t *compiled, char *input, ir_stack_t **ret);

#endif
