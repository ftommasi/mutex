#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define INCSIZE 1
#define LOOPSIZE 200000000
int race = 0;
void* subtractor(void * args){
  pthread_mutex_t* my_args = (pthread_mutex_t*)args;
  int lock = pthread_mutex_lock(my_args);
  while(!lock){
    //printf("ERROR LOCKING IN SUBTRACTOR\n");
    //wait...
    lock = pthread_mutex_lock(my_args);
  }
  int i =0;
  for(i;i<LOOPSIZE;i++){
    race -= INCSIZE;
  }
  int unlock = pthread_mutex_unlock(my_args);
  if(!unlock){
    printf("ERROR UNLOCKING IN SUBTRACTOR\n");
  }
  return NULL;

}
void* adder(void* args){
  pthread_mutex_t* my_args = (pthread_mutex_t*)args;
  int lock = pthread_mutex_lock(my_args);
  while(!lock){
    //printf("ERROR LOCKING IN ADDER\n");
    //wait//
    lock = pthread_mutex_lock(my_args);
  }

  int i =0;
  for(i;i<LOOPSIZE;i++){
    race += INCSIZE;
  }

  int unlock = pthread_mutex_unlock(my_args);
  if(!unlock){
    printf("ERROR UNLOCKING IN ADDER\n");
  }  
  return NULL;
}

int main(int argc, char** argv){
  pthread_t id1,id2,id3,id4;
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER ;
  int test;
  printf("race is %d\n",race);
  test = pthread_create(&id1,NULL,adder,&m);//adder();
  printf("race is %d\n",race);
  //adder();
  test = pthread_create(&id2,NULL,subtractor,&m);//adder();
  printf("race is %d\n",race);
  //subtractor();
  test = pthread_create(&id3,NULL,adder,&m);//adder();
  printf("race is %d\n",race);
  //subtractor();
  test = pthread_create(&id4,NULL,subtractor,&m);//adder();
  printf("race is %d\n",race);

  test = pthread_join(id1,NULL);
  test = pthread_join(id2,NULL);
  test = pthread_join(id3,NULL);
  test = pthread_join(id4,NULL);

  int destroy = pthread_mutex_destroy(&m);
  if(!destroy){
    printf("error with destroying mutex\n");
    return -1;
  }
  return 0;
}
