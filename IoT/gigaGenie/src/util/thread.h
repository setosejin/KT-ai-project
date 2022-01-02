#ifndef IOTMAKERS_THREAD_H
#define IOTMAKERS_THREAD_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(WIN32) || defined(WIN64)
	#include <Windows.h>
	#define thread_type		HANDLE
	#define thread_id_type	DWORD
	#define thread_return_type	DWORD
	#define thread_fn		LPTHREAD_START_ROUTINE

	#define mutex_type		HANDLE
	#define cond_type		HANDLE
	#define sem_type		HANDLE

#else
	#include <pthread.h>
	#include <semaphore.h>
	#define thread_type		pthread_t
	#define thread_id_type	pthread_t
	#define thread_return_type void*
	typedef thread_return_type (*thread_fn)(void*);
	#define mutex_type pthread_mutex_t*
	typedef struct { pthread_cond_t cond; pthread_mutex_t mutex; } cond_type_struct;
	typedef cond_type_struct *cond_type;
	typedef sem_t *sem_type;

#define WINAPI
#endif


thread_id_type im_thread_getid();
thread_type im_thread_start(thread_fn fn, void* parameter);
mutex_type im_thread_create_mutex();
int im_thread_lock_mutex(mutex_type mutex);
int im_thread_unlock_mutex(mutex_type mutex);
void im_thread_destroy_mutex(mutex_type mutex);


#ifdef __cplusplus
}
#endif

#endif
