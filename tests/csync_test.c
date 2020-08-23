#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "wait_group.h"
#include "cond.h"

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

void test_csync_cond_new(void **state) {
  csync_cond_t cond;
  csync_cond_new(&cond);
  csync_cond_broadcast(&cond);
  csync_cond_signal(&cond);
  // todo: we need to create a thread to do the signalling so we can be blocked on wait
}

void test_csync_cond_new_null(void **state) {
  csync_cond_t *cond = csync_cond_new(NULL);
  csync_cond_broadcast(cond);
  csync_cond_signal(cond);
  // todo: we need to create a thread to do the signalling so we can be blocked on wait
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_csync_wait_group_new),
        cmocka_unit_test(test_csync_wait_group_new_null),
        cmocka_unit_test(test_csync_cond_new),
        cmocka_unit_test(test_csync_cond_new_null)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
