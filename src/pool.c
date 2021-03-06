/*!
  * @file pool.h
  * @brief allows reuse memory allocated objects to reduce overall memory allocations at the expense of increase memory consumption
  * @details it does this by allowing you to store objects when you are done with them
  * @details it is not suitable for memory constrained environments as it doesnt deallocate
  * @details the memory allocated for the objects when they arent in use
*/

#include <stdlib.h>
#include <pthread.h>
#include "pool.h"

/*!
  * @brief intializes a pool with size available slots for objects
  * @details allows reuse objects created by alloc_fn to reduce memory allocations
  * @details you want to provide a function that will allocate, and initialize an object
  * @details returning the object as a void pointer that you can typecast to the needed type
  * @details whenever you retrieve an object you should make sure to zeroize its fields
  * @details as if the object wasn't created and was reused it will likely have its data set to non-relevant information
  * @param size the initial aray size that will allow us to hold size elements
  * @param size this is not a max limit, and will be doubled whenever we have allocated size amount
  * @param alloc_fn a function that when called will return a new chunk of memory
  * @param free_fn a function that is used to free up the objects returned by alloc_fn
*/
csync_pool_t *csync_pool_new(unsigned int size, csync_pool_alloc alloc_fn, csync_pool_free free_fn) {
    csync_pool_t *pool = calloc(1, sizeof(csync_pool_t));
    if (pool == NULL) {
        return NULL;
    }
    pool->items = calloc(1, size);
    pool->size = size;
    pool->count = 0;
    pool->alloc_fn = alloc_fn;
    pool->free_fn = free_fn;
    pthread_mutex_init(&pool->mutex, NULL);
    return pool;
}

/*!
  * @brief returns an existing object, creating one from scratch if none are currently stored
  * @warning after you are done using the object you need to put it back into the pool
*/
void *csync_pool_get(csync_pool_t *pool) {
    pthread_mutex_lock(&pool->mutex);
    if (pool->count > 0) {
        // since array start at 0, minus 1 from count
        void *item = pool->items[pool->count - 1];
        pool->count -= 1;
        pthread_mutex_unlock(&pool->mutex);
        return item;
    }
    void *item = pool->alloc_fn();
    pthread_mutex_unlock(&pool->mutex);
    return item;
}

/*!
  * @brief returns an item back into the pool so it may be reused
  * @warning do not use the pointer after you have returned it to the pool
  * @param pool an initializedd instance of csync_pool_t
  * @param item the object to put back into the pool
  * @note it will resize the items array if needed
*/
void csync_pool_put(csync_pool_t *pool, void *item) {
    pthread_mutex_lock(&pool->mutex);
    if (pool->count >= pool->size) {
        // increase size by 2
        pool->size *= 2;
        // reallocate the memory
        pool->items = realloc(pool->items, pool->size);
        if (pool->items == NULL) {
            // todo: gracefully handle
            exit(1);
        }

    }
    pool->items[pool->count] = item;
    pool->count += 1;
    pthread_mutex_unlock(&pool->mutex);
}

/*!
  * @brief used to completely destroy the pool and all allocated objects
  * @warning do not use while any objects are borrowed from the pool
*/
void csync_pool_destroy(csync_pool_t *pool) {
  pthread_mutex_lock(&pool->mutex);

  for (unsigned int i = 0; i < pool->count; i++) {
    pool->free_fn(pool->items[i]);
  }

  pthread_mutex_destroy(&pool->mutex);

  free(pool);
}