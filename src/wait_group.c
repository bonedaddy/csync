/*!
  * @brief will initialize the given csync_wait_group_t instance
  * @details do not use this function if you are initializing 
  * @details the struct yourself, however the recommended
  * @details way of using this library is to declare the 
  * @details variable, and then pass a pointer to it into this function
  * @param wg a declared but uninitialized csync_wait_group_t instance
*/

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "wait_group.h"

/*!
  * @brief will initialize the given csync_wait_group_t instance
  * @details do not use this function if you are initializing 
  * @details the struct yourself, however the recommended
  * @details way of using this library is to declare the 
  * @details variable, and then pass a pointer to it into this function
  * @param wg a declared but uninitialized csync_wait_group_t instance
  * @note you may ignore the return value if wg is not NULL
  * @note otherwise return value must be checked 
  * @return Success (wg != NULL): NULL
  * @return Success (wg == NULL): instance of csync_wait_group_t
  * @return Failure (wg == NULL): NULL
*/
csync_wait_group_t *csync_wait_group_new(csync_wait_group_t *wg) {
    if (wg == NULL) {
        wg = calloc(1, sizeof(csync_wait_group_t));
        if (wg == NULL) {
            return NULL;
        }
    }
    wg->count = 0;
    pthread_rwlock_init(&wg->mutex, NULL);
    return wg;
}

/*!
  * @brief waits until count reaches 0
  * @details used to wait on different threads/processes
*/
void csync_wait_group_wait(csync_wait_group_t *wg) {

    if (csync_wait_group_count(wg) == 0) {
        return;
    }
    
    for (;;) {
        // 1000 us == 1 millisecond
        usleep(50000); // 50 millisecond
        if (csync_wait_group_count == 0) {
            return;
        }
    }
}

/*!
  * @brief increments the active count by the given number
  * @warning if count + num overflows this is considered a runtime error
  * @warning and we will exit
*/
void csync_wait_group_add(csync_wait_group_t *wg, unsigned int num) {
    unsigned int count = csync_wait_group_count(wg);
    if (count + num < count) {
        exit(1);
    }
    pthread_rwlock_wrlock(&wg->mutex);
    wg->count += num;
    pthread_rwlock_unlock(&wg->mutex);
}

/*!
  * @brief decreases the active count typically used to signal a thread is done
  * @details if the active count is 0 and this is called, it is considered
  * @details a runtime error and we exit
*/
void csync_wait_group_done(csync_wait_group_t *wg) {
    unsigned int count = csync_wait_group_count(wg);
    if (count == 0) {
        exit(1);
    }
    pthread_rwlock_wrlock(&wg->mutex);
    wg->count -= 1;
    pthread_rwlock_unlock(&wg->mutex);
}

/*!
  * @brief used to return the number of active actors
  * @return number of active actors
*/
unsigned int csync_wait_group_count(csync_wait_group_t *wg) {
    pthread_rwlock_rdlock(&wg->mutex);
    unsigned int count = wg->count;
    pthread_rwlock_unlock(&wg->mutex);
    return count;
}