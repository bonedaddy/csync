#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "wait_group.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

void test_csync_wait_group_new(void **state) {
  csync_wait_group_t wg;
  csync_wait_group_new(&wg);
  csync_wait_group_add(&wg, 1);
  csync_wait_group_done(&wg);
  pthread_rwlock_destroy(&wg.mutex);
}

void test_csync_wait_group_new_null(void **state) {
  csync_wait_group_t *wg = csync_wait_group_new(NULL);
  assert(wg != NULL);
  csync_wait_group_add(wg, 1);
  csync_wait_group_done(wg);
  pthread_rwlock_destroy(&wg->mutex);
  free(wg);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_csync_wait_group_new),
        cmocka_unit_test(test_csync_wait_group_new_null)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
