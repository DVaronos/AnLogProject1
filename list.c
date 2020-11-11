#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//--------------------Sinartiseis gia thn domh CList-----------------------------\\


CList* CreateCList(){ //Dimiourgei mia nea CList
	CList *L;
	L=(CList *)malloc(sizeof(CList));
	L->Next=NULL;
  L->name=NULL;
	return L;
}


void InsertCList(CList* L,char* product,NList* Head){//Eisagei ena neo product sthn arxh ths CList
  CList *N;
  N=(CList*)malloc(sizeof(CList));
  N->name=malloc(sizeof(char)*(strlen(product)+1));
  strcpy(N->name,product);
	N->Head=Head;
  N->Next=L->Next;
  L->Next=N;
}


void TransferCList(CList* L,FILE* csvfile){	//Eisagi  ta proionta sto csv file
  CList* N;
  char* current;
  while(L->Next!=NULL){
    L=L->Next;
    N=L;
    current=malloc(sizeof(char)*(strlen(L->name)+1));//apothikefsi tou trexon komvou
    strcpy(current,L->name);
    while(N->Next!=NULL){	//Eisagogi tou trexon komvou mazi me ton kathe ena ap tous epomenous
      N=N->Next;
      fprintf(csvfile,"%s, %s\n",current,N->name);

    }
    free(current);
  }
}


CList* AppendCList(CList* L ,CList* N){
	CList* T=L;
	CList* Head;
	Head=N; //To head einai h kefali ths listas N
	N=N->Next; //O diktis N pleon dixnei ston porto komvo me dedomena ths listas N kai oxi stin kefali ths

	Head->Next=NULL;
	free(Head);	//Apodesmevo thn kefali ths listas N
	while(L->Next!=NULL){//Pigene ston telefteo komvo ths listas L
		L=L->Next;
	}
	L->Next=N;//Valethn lista N sto telos ths listas L
	while(L->Next!=NULL){//O diktis tou komvou pou einai apothikevmeno to kathe neo product na deixnei se afthn thn klika(diladi thn lista L)
		L=L->Next;
		L->Head->clique=T;
	}
	return T;//Epestrepse thn L
}


void FreeCList(CList* L){//Apodesmevi thn CList
  CList* T;
  while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    free(T->name);
    free(T);
  }
	free(L->name);
  free(L);
}


//--------------------Sinartiseis gia thn domh TList-----------------------------\\


TList* CreateTList(){ //Dimiourgei mia nea TList
	TList *L;
	L=(TList *)malloc(sizeof(TList));
	L->Next=NULL;
  L->node=NULL;
	return L;
}

int FindTList(TList* L,CList* node){//Epistrefei 0 an iparxi to node sthn lista allios epistrefei 1
	while(L->Next!=NULL){
		L=L->Next;
		if(L->node==node) return 0;
	}
	return 1;
}

void InsertTList(TList* L,CList* node){

		TList* N;
		N=(TList*)malloc(sizeof(TList));
		N->node=node;
	  N->Next=L->Next;
	  L->Next=N;
}

void FreeTList(TList* L){//Apodesmevi thn CList
  TList* T;
  while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    T->node=NULL;
    free(T);
  }
  free(L);
}

void FreeeTList(TList* L){  //Apodesmevi thn TList kai tis CList pou exei os dedomena
	TList* T;
	while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    FreeCList(T->node);
    free(T);
  }
  free(L);
}


//--------------------Sinartiseis gia thn domh NList-----------------------------\\


NList* CreateNList(){ /*Dimiourgei mia nea SList*/
	NList *L;
	L=(NList *)malloc(sizeof(NList));
	L->Next=NULL;
  L->camera=NULL;
	return L;
}

void InsertNList(NList* L,Camera* camera){//Eisagei ena neo product sthn arxh ths NList
	NList *N;
  N=(NList*)malloc(sizeof(NList));
  N->camera=camera;
	N->clique=NULL;
  N->Next=L->Next;
  L->Next=N;
}


CList* ConectNList(NList* L,char* product,CList* clique){//Eisagei ena neo product sthn arxh ths CList
	while(L->Next!=NULL){	//Vrisko ton komvo ston opio einai apothikevmeno to product
		L=L->Next;
		if(!strcmp(L->camera->id,product)){
				break;
		}
	}

	if(L->clique==NULL){	//Ean den einai to product idi se kapia klika

		if(clique==NULL){	//Ean den exei dwthei kapia klika wste na mpei ekei to product
			L->clique=CreateCList();	//Dimiourgise mia nea klika
			InsertCList(L->clique,product,L);	//Vale to product sthn nea klika
		}else{	//Ean exei dothei mia klika sthn opia prepei na valw afto kai tous komvous ths klikas tou
			L->clique=clique;	//O komvos pou einai apothikevmeno to product dixnei sthn klika pou dothike
			InsertCList(L->clique,product,L);	//vale to product sthn klika pou dothike
		}

	}else{	//Ean to product einai idi se kapia klika

		if(clique!=NULL){ //Ean exei dwthei kapia klika wste na mpei ekei to product
			if(L->clique!=clique){	//ean oi dio klikes den einai idies
					L->clique=AppendCList(clique,L->clique);	//sigxonevw tis dio klikes
			}
		}

	}
		return L->clique; //Epistrefo thn klika sthn opia evala to product
}


void PrintNList(NList* L){//Ektiponei thn NList
  while(L->Next!=NULL){
    L=L->Next;
		Camera_Print_Specs(L->camera);
	}
}

void TransferNList(NList* L,TList* Transfered,FILE* csvfile){	//Metaferi ta dedomena  ths NList
  while(L->Next!=NULL){
    L=L->Next;
		if(L->clique!=NULL){
				if(FindTList(Transfered,L->clique)){//Ean h klika pou dixnei o komvos L den exei metaferthi
						TransferCList(L->clique,csvfile);
						InsertTList(Transfered,L->clique);//Vale thn klika pou dixnei o komvos L se aftes pou exoun metaferthei
					}
  		}
		}
}


void FreeNList(NList* L,TList* Deleted){//Apodesmevi thn NList
	NList* T;
  while(L->Next!=NULL){

    T=L->Next;
    L->Next=T->Next;
    Delete_Camera(T->camera);
		if(T->clique!=NULL){
			if(FindTList(Deleted,T->clique)){//Ean h klika pou dixnei o komvos L den exei ektipothei
				InsertTList(Deleted,T->clique);//Vale thn klika pou dixnei o komvos L stis ektipomenes
			}
		}
		free(T);
  }

  free(L);
}


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

  for(i=0 ; i< size ; i++){//gia kathe bucket tou HashTable dimiourgise mia nea NList
		H[i].Head=CreateNList();
	}
	return H;
}


void FreeHash(Hash* H,TList* Deleted){//Apodesmevi to HashTable
  int i;
	for(i=0 ; i< H->size ; i++){
		FreeNList(H[i].Head,Deleted);
	}

  free(H);

}


void HashInsert(Hash* H,Camera* camera){ /*Eisagei enan neo product sto HashTable*/
  NList *L;
	int i=0,j=0,in;
	char name[20];

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
  return InsertNList(L,camera);
}

CList* HashConect(Hash* H,char* product,CList* Clique){ /*Eisagei enan neo product sto HashTable*/
  NList *L;
	int i=0,j=0,in;
	char name[20];

	while(product[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(product)){
		name[j]=product[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
  L=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
  return ConectNList(L,product,Clique);
}

void HashTransfer(Hash* H,TList* Transfered,FILE* csvfile){//Metaferi ta dedomena  tou HashTable
	NList *L;
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		TransferNList(L,Transfered,csvfile);
	}
}

void HashPrint(Hash* H){//Ektiponei to HashTable
	NList *L;
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		PrintNList(L);
	}
}


//--------------------Sinartiseis gia thn domh SList-----------------------------\\


SList* CreateSList(){ /*Dimiourgei mia nea SList*/
	SList *L;
	L=(SList *)malloc(sizeof(SList));
	L->Next=NULL;
  L->name=NULL;
	return L;
}


void InsertSList(SList* L,Camera* camera){//Eisagei dio stixoia sthn SList
  char s[30];
  char* site;
  int exist=0,i=0;

  while(camera->id[i]!='/'){//Apothikefsi tou onomatos ths istoselidas tou protou proiontos
    s[i]=camera->id[i];
    i++;
  }
  s[i]='\0';
  site=malloc(sizeof(char)*(strlen(s)+1));
  strcpy(site,s);
	SList* f;

  while(L->Next!=NULL && exist==0){//Elegxos an iparxoun idi oi istoselides sthn lista
    L=L->Next;
    if(!strcmp(site,L->name)){//An ipari h proth istoselida tote f1=1
      exist=1;
			f=L;
  	}
	}
	if(exist){
		HashInsert(f->products,camera);
	}else{
		SList* N;
		N=(SList*)malloc(sizeof(SList));
		N->name=malloc(sizeof(char)*(strlen(site)+1));
		strcpy(N->name,site);
		N->Next=NULL;
		N->products=HashCreate(20);
		HashInsert(N->products,camera);
		L->Next=N;//Eimaste ston telefteo komvo ths listas opote vazoume ton komvo F sto telos
	}
	free(site);
}

void ConectSList(SList* L,char* first,char* second){//Eisagei dio stixoia sthn SList
  char s[30];
  char* site1;
  char* site2;
  int f1=0,s1=0,i=0;

  while(first[i]!='/'){//Apothikefsi tou onomatos ths istoselidas tou protou proiontos
    s[i]=first[i];
    i++;
  }
  s[i]='\0';
  site1=malloc(sizeof(char)*(strlen(s)+1));
  strcpy(site1,s);

	i=0;
  while(second[i]!='/'){//Apothikefsi tou onomatos ths istoselidas tou defterou proiontos
    s[i]=second[i];
    i++;
  }
  s[i]='\0';
  site2=malloc(sizeof(char)*(strlen(s)+1));
  strcpy(site2,s);

	SList* fi;
	SList* se;
  while(L->Next!=NULL && (f1==0 || s1==0)) {//Elegxos an iparxoun idi oi istoselides sthn lista
    L=L->Next;
    if(!strcmp(site1,L->name)){//An ipari h proth istoselida tote f1=1
      f1=1;
			fi=L;

    }
    if(!strcmp(site2,L->name)){//An ipari h defterh istoselida tote f2=1
      s1=1;
			se=L;
    }
  }
	CList* clique;

	clique=HashConect(fi->products,first,NULL);
	HashConect(se->products,second,clique);

	free(site1);
	free(site2);

}


void PrintSList(SList* L){//Ektiponei thn SList
  while(L->Next!=NULL){
    L=L->Next;
		HashPrint(L->products);
  }
}

void TransferSList(SList* L,FILE* csvfile){	//Metaferei ta dedomena ths SList
	TList* Transfered=CreateTList();	//Lista opou tha apothikevontai oi klikes pou exoun idi metaferthei so csv file
  while(L->Next!=NULL){
    L=L->Next;
		HashTransfer(L->products,Transfered,csvfile);
  }
	FreeTList(Transfered);
}


void FreeSList(SList* L){//Apodesmevi thn SList
	SList* T;
	TList* Deleted=CreateTList();
  while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    free(T->name);
		FreeHash(T->products,Deleted);
    free(T);
  }
	FreeeTList(Deleted);
  free(L);
}
