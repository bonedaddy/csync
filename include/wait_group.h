/*!
  * @file csync_wait_group.h
  * @brief enables waiting on multiple different threads
  * @details can be used for things like safe exits, waiting for all pending processes to exit before deallocating memory
*/

#include <pthread.h>

/*!
  * @brief allows waiting on other threads/processes
  * @details is roughly equivalent to Golang's sync.WaitGroup type
*/
typedef struct csync_wait_group {
    unsigned int count; /*! @brief the current number of active threads/processes */
    pthread_rwlock_t mutex; /*! @brief guards access to the count member */
} csync_wait_group_t;


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
csync_wait_group_t *csync_wait_group_new(csync_wait_group_t *wg);

/*!
  * @brief waits until count reaches 0
  * @details used to wait on different threads/processes
*/
void csync_wait_group_wait(csync_wait_group_t *wg);

/*!
  * @brief increments the active count by the given number
  * @warning if count + num overflows this is considered a runtime error
  * @warning and we will exit
*/
void csync_wait_group_add(csync_wait_group_t *wg, unsigned int num);

/*!
  * @brief decreases the active count typically used to signal a thread is done
  * @details if the active count is 0 and this is called, it is considered
  * @details a runtime error and we exit
*/
void csync_wait_group_done(csync_wait_group_t *wg);

/*!
  * @brief used to return the number of active actors
  * @return number of active actors
*/
unsigned int csync_wait_group_count(csync_wait_group_t *wg);