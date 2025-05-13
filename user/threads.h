#ifndef _THREADS_H_
#define _THREADS_H_

//Creates new thread and starts executing at start_routine(arg)
//Returns 0 in success, -1 on failure
int thread_create(void (*start_routine)(void*), void *arg, void *stack);

//Wait for the thread with the given thread_id to finish
//Returns 0 in success, -1 on failure
int thread_join(int thread_id);

//Exits a thread
void thread_exit();

#endif
