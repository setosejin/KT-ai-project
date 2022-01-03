#include <stdlib.h>
#include "util/thread.h"
#include "util/log.h"

/**
 */
thread_id_type im_thread_getid()
{
#if defined(WIN32) || defined(WIN64)
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}


thread_type im_thread_start(thread_fn fn, void* parameter)
{
#if defined(WIN32) || defined(WIN64)
	thread_type thread = NULL;
#else
	thread_type thread = 0;
	pthread_attr_t attr;
#endif

#if defined(WIN32) || defined(WIN64)
	thread = CreateThread(NULL, 0, fn, parameter, 0, NULL);
#else
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&thread, &attr, fn, parameter) != 0)
		thread = 0;
	pthread_attr_destroy(&attr);
#endif

	return thread;
}

/**
 * Create a new mutex
 * @return the new mutex
 */
mutex_type im_thread_create_mutex()
{
	mutex_type mutex = NULL;
	int rc = 0;

	#if defined(WIN32) || defined(WIN64)
		mutex = CreateMutex(NULL, 0, NULL);
	#else
		mutex = malloc(sizeof(pthread_mutex_t));
		rc = pthread_mutex_init(mutex, NULL);
	#endif
	return mutex;
}


/**
 * Lock a mutex which has already been created, block until ready
 * @param mutex the mutex
 * @return completion code, 0 is success
 */
int im_thread_lock_mutex(mutex_type mutex)
{
	int rc = -1;

	/* don't add entry/exit trace points as the stack log uses mutexes - recursion beckons */
	#if defined(WIN32) || defined(WIN64)
		/* WaitForSingleObject returns WAIT_OBJECT_0 (0), on success */
		rc = WaitForSingleObject(mutex, INFINITE);
	#else
		rc = pthread_mutex_lock(mutex);
	#endif

	return rc;
}


/**
 * Unlock a mutex which has already been locked
 * @param mutex the mutex
 * @return completion code, 0 is success
 */
int im_thread_unlock_mutex(mutex_type mutex)
{
	int rc = -1;

	/* don't add entry/exit trace points as the stack log uses mutexes - recursion beckons */
	#if defined(WIN32) || defined(WIN64)
		/* if ReleaseMutex fails, the return value is 0 */
		if (ReleaseMutex(mutex) == 0)
			rc = GetLastError();
		else
			rc = 0;
	#else
		rc = pthread_mutex_unlock(mutex);
	#endif

	return rc;
}


/**
 * Destroy a mutex which has already been created
 * @param mutex the mutex
 */
void im_thread_destroy_mutex(mutex_type mutex)
{
	int rc = 0;

	#if defined(WIN32) || defined(WIN64)
		rc = CloseHandle(mutex);
	#else
		rc = pthread_mutex_destroy(mutex);
		free(mutex);
	#endif
}

