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


void PrintCList(CList* L,FILE* csvfile){//Ektiponei ta proionta opos zitountai sthn eksodo
  CList* N;
  char* current;
  while(L->Next!=NULL){
    L=L->Next;
    N=L;
    current=malloc(sizeof(char)*(strlen(L->name)+1));//apothikefsi tou trexon komvou
    strcpy(current,L->name);
    while(N->Next!=NULL){//ektiposi tou trexon komvou mazi me ton kathe ena ap tous epomenous
      N=N->Next;
      fprintf(csvfile,"%s, %s\n",current,N->name);
    }
    free(current);
  }
}


CList* AppendCList(CList* L ,CList* N){
	CList* T=L;
	N=N->Next;
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

int FindTlist(TList* L,CList* node){//Epistrefei 0 an iparxi to node sthn lista allios epistrefei 1
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

void FreeeTList(TList* L){
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
  L->name=NULL;
	return L;
}

CList* InsertNList(NList* L,char* product,CList* clique){//Eisagei ena neo product sthn arxh ths CList
	int i=0;
	while(L->Next!=NULL){//Elegxos gia to an iparito product sthn lista
		L=L->Next;
		if(!strcmp(L->name,product)){
				i=1;
				break;
		}
	}

	if(i){//Ean iparxi to product sthn lista
		if(clique!=NULL){//Ean exei dothei mia klika sthn opia prepei na valw afto kai tous komvous ths klikas tou
			if(L->clique!=clique){//ean oi dio klikes den einai idies
					L->clique=AppendCList(clique,L->clique);//sigxonevw tis dio klikes
			}
		}
		return L->clique;//epistrefei thn nea klika
	}else{//Ean den ipari to product sthn lista

		NList *N;//Eisagogh tou product sto telos ths listas
  	N=(NList*)malloc(sizeof(NList));
  	N->name=malloc(sizeof(char)*(strlen(product)+1));
  	strcpy(N->name,product);
  	N->Next=NULL;
  	L->Next=N;
		if(clique==NULL){//Ean den exei dwthei kapia klika wste na mpei ekei to prodact
			N->clique=CreateCList();//Dimiourgise mia nea klika
			InsertCList(N->clique,product,N);//cale to product sthn nea klika
		}else{//ean exei dwthei klika sthn opia prepei na mpei to product
			N->clique=clique;//O komvos pou einai apothikevmeno to product dixnei sthn klika pou dothike
			InsertCList(N->clique,product,N);//vale to product sthn klika pou dothike
		}
		return N->clique;//Epestrepse thn nea klika
	}
}


void PrintNList(NList* L,TList* Printed,FILE* csvfile){//Ektiponei thn NList
  while(L->Next!=NULL){
    L=L->Next;
		if(FindTlist(Printed,L->clique)){//Ean h klika pou dixnei o komvos L den exei ektipothei
			PrintCList(L->clique,csvfile);
			InsertTList(Printed,L->clique);//Vale thn klika pou dixnei o komvos L stis ektipomenes
		}
  }
}


void FreeNList(NList* L,TList* Deleted){//Apodesmevi thn NList
	NList* T;
  while(L->Next!=NULL){

    T=L->Next;
    L->Next=T->Next;
    free(T->name);
		if(FindTlist(Deleted,T->clique)){//Ean h klika pou dixnei o komvos L den exei ektipothei
			InsertTList(Deleted,T->clique);//Vale thn klika pou dixnei o komvos L stis ektipomenes
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


CList* HashInsert(Hash* H,char* product,CList* Clique){ /*Eisagei enan neo product sto HashTable*/
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
  return InsertNList(L,product,Clique);
}

void HashPrint(Hash* H,TList* Printed,FILE* csvfile){//Ektiponei to HashTable
	NList *L;
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		PrintNList(L,Printed,csvfile);
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


void InsertSList(SList* L,char* first,char* second){//Eisagei dio stixoia sthn SList
  char site[30];
  char* p1;
  char* p2;
  int f1=0,f2=0,i=0;

  while(first[i]!='/'){//Apothikefsi tou onomatos ths istoselidas tou protou proiontos
    site[i]=first[i];
    i++;
  }
  site[i]='\0';
  p1=malloc(sizeof(char)*(strlen(site)+1));
  strcpy(p1,site);

	i=0;
  while(second[i]!='/'){//Apothikefsi tou onomatos ths istoselidas tou defterou proiontos
    site[i]=second[i];
    i++;
  }
  site[i]='\0';
  p2=malloc(sizeof(char)*(strlen(site)+1));
  strcpy(p2,site);

	SList* fi;
	SList* se;

  while(L->Next!=NULL && (f1==0 || f2==0)){//Elegxos an iparxoun idi oi istoselides sthn lista
    L=L->Next;
    if(!strcmp(p1,L->name)){//An ipari h proth istoselida tote f1=1
      f1=1;
			fi=L;

    }
    if(!strcmp(p2,L->name)){//An ipari h defterh istoselida tote f2=1
      f2=1;
			se=L;
    }
  }

	CList* clique;
  if(!f1 && !f2){//Ean den iparxi kamia ap tis istoselides
		 SList* F;
		 SList* N;

	   F=(SList*)malloc(sizeof(SList));//Dimiourgia enos neou komvou SList
	   F->name=malloc(sizeof(char)*(strlen(p1)+1));
	   strcpy(F->name,p1);
	   F->Next=NULL;
	   F->products=HashCreate(20);
	   clique=HashInsert(F->products,first,NULL);//Eisagogh tou protou proiontos sto HashTable tou komvou F

	   L->Next=F;//Eimaste ston telefteo komvo ths listas opote vazoume ton komvo F sto telos
	   L=L->Next;//O dixtis L dixnei ston telefteo komvo ths listas(F)

		 if(!strcmp(p1,p2)){//Ean kai ta dio proionta anikoun sto idio site tote den ftiaxnw neo komvo
			 HashInsert(F->products,second,clique);//Eisagogh tou defterou proiontos sto HashTable tou komvou F

		 }else{
			N=(SList*)malloc(sizeof(SList));
			N->name=malloc(sizeof(char)*(strlen(p2)+1));
			strcpy(N->name,p2);
			N->Next=NULL;
			N->products=HashCreate(20);
			HashInsert(N->products,second,clique);
			L->Next=N;//Eimaste ston telefteo komvo ths listas opote vazoume ton komvo F sto telos
 	   	L=L->Next;//O dixtis L dixnei ston telefteo komvo ths listas(F)

		 }
	 }else if(f1 && !f2){//Ean ipari to prwto site kai oxi to deftero
		 clique=HashInsert(fi->products,first,NULL);//Eisagogh tou protou proiontos sto HashTable tou komvou fi

		 SList* N;
		 N=(SList*)malloc(sizeof(SList));
		 N->name=malloc(sizeof(char)*(strlen(p2)+1));
		 strcpy(N->name,p2);
		 N->Next=NULL;
		 N->products=HashCreate(20);
		 HashInsert(N->products,second,clique);//Eisagogh tou defterou proiontos sto HashTable tou komvou N
		 L->Next=N;//Eimaste ston telefteo komvo ths listas opote vazoume ton komvo F sto telos
		 L=L->Next;//O dixtis L dixnei ston telefteo komvo ths listas(F)

	 }else if(!f1 && f2){//Ean iparxi to deftero site kai oxi to proto
		 clique=HashInsert(se->products,second,NULL);//Eisagogh tou defterou proiontos sto HashTable tou komvou se

		 SList* F;
		 F=(SList*)malloc(sizeof(SList));
		 F->name=malloc(sizeof(char)*(strlen(p1)+1));
		 strcpy(F->name,p1);
		 F->Next=NULL;
		 F->products=HashCreate(20);
		 HashInsert(F->products,first,clique);//Eisagogh tou protou proiontos sto HashTable tou komvou F

		 L->Next=F;//Eimaste ston telefteo komvo ths listas opote vazoume ton komvo F sto telos
		 L=L->Next;

	 }else if(f1 && f1){//Ean iparoun kai ta dio site
		 clique=HashInsert(fi->products,first,NULL);
		 HashInsert(se->products,second,clique);
	 }

	free(p1);
	free(p2);

}


void PrintSList(SList* L,FILE* csvfile){//Ektiponei thn SList
	TList* Printed=CreateTList();//Lista opou tha apothikevontai oi idi ektipomenes klikes
  while(L->Next!=NULL){
    L=L->Next;
		HashPrint(L->products,Printed,csvfile);
  }
	FreeTList(Printed);
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
