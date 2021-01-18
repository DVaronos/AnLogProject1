#include <pthread.h>

typedef void (*tfunc)(void *arg);

//--------------------Domh Job-----------------------------\\

typedef struct job{
  tfunc func;
  void *arg;
  struct job* Next;
}Job;

Job* JobCreate(tfunc ,void* arg);
void JobDestroy(Job* );

//--------------------Domh JobSheduler-----------------------------\\

typedef struct js{
  Job* First;
  Job* Last;
  pthread_t *threads;
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_cond_t thworking;
  int threadsnum;
  int threadswork;
  int num;
  int working;
}JobSheduler;

Job* JSGetWork(JobSheduler* );
JobSheduler* JSInit(int );


void JSAddWork(JobSheduler* ,Job* );
void JSWaitalltasks(JobSheduler* );
void JSDestroy(JobSheduler* );

void* Worker(void* arg);
