#ifndef LIBCUT_H
#define LIBCUT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef LIBCUT_CTX_FIELDS
#define LIBCUT_CTX_FIELDS
#endif

struct __libcut_ctx_t {
    char* __name;
    size_t __nlen;
    char** __msg;
    char* __file;
    size_t __lineno;
    void** __trackptr;
    LIBCUT_CTX_FIELDS
};

struct __libcut_err_t {
    char* msg, *file, *name;
    size_t lineno;
};

#define LIBCUT_TEST(name)\
void __libcut_test_##name (struct __libcut_ctx_t*);\
void name(struct __libcut_ctx_t* ctx) {\
    ctx->__file = __libcut_malloc(sizeof(__FILE__));\
    memcpy(ctx->__file, __FILE__, sizeof(__FILE__));\
    ctx->__name = __libcut_malloc(sizeof(#name));\
    memcpy(ctx->__name, #name, sizeof(#name));\
    ctx->__nlen = sizeof(#name)-1;\
    *ctx->__msg = NULL;\
    __libcut_test_##name (ctx); }\
void __libcut_test_##name (struct __libcut_ctx_t* ctx)

#define LIBCUT_TRACK(m) (*ctx->__trackptr++ = (m))
#define LIBCUT_UNTRACK(m) do {\
    void** __libcut_m = ctx->__trackptr;\
    while (--__libcut_m!=ctx->__trackptr)\
        if(*__libcut_m&&*__libcut_m==m){ *__libcut_m=NULL; break; }\
} while (0)

typedef void (*libcut_func_t)(struct __libcut_ctx_t*);

#define LIBCUT_MAIN(...) int main(int argc, char** argv) {\
    libcut_func_t tests[] = {__VA_ARGS__, NULL};\
    libcut_func_t* tptr = tests;\
    struct __libcut_err_t errl[sizeof(tests) / sizeof(tests[0])];\
    memset(errl, 0, sizeof(errl));\
    struct __libcut_err_t* errptr = errl;\
    int passed = 0, failed = 0;\
    char* msg;\
    void* tracked[64];\
    struct __libcut_ctx_t ctx;\
    ctx.__msg = &msg;\
    for (libcut_func_t func = *tptr; func != NULL; func = *++tptr) {\
        memset(tracked, 0, sizeof(tracked));\
        ctx.__trackptr = &tracked[0];\
        func(&ctx);\
        if (msg) {\
            errptr->file = ctx.__file;\
            errptr->msg = msg;\
            errptr->lineno = ctx.__lineno;\
            errptr->name = ctx.__name;\
            ++errptr;\
            ++failed;\
            printf("\033[31mF\033[0m");\
        }\
        else {\
            ++passed;\
            printf("\033[32m.\033[0m");\
            free(ctx.__file);\
            free(ctx.__msg);\
            free(ctx.__name);\
        }\
        if (ctx.__trackptr != &tracked[0])\
            for (void** m=ctx.__trackptr-1; m != tracked-1; --m) if(*m)free(*m);\
    }\
    puts("");\
    for (struct __libcut_err_t* err = errl; err != errptr; ++err) {\
        printf("\033[31merror in test %s at %s:%zu:\033[0m\033[1m %s\033[0m\n", \
               err->name, err->file, err->lineno, err->msg);\
        free(err->file);\
        free(err->msg);\
        free(err->name);\
    }\
    printf("\033[34mTests run: %d\n", passed+failed);\
    printf("\033[32mTests succeeded: %d\n", passed);\
    printf("\033[31mTests failed: %d\033[0m\n", failed);\
    return failed ? 1 : 0; }

static void* __libcut_malloc(size_t bytes) {
    void* ptr = malloc(bytes);
    if (!ptr) {
        fputs("__libcut_malloc: fatal error (out of memory)\n", stderr);
        abort();
    }
    return ptr;
}

static void __libcut_asprintf(char** str, char* format, ...) {
    va_list args, args2;
    int len;

    va_copy(args2, args);
    va_start(args2, format);
    len = vsnprintf(NULL, 0, format, args2);
    if (len < 1) {
        fputs("__libcut_asprintf: fatal error calling vsnprintf\n", stderr);
        abort();
    }
    va_arg(args2, void*);
    va_end(args2);

    *str = __libcut_malloc(len + 1);
    va_start(args, format);
    vsprintf(*str, format, args);
    va_arg(args, void*);
    va_end(args);
}

#define LIBCUT_TEST_BASE(cond, onfail, msg, ...) do { if (!(cond)) {\
    __libcut_asprintf(ctx->__msg, msg, __VA_ARGS__, NULL);\
    ctx->__lineno = __LINE__; onfail; return; } } while (0)

#define LIBCUT_STRF(x) _Generic((x),\
    char*: "%s",\
    const char*: "%s",\
    int: "%d",\
    const int: "%d",\
    long: "%ld",\
    const long: "%ld",\
    unsigned int: "%u",\
    const unsigned int: "%u",\
    unsigned long: "%lu",\
    const unsigned long: "%lu",\
    char: "%c",\
    const char: "%c",\
    float: "%f",\
    const float: "%f",\
    double: "%lg",\
    const double: "%lg")

#define LIBCUT_STRQF(x) _Generic((x),\
    char*: "'%s'",\
    const char*: "'%s'",\
    default: LIBCUT_STRF(x))

#define LIBCUT_TEST_CMP(a, b, expr, op) do {\
    char* __libcut_abuf, *__libcut_bbuf;\
    __libcut_asprintf(&__libcut_abuf, LIBCUT_STRQF(a), (a), NULL);\
    __libcut_asprintf(&__libcut_bbuf, LIBCUT_STRQF(b), (b), NULL);\
    LIBCUT_TEST_BASE((expr), (free(__libcut_abuf), free(__libcut_bbuf)),\
        "%s " op " %s", __libcut_abuf, __libcut_bbuf);\
    free(__libcut_abuf); free(__libcut_bbuf);\
} while (0)

#define LIBCUT_TEST_EQ(a, b) LIBCUT_TEST_CMP(a, b, a == b, "!=")
#define LIBCUT_TEST_NE(a, b) LIBCUT_TEST_CMP(a, b, a != b, "==")
#define LIBCUT_TEST_LT(a, b) LIBCUT_TEST_CMP(a, b, a < b, ">=")
#define LIBCUT_TEST_GT(a, b) LIBCUT_TEST_CMP(a, b, a > b, "<=")
#define LIBCUT_TEST_LE(a, b) LIBCUT_TEST_CMP(a, b, a <= b, ">")
#define LIBCUT_TEST_GE(a, b) LIBCUT_TEST_CMP(a, b, a >= b, "<")

#define LIBCUT_TEST_STREQ(a, b) LIBCUT_TEST_CMP((const char*)a, (const char*)b,\
                                                strcmp(a, b) == 0, "!=")
#define LIBCUT_TEST_STRNE(a, b) LIBCUT_TEST_CMP((const char*)a, (const char*)b,\
                                                strcmp(a, b) != 0, "==")

#endif
