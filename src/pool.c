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
*/
csync_pool_t *csync_pool_new(unsigned int size, csync_pool_alloc alloc_fn) {
    csync_pool_t *pool = calloc(1, sizeof(csync_pool_t));
    if (pool == NULL) {
        return NULL;
    }
    pool->items = calloc(1, size);
    pool->size = size;
    pool->count = 0;
    pool->alloc_fn = alloc_fn;
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
        void *item = pool->items[pool->count];
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
        pool->items = realloc(pool->items, pool->size * 2);
        if (pool->items == NULL) {
            // todo: gracefully handle
            exit(1);
        }
        // increase size by 2
        pool->size *= 2;
    }
    pool->items[pool->count] = item;
    pool->count += 1;
    pthread_mutex_unlock(&pool->mutex);
}