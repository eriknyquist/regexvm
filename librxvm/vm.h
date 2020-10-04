#ifndef RXVM_VM_H
#define RXVM_VM_H

extern const int udiff;

#define tolower(x) (((x - 'A') <= udiff) ? x + 32 : x)
#define char_match(icase, a, b) ((icase) ? tolower(a) == tolower(b) : a == b)

typedef struct threads threads_t;

struct threads {
    int64_t chars;          /* Number of input chars processed */
    int64_t match_start;    /* Start of matching portion of input stream */
    int64_t match_end;      /* End of matching portion of input stream */
    int *cp;                /* Threads for current input char. */
    int *np;                /* Threads for next input char. */
    uint8_t *cp_lookup;     /* Lookup table for current input char. */
    uint8_t *np_lookup;     /* Lookup table for next input char. */
    uint8_t *table_base;    /* Base address for memory area containing tables */
    char (*getchar)(void*); /* Ptr to function for getting next input char */
    void *getchar_data;     /* Data for getchar function */
    char endchar;           /* The char that signals the end of input */
    char lastinput;         /* Save previous input char to allow back-peek */
    int csize;              /* No. of threads queued for current input char. */
    int nsize;              /* No. of threads queued for next input. char. */

    /* Flags */
    uint8_t icase;
    uint8_t nongreedy;
    uint8_t multiline;
    uint8_t search;
};

int vm_init (threads_t *tm, unsigned int size);
int vm_execute (threads_t *tm, rxvm_t *compiled, int64_t input_limit);
void vm_cleanup (threads_t *tm);

#endif
