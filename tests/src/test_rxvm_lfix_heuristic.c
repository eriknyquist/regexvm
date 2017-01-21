#include <stdio.h>
#include <stdlib.h>
#include "rxvm.h"
#include "test_common.h"

static int tests;

static void log_trs (char *msg, const char *func)
{
    fprintf(trsfp, ":test-result: %s %s #%d\n", msg, func, tests);
}

void verify_rxvm_lfix (char *regex, unsigned int lfix0,
                      unsigned int lfixn, const char *modname)
{
    char *msg;
    int ret;
    rxvm_t compiled;

    if ((ret = rxvm_compile(&compiled, regex)) < 0) {
        fprintf(logfp, "Failed to compile expression %s\n", regex);
        log_trs("FAIL", modname);
        return;
    }

    if (compiled.lfix0 != lfix0 || compiled.lfixn != lfixn) {
        fprintf(logfp, "\nFail: Incorrect longest-fixed-string markers "
                       "generated by rxvm_compile, for expression:\n%s\n\n",
                       regex);
        fprintf(logfp, "expected:\n\tlfix0 = %u (%c)\n\tlfixn = %u (%c)\n\n",
                       lfix0, regex[lfix0], lfixn, regex[lfixn]);
        fprintf(logfp, "seen:\n\tlfix0 = %u (%c)\n\tlfixn = %u (%c)\n\n",
                       compiled.lfix0, regex[compiled.lfix0], compiled.lfixn,
                       regex[compiled.lfixn]);
        msg = "FAIL";
    } else {
        msg = "PASS";
    }

    rxvm_free(&compiled);
    log_trs(msg, modname);
    printf("%s: %s #%i\n", msg, modname, ++tests);
}

void test_rxvm_lfix_heuristic (void)
{
    tests = 0;

    verify_rxvm_lfix("AaBb+", 0, 3, __func__);
    verify_rxvm_lfix("ab+c", 0, 1, __func__);
    verify_rxvm_lfix("abc?d", 0, 1, __func__);
    verify_rxvm_lfix("ab*c", 0, 0, __func__);

    verify_rxvm_lfix("abcd{,5}c", 0, 2, __func__);
    verify_rxvm_lfix("abcd{0,}c", 0, 2, __func__);
    verify_rxvm_lfix("abcd{1,}c", 0, 3, __func__);
    verify_rxvm_lfix("ab{123,}", 0, 1, __func__);
    verify_rxvm_lfix("ab{1234,}123", 9, 11, __func__);

    verify_rxvm_lfix("abc(defg)", 0, 2, __func__);
    verify_rxvm_lfix("abc(defg)hijk", 9, 12, __func__);

    verify_rxvm_lfix("abc(d|e)fghi", 8, 11, __func__);
    verify_rxvm_lfix("abc(de)hij|k", 0, 0, __func__);
    verify_rxvm_lfix("aaaaa|bbbbb", 0, 0, __func__);
    verify_rxvm_lfix("aaa|bbb|ccc|ddd", 0, 0, __func__);
    verify_rxvm_lfix("aaa", 0, 0, __func__);
    verify_rxvm_lfix("aaa\\*", 0, 0, __func__);

    verify_rxvm_lfix("abc(def)+ijkl", 9, 12, __func__);
    verify_rxvm_lfix("abc(def)+ijkl*", 0, 2, __func__);
    verify_rxvm_lfix("abc(def)+ijklm*", 9, 12, __func__);
    verify_rxvm_lfix("abcc(def)+ijkl", 0, 3, __func__);

    verify_rxvm_lfix("abc(d|(e?|f|g+)|h)ijkl", 18, 21, __func__);
    verify_rxvm_lfix("abc(d|(e?|f|g+)|h)ijkl*", 0, 2, __func__);
    verify_rxvm_lfix("abc(d|(e?|f|g+)|h)ijklm*", 18, 21, __func__);
    verify_rxvm_lfix("abcc(d|(e?|f|g+)|h)ijkl", 0, 3, __func__);

    verify_rxvm_lfix("abcd*efghi", 5, 9, __func__);
    verify_rxvm_lfix("abcd\nefghi", 0, 3, __func__);
    verify_rxvm_lfix("abcd[ \t\n]efghi", 0, 3, __func__);
    verify_rxvm_lfix("abcd[ \t]efghi", 8, 12, __func__);
    verify_rxvm_lfix("abcd(ef\n)?efghi", 0, 3, __func__);

    verify_rxvm_lfix("ab\\*\nefghi", 0, 2, __func__);
    verify_rxvm_lfix("ab\\*\n(e|fg)hi", 0, 2, __func__);
    verify_rxvm_lfix("ab\\*\\+\\?\nefghi", 0, 4, __func__);
    verify_rxvm_lfix("abc(def)*ghi\\+", 9, 12, __func__);

    verify_rxvm_lfix("abc[defgh]", 0, 2, __func__);
    verify_rxvm_lfix("abc[defgh]ijkl", 10, 13, __func__);
    verify_rxvm_lfix("abc[d\\]fgh]ijkl", 11, 14, __func__);
    verify_rxvm_lfix("abc[d\\]fgh]ij\\kl", 11, 14, __func__);
}
