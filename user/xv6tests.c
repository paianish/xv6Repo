#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "threads.h"


char *shared_buffer;


void my_thread_func(void *arg){
  int thread_num = *(int *) arg;

  printf("\t[T%d] Running\n", thread_num);
  sleep(10);

  if(thread_num == 0){
    int x = 42;
    printf("\t[T0] Created int x at address: %p\n", &x);
  }

  printf("\t[T%d] Reads %s from shared_buffer\n", thread_num, shared_buffer);
  shared_buffer[0] = 'A' + thread_num;
  printf("\t[T%d] Wrote %c to shared_buffer\n", thread_num, shared_buffer[0]);
  sleep(10);

  printf("\t[T%d] Exiting\n", thread_num);
  thread_exit();
}

void shared_thread_one(void *arg){
  int thread_num = *(int *)arg;

  printf("\t[T%d] Running \n", thread_num);
  sleep(10);
  printf("\t[T%d] Reads %s from shared_buffer\n", thread_num, shared_buffer);
  shared_buffer[0] = 'A' + thread_num;
  printf("\t[T%d] Wrote %c to shared_buffer\n", thread_num, shared_buffer[0]);
  sleep(10);
  printf("\t[T%d] Exiting\n", thread_num);
  thread_exit();
}

void shared_thread_two(void *arg){
  int thread_num = *(int *)arg;

  printf("\t[T%d] Running \n", thread_num);
  sleep(10);
  printf("\t[T%d] Reads %s from shared_buffer\n", thread_num, shared_buffer);
  if(shared_buffer[0] == 'E'){
    printf("\t[PASS] Thread%d read the correct value from shared buffer\n", thread_num);
  }
  shared_buffer[0] = 'A' + thread_num;
  printf("\t[T%d] Wrote %c to shared_buffer\n", thread_num, shared_buffer[0]);
  sleep(10);
  printf("\t[T%d] Exiting\n", thread_num);
  thread_exit();
}



int test_thread_create(){
  printf("=========== TEST 1: thread_create ===========\n");

  void *stack = malloc(4096);
  if(!stack){
    printf("\t[FAIL] malloc failed\n");
    return 0;
  }

  printf("\t[Parent] created thread stack at address: %p\n", stack);

  strcpy(shared_buffer, "Test 1");
  printf("\t[Parent] Wrote %s to shared_buffer\n",shared_buffer);
  sleep(1);
  int arg = 0;
  int tid = thread_create((void*) my_thread_func, (void *) &arg, stack);

  if(tid < 0){
    printf("\t[FAIL] thread_create returned %d\n", tid);
    return 0;
  }
  
  if(thread_join(tid)<0){
    printf("\t[FAIL] thread_join failed\n");
    return 0;
  }
  printf("\t[PASS] thread_create worked and thread exited\n");
  return 1;
}


int test_thread_exit_and_join(){
  printf("=========== TEST 2: thread_exit + thread_join ===========\n");
  
  void *stack1 = malloc(4096);
  void *stack2 = malloc(4096);
  int arg1 = 1;
  int arg2 = 2;

  if(!stack1 || !stack2){
    printf("\t[FAIL] malloc failed\n");
    return 0;
  }

  strcpy(shared_buffer, "Test 2");
  printf("\t[Parent] Wrote %s to shared_buffer\n",shared_buffer);
  sleep(1);

  int tid1 = thread_create((void *) my_thread_func, (void *) &arg1, stack1);
  sleep(10);
  int tid2 = thread_create((void *) my_thread_func, (void *) &arg2, stack2);
  
  if(tid1< 0 || tid2<0){
    printf("\t[FAIL] thread_create failed\n");
    return 0;
  }

  thread_join(tid1);
  thread_join(tid2);

  printf("\t[PASS] both threads exited and joined successfully\n");
  return 1;
}

int test_shared_memory(){

  printf("=========== TEST 3: shared memory between threads ===========\n");

  void *stack = malloc(4096);
  if(!stack){
    printf("\t[FAIL] malloc failed\n");
    return 0;
  }

  int arg = 3;

  strcpy(shared_buffer, "Test 3");
  printf("\t[Parent] Wrote %s to shared_buffer\n",shared_buffer);
  sleep(1);

  int tid = thread_create((void *) my_thread_func, (void*) &arg, stack);

  thread_join(tid);
  
  printf("\t[PASS] shared_buffer successfully modified by thread: %d\n", arg);
  return 1;
}

int test_shared_mem_advanced(){
  printf("=========== TEST 4: advanced shared memory between threads ===========\n");
  void *stack = malloc(4096);
  if(!stack){
    printf("\t[FAIL] malloc failed\n");
    return 0;
  }

  int arg = 4;
  strcpy(shared_buffer, "Test 4");
  printf("\t[Parent] Wrote %s to shared_buffer\n",shared_buffer);
  sleep(1);
  int tid = thread_create((void *) shared_thread_one, (void*)&arg, stack);
  thread_join(tid);

  arg = 5;
  tid = thread_create((void *) shared_thread_two, (void*)&arg, stack);
  thread_join(tid);

  
  printf("\t[PASS] shared_buffer successfully modified by thread: %d\n", arg);
  return 1;
}


int main(int argc, char *argv[]){
  printf("================ THREAD TESTING ================\n");
  shared_buffer = malloc(4096);

  int score = 0;
  score += test_thread_create();
  score += test_thread_exit_and_join();
  score += test_shared_memory();
  score += test_shared_mem_advanced();

  if(score == 4){
    printf("================ ALL TESTS PASSED ================\n");
  } else {
    printf("================ Test Failure ==================\n");
  }
  exit(0);
}
