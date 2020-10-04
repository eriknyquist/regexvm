#ifndef RXVM_STRING_BUILDER_H
#define RXVM_STRING_BUILDER_H

typedef struct strb strb_t;

struct strb {
    unsigned long int size;
    unsigned long int space;

    char *buf;
    unsigned int block_size;
};

int strb_init (strb_t *cfg, unsigned int block_size);
int strb_addc (strb_t *cfg, char c);
int strb_adds (strb_t *cfg, char *s, int n);
int strb_addu (strb_t *cfg, unsigned int i);

#endif
