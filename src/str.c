#include <limits.h>
#include <stdio.h>

enum {
    STR_TO_INT_OK = 0,
    STR_TO_INT_ERR = 1,
    STR_TO_INT_OVERFLOW = 2,
};

int str_is_digit(char c) { return c >= '0' && c <= '9'; }

int str_is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

int str_len(char *s) {
    int len = 0;
    while (s[len]) {
        len++;
    }
    return len;
}

char *str_ltrim(char *s) {
    while (str_is_space(*s)) {
        s++;
    }
    return s;
}

char *str_rtrim(char *s) {
    int len = str_len(s);
    while (len > 0 && str_is_space(s[len - 1])) {
        len--;
    }
    s[len] = '\0';
    return s;
}

char *str_trim(char *s) { return str_rtrim(str_ltrim(s)); }

int str_to_int(char *str, int *err) {
    int res = 0;
    char *curr = str;
    int sign = 1;
    int seen_sign = 0;
    int seen_digit = 0;
    while (*curr) {
        if (*curr == '-' || *curr == '+') {
            if (seen_sign || seen_digit) {
                *err = STR_TO_INT_ERR;
                return 0;
            } else if (*curr == '-') {
                sign = -1;
            }
            seen_sign = 1;
        } else if (!str_is_digit(*curr)) {
            *err = STR_TO_INT_ERR;
            return 0;
        } else {
            int digit = *curr - '0';
            seen_digit = 1;
            if (sign > 0) {
                if (res > (INT_MAX - digit) / 10) {
                    *err = STR_TO_INT_OVERFLOW;
                    return 0;
                }
                res = res * 10 + digit;
            } else {
                if (res < (INT_MIN + digit) / 10) {
                    *err = STR_TO_INT_OVERFLOW;
                    return 0;
                }
                res = res * 10 - digit;
            }
        }
        curr++;
    }
    if (!seen_digit) {
        *err = STR_TO_INT_ERR;
        return 0;
    }
    *err = STR_TO_INT_OK;
    return res;
}

#ifdef TEST

void str_to_int_test_case(char *input, int expected_res, int expected_err, int *failed,
                          int *passing) {
    int err = 0;
    int res = str_to_int(input, &err);
    int is_passing = res == expected_res && err == expected_err;
    if (!is_passing) {
        printf("input: %s\n", input);
        printf("result: %d\n", res);
        printf("expected result: %d\n", expected_res);
        printf("error: %d\n", err);
        printf("expected error: %d\n\n", expected_err);
    }
    *failed += !is_passing;
    *passing += is_passing;
}

void test_str_to_int() {
    int failed = 0;
    int passing = 0;
    str_to_int_test_case("+10", 10, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("-10", -10, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("-10  ", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("    +10  ", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("129434", 129434, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("129+434", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("92233720368547758071", 0, STR_TO_INT_OVERFLOW, &failed, &passing);
    str_to_int_test_case("-92233720368547758071", 0, STR_TO_INT_OVERFLOW, &failed, &passing);
    str_to_int_test_case("asd12asd", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("12asd", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-+932", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("--932", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-932", -932, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("   -932\t", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("0", 0, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("    \n\t", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("    104  129", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("привіт світ", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("0004", 4, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("  0004", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-0004", -4, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("  -0004", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-0004\n", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-0004049", -4049, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("  -0004049", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-   1", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-0 004049", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("00-004049", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("0 0", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("-", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("+", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("+ +1", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("++1", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("+-1", 0, STR_TO_INT_ERR, &failed, &passing);
    str_to_int_test_case("2147483647", INT_MAX, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("2147483648", 0, STR_TO_INT_OVERFLOW, &failed, &passing);
    str_to_int_test_case("-2147483648", INT_MIN, STR_TO_INT_OK, &failed, &passing);
    str_to_int_test_case("-2147483649", 0, STR_TO_INT_OVERFLOW, &failed, &passing);
    printf("%d tests are passing\n", passing);
    printf("%d tests are failing\n", failed);
}

#endif
