#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "wait_group.h"
#include "cond.h"
#include "pool.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"


typedef struct object_test {
  int a;
  int b;
} object_test_t;

void free_object_test(void *obj) {
  free((object_test_t *)obj);
}

void *new_object_test() {
  object_test_t *obj = calloc(1, sizeof(object_test_t));
  assert(obj != NULL);
  obj->a = 0;
  obj->b = 0;
  return obj;
}

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


void test_csync_pool(void **state) {
  csync_pool_t *pool = csync_pool_new(10, new_object_test, free_object_test);
  assert(pool != NULL);

  // this should create a new object
  object_test_t *obj = (object_test_t *)csync_pool_get(pool);
  assert(obj != NULL);
  
  // set values
  obj->a = 1;
  obj->b = 2;

  // lets get another object which should create a new one
  // as we havent returned it yet
  object_test_t *obj2 = (object_test_t *)csync_pool_get(pool);
  assert(obj2->a == 0);
  assert(obj2->b == 0);

  // put the first object back
  csync_pool_put(pool, obj);
  assert(pool->count == 1);

  // this should return the already created object
  obj = (object_test_t *)csync_pool_get(pool);
  assert(obj->a == 1);
  assert(obj->b == 2);

  assert(pool->count == 0);

  // put both objects back
  csync_pool_put(pool, obj);
  csync_pool_put(pool, obj2);
  assert(pool->count == 2);

  csync_pool_destroy(pool);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_csync_wait_group_new),
        cmocka_unit_test(test_csync_wait_group_new_null),
        cmocka_unit_test(test_csync_cond_new),
        cmocka_unit_test(test_csync_cond_new_null),
        cmocka_unit_test(test_csync_pool)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
