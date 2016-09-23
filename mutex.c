#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define INCSIZE 1
#define LOOPSIZE 20000000
int race = 0;
void* subtractor(void * args){
  pthread_mutex_t* my_args = (pthread_mutex_t*)args;
  int lock = pthread_mutex_lock(my_args);

  int i =0;
  for(i;i<LOOPSIZE;i++){
    race -= INCSIZE;
  }

  int unlock = pthread_mutex_unlock(my_args);
  if(unlock){
    printf("ERROR UNLOCKING IN SUBTRACTOR\n");
  }
  return NULL;

}
void* adder(void* args){
  pthread_mutex_t* my_args = (pthread_mutex_t*)args;
  int lock = pthread_mutex_lock(my_args);

  int i =0;
  for(i;i<LOOPSIZE;i++){
    race += INCSIZE;
  }

  int unlock = pthread_mutex_unlock(my_args);
  if(unlock){
    printf("ERROR UNLOCKING IN ADDER\n");
  }  
  return NULL;
}

int main(int argc, char** argv){
  pthread_t id1,id2;
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  int test;

  test = pthread_create(&id1,NULL,adder,&m);
  test = pthread_create(&id2,NULL,subtractor,&m);

  test = pthread_join(id1,NULL);
  test = pthread_join(id2,NULL);

  printf("race is %d\n",race);

  int destroy = pthread_mutex_destroy(&m);
  if(destroy){
    printf("error with destroying mutex\n");
    return -1;
  }
  return 0;
}
