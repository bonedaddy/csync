/*!
  * @file pool.h
  * @brief allows reuse memory allocated objects to reduce overall memory allocations at the expense of increase memory consumption
  * @details it does this by allowing you to store objects when you are done with them
  * @details it is not suitable for memory constrained environments as it doesnt deallocate
  * @details the memory allocated for the objects when they arent in use
*/

#include <pthread.h>

/*!
  * @brief function signature for the function used to create new objects in the pool
*/
typedef void *(*csync_pool_alloc)(void);

/*!
  * @brief a pool of void pointers along with a function to allocate new ones
  * @details it is essentially a pool of reusable objects that reduce memory allocations
  * @details it does this by providing a means of creating new objects if none are available for reuse
  * @details and allowing you to return the object back to the pool when you no longer need it
  * @details subsequent attempst to creating a new object will be skipped as long as there is an object in the pool
  * @note it is threadsafe as long as all interaction with the pool are done through the functions
  * @todo add a function to free up all pool resources
*/
typedef struct csync_pool {
    void **items;
    unsigned int count;
    unsigned int size;
    pthread_mutex_t mutex;
    csync_pool_alloc alloc_fn;
} csync_pool_t;

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
csync_pool_t *csync_pool_new(unsigned int size, csync_pool_alloc alloc_fn);

/*!
  * @brief returns an existing object, creating one from scratch if none are currently stored
  * @warning after you are done using the object you need to put it back into the pool
*/
void *csync_pool_get(csync_pool_t *pool);

/*!
  * @brief returns an item back into the pool so it may be reused
  * @warning do not use the pointer after you have returned it to the pool
  * @param pool an initializedd instance of csync_pool_t
  * @param item the object to put back into the pool
  * @note it will resize the items array if needed
*/
void csync_pool_put(csync_pool_t *pool, void *item);