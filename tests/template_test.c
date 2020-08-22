#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

/*
  write your tests here
*/

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(/* your test here */)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
