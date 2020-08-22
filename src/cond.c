/*!
  * @file cond.h
  * @brief a wrapper around pthread conditions
  * @todo should we auotmatically determine whether or not to use signal/boradcast? recommended way of using pthread is if more than 1 thread is waiting use broadcast instead of signal and it would be neat if we can automatically delegate to the appropriate function
*/

#include <pthread.h>
#include <stdlib.h>
#include "cond.h"

/*!
  * @brief will initialize the given csync_cond_t instance
  * @details do not use this function if you are initializing 
  * @details the struct yourself, however the recommended
  * @details way of using this library is to declare the 
  * @details variable, and then pass a pointer to it into this function
  * @param cond a declared but uninitialized csync_cond_t instance
  * @note you may ignore the return value if wg is not NULL
  * @note otherwise return value must be checked 
  * @return Success (wg != NULL): NULL
  * @return Success (wg == NULL): instance of csync_cond_t
  * @return Failure (wg == NULL): NULL
*/
csync_cond_t *csync_cond_new(csync_cond_t *cond) {
    if (cond == NULL) {
        cond = calloc(1, sizeof(csync_cond_t));
        if (cond == NULL) {
            return NULL;
        }
    }
    pthread_mutex_init(&cond->mutex, NULL);
    pthread_cond_init(&cond->cond, NULL);
    return cond;
}

/*!
 * @brief wrapper around pthread_cond_signal that handles locking and unlocking
*/
void csync_cond_signal(csync_cond_t *cond) {
    pthread_mutex_lock(&cond->mutex);
    pthread_cond_signal(&cond->cond);
    pthread_mutex_unlock(&cond->mutex);
}

/*!
  * @brief wrapper around pthread_cond_broadcast that handles locking/unlocking
*/
void csync_cond_broadcast(csync_cond_t *cond) {
    pthread_mutex_lock(&cond->mutex);
    pthread_cond_broadcast(&cond->cond);
    pthread_mutex_unlock(&cond->mutex);
}

/*!
  * @brief wrapper around pthread_cond_wait that handles locking/unlocking
*/
void csync_cond_wait(csync_cond_t *cond) {
    pthread_mutex_lock(&cond->mutex);
    pthread_cond_wait(&cond->cond, &cond->mutex);
    pthread_mutex_unlock(&cond->mutex);
}