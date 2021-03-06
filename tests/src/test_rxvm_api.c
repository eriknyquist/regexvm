#include <stdio.h>
#include <stdlib.h>
#include "rxvm.h"
#include "test_common.h"

static const char *func;
static int count;

static void log_trs (char *msg, const char *func)
{
    fprintf(trsfp, ":test-result: %s %s #%d\n", msg, func, count);
}

static void verify_search_flags (char *regex, char *input, char *output,
        int flags)
{
    rxvm_t *cp;
    rxvm_t compiled;
    char *msg, *start, *end;
    int err;

    cp = NULL;
    msg = "PASS";

    if (regex) {
        if ((err = compile_testexp(&compiled, regex)) < 0) {
            log_trs("FAIL", func);
            fprintf(logfp, "Error: compilation failed (%d): %s\n",
                    err, regex);
            return;
        }

        cp = &compiled;
    }

    if ((err = rxvm_search(cp, input, &start, &end, flags)) != 1) {
        fprintf(logfp, "verify_search_flags: regex=%s, input=%s (flags=%d): "
                "\nExpecting 1, got %d\n", regex, input, flags, err);
        msg = "FAIL";
    } else if (!substring_match(output, start, end)) {
        fprintf(logfp, "verify_search_flags: regex=%s, input=%s (flags=%d): "
                "\nExpecting substring \"%s\", got \"", regex, input,
                flags, output);

        while (start <= end) {
            fputc(*(start++), logfp);
        }
        fprintf(logfp, "\"\n");
        msg = "FAIL";
    }

    if (cp) rxvm_free(cp);

    log_trs(msg, func);
    printf("%s: %s #%d\n", msg, func, ++count);
}

static void verify_search_api (char *regex, char *input, char **start,
        char **end, int flags, int ret)
{
    rxvm_t *cp;
    rxvm_t compiled;
    char *msg;
    int err;

    cp = NULL;
    msg = "PASS";

    if (regex) {
        if ((err = compile_testexp(&compiled, regex)) < 0) {
            log_trs("FAIL", func);
            fprintf(logfp, "Error: compilation failed (%d): %s\n",
                    err, regex);
            return;
        }

        cp = &compiled;
    }

    if ((err = rxvm_search(cp, input, start, end, flags)) != ret) {
        fprintf(logfp, "verify_search_api: regex=%s, input=%s (flags=%d): "
                "Expecting %d, got %d\n", regex, input, flags, ret, err);
        msg = "FAIL";
    }

    if (cp) rxvm_free(cp);

    log_trs(msg, func);
    printf("%s: %s #%d\n", msg, func, ++count);
}

#ifndef NOEXTRAS
static void verify_fsearch_api (char *regex, FILE *fp, int64_t *msize,
        int flags, int ret)
{
    rxvm_t *cp;
    rxvm_t compiled;
    char *msg;
    int err;

    cp = NULL;
    msg = "PASS";

    if (regex) {
        if ((err = compile_testexp(&compiled, regex)) < 0) {
            log_trs("FAIL", func);
            fprintf(logfp, "Error: compilation failed (%d): %s\n",
                    err, regex);
            return;
        }

        cp = &compiled;
    }

    if ((err = rxvm_fsearch(cp, fp, msize, flags)) != ret) {
        fprintf(logfp, "verify_fsearch_api: regex=%s (flags=%d): Expecting %d,"
                " got %d\n", regex, flags, ret, err);
        msg = "FAIL";
    }

    if (cp) rxvm_free(cp);

    log_trs(msg, func);
    printf("%s: %s #%d\n", msg, func, ++count);
}
#endif

static void verify_match_api (char *regex, char *input, int flags, int ret)
{
    rxvm_t *cp;
    rxvm_t compiled;
    char *msg;
    int err;

    cp = NULL;
    msg = "PASS";

    if (regex) {
        if ((err = compile_testexp(&compiled, regex)) < 0) {
            log_trs("FAIL", func);
            fprintf(logfp, "Error: compilation failed (%d): %s\n",
                    err, regex);
            return;
        }

        cp = &compiled;
    }

    if ((err = rxvm_match(cp, input, flags)) != ret) {
        fprintf(logfp, "verify_match_api: input=%s, regex=%s, (flags=%d): "
                "Expecting %d, got %d\n", input, regex, flags, ret, err);
        msg = "FAIL";
    }

    if (cp) rxvm_free(cp);

    log_trs(msg, func);
    printf("%s: %s #%d\n", msg, func, ++count);
}

void test_rxvm_api (void)
{
    FILE *fp;
    int64_t msize;
    char *start;
    char *end;

    count = 0;
    func = __func__;

    if ((fp = fopen("testfile", "a")) == NULL) {
        log_trs("FAIL", func);
        fprintf(logfp, "Can't open test file for reading\n");
        return;
    }

    verify_match_api("a+", "aaa", 0, 1);
    verify_match_api("a+", NULL, 0, RXVM_EPARAM);
    verify_match_api(NULL, "aaa", 0, RXVM_EPARAM);
    verify_match_api(NULL, NULL, 0, RXVM_EPARAM);
    verify_match_api("a", "A", 0, 0);
    verify_match_api("[a-f]+", "aBcDEf", 0, 0);
    verify_match_api("a", "A", RXVM_ICASE, 1);
    verify_match_api("[a-f]+", "aBcDEf", RXVM_ICASE, 1);

    verify_search_api("a+", "aa", NULL, NULL, 0, 1);
    verify_search_api("a+", NULL, NULL, NULL, 0, RXVM_EPARAM);
    verify_search_api(NULL, "aa", NULL, NULL, 0, RXVM_EPARAM);
    verify_search_api(NULL, NULL, NULL, NULL, 0, RXVM_EPARAM);
    verify_search_api(NULL, NULL, &start, &end, 0, RXVM_EPARAM);
    verify_search_api("q", "iiQii", NULL, NULL, 0, 0);
    verify_search_api("[m-z]", "aaPaa", NULL, NULL, 0, 0);
    verify_search_api("q", "iiQii", NULL, NULL, RXVM_ICASE, 1);
    verify_search_api("[m-z]", "aaPaa", NULL, NULL, RXVM_ICASE, 1);
    verify_search_api("^str", "xx\nstr\nxx", NULL, NULL, 0, 0);
    verify_search_api("str$", "xx\nstr\nxx", NULL, NULL, 0, 0);
    verify_search_api("^str", "xx\nstr\nxx", NULL, NULL, RXVM_MULTILINE, 1);
    verify_search_api("str$", "xx\nstr\nxx", NULL, NULL, RXVM_MULTILINE, 1);

    verify_search_flags("ab+", "abbb", "abbb", 0);
    verify_search_flags("ab*", "abbb", "abbb", 0);
    verify_search_flags("ab{,3}", "abbb", "abbb", 0);
    verify_search_flags("<.*>", "<a<b>c>", "<a<b>c>", 0);
    verify_search_flags("ab+", "xxxxabbby", "abbb", 0);
    verify_search_flags("ab*", "xxxxabbbyy", "abbb", 0);
    verify_search_flags("ab{,3}", "xxxxabbby", "abbb", 0);
    verify_search_flags("<.*>", "xxxx<a<b>c>yy", "<a<b>c>", 0);
    verify_search_flags("ab+", "abbb", "ab", RXVM_NONGREEDY);
    verify_search_flags("ab*", "abbb", "a", RXVM_NONGREEDY);
    verify_search_flags("ab{,3}", "abbb", "a", RXVM_NONGREEDY);
    verify_search_flags("ab{1,3}", "abbb", "ab", RXVM_NONGREEDY);
    verify_search_flags("ab{2,3}", "abbb", "abb", RXVM_NONGREEDY);
    verify_search_flags("<.*>", "<a<b>c>", "<a<b>", RXVM_NONGREEDY);
    verify_search_flags("ab+", "xxxxabbby", "ab", RXVM_NONGREEDY);
    verify_search_flags("ab*", "xxxxabbbyy", "a", RXVM_NONGREEDY);
    verify_search_flags("ab{,3}", "xxxxabbbyy", "a", RXVM_NONGREEDY);
    verify_search_flags("ab{1,3}", "xxxxabbby", "ab", RXVM_NONGREEDY);
    verify_search_flags("ab{2,3}", "xxxxabbbyy", "abb", RXVM_NONGREEDY);
    verify_search_flags("<.*>", "xxxx<a<b>c>yyy", "<a<b>", RXVM_NONGREEDY);

#ifndef NOEXTRAS
    verify_fsearch_api("a+", fp, NULL, 0, 0);
    verify_fsearch_api(NULL, fp, NULL, 0, RXVM_EPARAM);
    verify_fsearch_api("a+", NULL, NULL, 0, RXVM_EPARAM);
    verify_fsearch_api(NULL, NULL, NULL, 0, RXVM_EPARAM);
    verify_fsearch_api(NULL, NULL, &msize, 0, RXVM_EPARAM);
#endif

    (void)msize;
    fclose(fp);
}
