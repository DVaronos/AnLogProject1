#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

//--------------------Sinartiseis gia thn domh Hash-----------------------------\\

int hash(int id,int size){

  id^= (id << 13);
  id^= (id >> 17);
  id^= (id << 5);
  return abs(id % size);

}

Hash* HashCreate(int size){ //Dimiourgei ena neo HashTable megetous size
  int i;
	Hash *H;
	H=(Hash* )malloc(size *  sizeof(Hash) );
	H->size=size;
  H->count=0;

  for(i=0 ; i< size ; i++){//gia kathe bucket tou HashTable dimiourgise mia nea NList
		H[i].Head=CreateNList();
	}
	return H;
}


void FreeHash(Hash* H){//Apodesmevi to HashTable
  int i;
  TList* Deleted=CreateTList();
	for(i=0 ; i< H->size ; i++){
		FreeNList(H[i].Head,Deleted);
	}
  free(H);
  FreeeTList(Deleted);
}


Hash* HashInsert(Hash* H,Camera* camera){ /*Eisagei enan neo product sto HashTable*/
  NList *L;
	int i=0,j=0,in;
	char name[20];
  H->count++;
	while(camera->id[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(camera->id)){
		name[j]=camera->id[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
  L=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
  InsertNList(L,camera);
  float n=(1.0*H->count)/H->size;
  if(n>=0.9){
    H=rehash(H);
  }
  return H;
}


Hash* rehash(Hash *H){
  NList* N;
  Hash*  Temp;
  int i;
  Temp=(Hash* )malloc( (H->size*1.5) *  sizeof(Hash) );
  Temp->size=H->size * 1.5;
  Temp->count=0;
  for(i=0 ; i< Temp->size ; i++){
    Temp[i].Head=CreateNList();
  }
  for(i=0 ; i< H->size ; i++){
    N=(NList*)H[i].Head;
    while(N->Next!=NULL){
      N=N->Next;
      Camera* camera;
      camera=N->camera;
      N->camera=NULL;
      HashInsert(Temp,camera);
    }
  }
  FreeHash(H);
  H=Temp;
  return H;
}

void HashConect(Hash* H,char* first,char* second,int match){ /*Eisagei enan neo product sto HashTable*/
  NList* F;
  NList* S;
	int i=0,j=0,in;
	char name[40];
	while(first[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(first)){
		name[j]=first[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
  F=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable

  i=0;
  j=0;
  while(second[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(second)){
		name[j]=second[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	index=hash(in,H->size);
  S=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable


  if(match){
    CList* clique;
    clique=ConectNList(F,first,NULL);
    ConectNList(S,second,clique);
  }else{
    CList* CF;
    CList* CS;
    CF=ConectNList(F,first,NULL);
    CS=ConectNList(S,second,NULL);
    Diff(CF,CS);
  }

}


void HashTransfer(Hash* H,FILE* csvfile){//Metaferi ta dedomena  tou HashTable
	NList *L;
  TList* Transfered=CreateTList();
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		TransferNList(L,Transfered,csvfile);
	}
  FreeTList(Transfered);
}

void HashDiff(Hash* H){//Metaferi ta dedomena  tou HashTable
	NList *L;
  TList* Transfered=CreateTList();
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		DiffNList(L,Transfered);
	}
  FreeTList(Transfered);
}


void HashPrint(Hash* H){//Ektiponei to HashTable
	NList *L;
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		PrintNList(L);
	}
}
