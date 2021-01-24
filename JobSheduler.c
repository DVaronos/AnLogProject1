#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "JobSheduler.h"


//--------------------Sinartiseis gia thn domh Job-----------------------------//

Job* JobCreate(tfunc func,void* arg){//Dimiourgei ena neo JOb
    Job* job;

    if(func==NULL) return NULL;

    job=(Job*)malloc(sizeof(Job));  //Desmefsth tou xwrou
    job->func=func; //Anathesi ths routinas
    job->arg=arg; //Anathesi tou input
    job->Next=NULL;
    return job;
}

void JobDestroy(Job* job){ //Apodesmevi ena job
  if(job!=NULL) free(job);
}


//--------------------Sinartiseis gia thn domh JobSheduler-----------------------------//


Job* JSGetWork(JobSheduler* Sheduler){  //Epistrefi to pio palio Job
  Job* job;

  job=Sheduler->First;
  if(job==NULL) return NULL;

  if(job->Next==NULL){  //Ean einai to telefteo job ths ouras
    Sheduler->First=NULL;
    Sheduler->Last=NULL;
  }else{  //Ean iparxoun kai alla job h oura dixnei ws prwto to epomeno
    Sheduler->First=job->Next;
  }
  return job;
}

JobSheduler* JSInit(int tnum){  //Dimiourgei ena Sheduler me tnum nimata
    JobSheduler* Sheduler;

    Sheduler=(JobSheduler*)malloc(sizeof(JobSheduler));
    Sheduler->threads=(pthread_t*)malloc(tnum*sizeof(pthread_t));
    Sheduler->threadsnum=tnum;
    Sheduler->num=tnum;
    Sheduler->threadswork=0;
    Sheduler->working=1;
    Sheduler->First=NULL;
    Sheduler->Last=NULL;

    pthread_mutex_init(&(Sheduler->lock), NULL);
    pthread_cond_init(&(Sheduler->notify), NULL);
    pthread_cond_init(&(Sheduler->thworking), NULL);

    for(int i=0 ; i<Sheduler->threadsnum ; i++){  //Kathe nima trexei thn sinartisi Worker
      pthread_create(&(Sheduler->threads[i]),NULL,Worker,(void*)Sheduler);
      pthread_detach(Sheduler->threads[i]);
    }
    return Sheduler;
}

void JSAddWork(JobSheduler* Sheduler,Job* job){ //Eisagi ena neo Job sthn oura

  pthread_mutex_lock(&(Sheduler->lock));  //Klidoma tou mutex
   if(Sheduler->First == NULL){ //Ean den iparxei kapio job
       Sheduler->First = job;
       Sheduler->Last = job;
   }else{ //Ean iparxei idi kapio job sthn oura
      Sheduler->Last->Next = job;
      Sheduler->Last=job;
   }

   pthread_cond_broadcast(&(Sheduler->notify)); //Idopeiei ta nimata oti iparxei kapio neo job sthn oura
   pthread_mutex_unlock(&(Sheduler->lock)); //Klidoma tou mutex

}

void JSWaitalltasks(JobSheduler* Sheduler){ //Perimenei na teliosoun ola ta nimata

  pthread_mutex_lock(&(Sheduler->lock));
  while(1){
    if (((Sheduler->working==0) && (Sheduler->threadsnum!=0)) ||((Sheduler->threadswork!=0) && (Sheduler->working!=0)) ){ //Oso iparxoun nimata pou litourgoun
      pthread_cond_wait(&(Sheduler->thworking), &(Sheduler->lock)); //Perimene mexri na teliosi to nima
    }else{
      break;
    }
  }
  pthread_mutex_unlock(&(Sheduler->lock));
}

void JSDestroy(JobSheduler* Sheduler){  //Katastrofh tou Sheduler

    Job* job;
    Job* tmpjob;

    pthread_mutex_lock(&(Sheduler->lock));
    job=Sheduler->First;
    while(job != NULL) {  //Katastrofh twn Job pou perimenoun sthn oura
        tmpjob=job->Next;
        JobDestroy(job);
        job=tmpjob;
    }
    Sheduler->working=0;  //Thetoume thn metavlhth workink 0 ara den dexonte alles doulies ta nimata
    pthread_cond_broadcast(&(Sheduler->notify));
    pthread_mutex_unlock(&(Sheduler->lock));

    JSWaitalltasks(Sheduler); //Perimenoume ta nimata pou litourgoun na teleiwsoun


    pthread_mutex_destroy(&(Sheduler->lock));
    pthread_cond_destroy(&(Sheduler->notify));
    pthread_cond_destroy(&(Sheduler->thworking));

    //Apodesmefsh tou Sheduler
    free(Sheduler->threads);
    free(Sheduler);
}

void* Worker(void* arg){ //I sinarthsh pou trexei kathe nima kai mesw afths ektelite to kathe diathesimo Job
  JobSheduler* Sheduler=(void*)arg;
  Job* job;

  while (1) {
    pthread_mutex_lock(&(Sheduler->lock)); //Klidoma tou mutex
    while (Sheduler->First == NULL && Sheduler->working)  //Oso den iparxei kapio job perimene
      pthread_cond_wait(&(Sheduler->notify), &(Sheduler->lock));

    if(Sheduler->working==0) break; //An termatistike h litourgia tou Sheduler vgenei ap thn while

    job=JSGetWork(Sheduler); //Fere thn teleftea doulia ap thn oura
    Sheduler->threadswork++;
    pthread_mutex_unlock(&(Sheduler->lock));

    if(job != NULL) { //Ektelite to job kai meta katastrefete
      job->func(job->arg);
      JobDestroy(job);
    }

    pthread_mutex_lock(&(Sheduler->lock));
    Sheduler->threadswork--;
    if ((Sheduler->threadswork==0) && (Sheduler->First==NULL) && (Sheduler->working==1)) //Ean den iparxei kapio job den trexei kanena nima enhmerwse
          pthread_cond_signal(&(Sheduler->thworking));
    pthread_mutex_unlock(&(Sheduler->lock));
  }

  Sheduler->threadsnum--; //Miosi tou arithmu twn nhmatwn pou einai diathesima
  pthread_cond_signal(&(Sheduler->thworking));
  pthread_mutex_unlock(&(Sheduler->lock));
  return NULL;
}
