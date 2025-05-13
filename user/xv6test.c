#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "threads.h"


void my_thread_func(void *arg){
  printf("\tChild: Running!\n");
  sleep(50);
  printf("\tChild: Exiting!\n");
  thread_exit();
}


int main(int argc, char *argv[]){
  printf("user sees my_thread_func at address: %p\n", (void * ) my_thread_func);
  printf("Starting Program. \n");

  void *stack = malloc(4096);
  if(stack == 0){
    printf("error: malloc failed\n");
    exit(1);
  }

  void *fnptr = (void*) my_thread_func;
  char *msg = "Message from Parent to Child";

  int thread_id = thread_create(fnptr, (void *) msg, (void *) stack);

  if(thread_id<0){
    printf("error: falied to create thread\n");
  }

  sleep(1);

  printf("Parent: Waiting for thread %d to finish...\n", thread_id);
  sleep(50);
  //thread_join(thread_id);

  printf("Test Success!\n");
  exit(0);
}
