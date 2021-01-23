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
	L->Diffrend=NULL;
	L->camera_count = 0;
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
  L->camera_count++;
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

void DiffCList(CList* L,TList* Transfered,FILE* csvfile){	//Eisagi  ta proionta sto csv file
  TList* N;
	TList* C;
	CList* T;
  char* current;
	int i=0;
	if(L->Diffrend!=NULL){//Ean yparxoun klikes pou diaferoun me thn L
			C=L->Diffrend;	//Krataw thn TList me oles tis klikes pou diaferoun me thn L
			while(L->Next!=NULL){ //Gia kathe proion ths L
				L=L->Next;
				current=malloc(sizeof(char)*(strlen(L->name)+1));//apothikefsi tou trexon komvou
		    strcpy(current,L->name);
				N=C;
				while(N->Next!=NULL){ //Gia kathe lista pou diaferei
					N=N->Next;
					T=N->node;

					if(FindTList(Transfered,T)){ //Ean den exw elegksei thn T pio prin
						while(T->Next!=NULL){ //Gia kathe proion ths T klikas
							T=T->Next;
							fprintf(csvfile,"%s, %s\n",current,T->name);
							i++;
						}
					}

				}
				free(current);
			}
	}
}

void Diff(CList* F,CList* S){
	if(F->Diffrend==NULL){	//Ean h Diffrend ths F einai NULL dimiourgisethn kai vale mesa thn S
		F->Diffrend=CreateTList();
		InsertTList(F->Diffrend,S);
	}else{ //Allios an h S den einai mesa sthn Diffrend ths F vale thn mesa
		if(FindTList(F->Diffrend,S)) InsertTList(F->Diffrend,S);
	}

	if(S->Diffrend==NULL){//Ean h Diffrend ths S einai NULL dimiourgisethn kai vale mesa thn F
		S->Diffrend=CreateTList();
		InsertTList(S->Diffrend,F);
	}else{//Allios an h F den einai mesa sthn Diffrend ths S vale thn mesa
		if(FindTList(S->Diffrend,F)) InsertTList(S->Diffrend,F);
	}
}


int SearchDiffList(TList* DiffList, char* cam_id)
{
	TList* difflist = DiffList->Next;
	while (difflist != NULL)
	{
		CList* clptr = difflist->node->Next;

		while (clptr != NULL)	//oso den einai o teleutaios komvos ths lisatas
		{
			if(strcmp(clptr->name, cam_id) == 0)	//an vre8ei sth lista
			return 1;
			clptr = clptr->Next;
		}
		difflist = difflist->Next;
	}
	return 0;
}


CList* AppendCList(CList* L ,CList* N){
	CList* T=L;
	CList* Head;
	Head=N; //To head einai h kefali ths listas N

	int ncount = N->camera_count;
	L->camera_count += ncount; 	//enhmerwnoume to count

	N=N->Next; //O diktis N pleon dixnei ston porto komvo me dedomena ths listas N kai oxi stin kefali ths

	if(Head->Diffrend!=NULL){ //	Oles oi klikes pou htan diaforetikes ap to head twra tha einai diaforetikes me thn L
		TList* Temp=Head->Diffrend;
		while(Temp->Next!=NULL){	//Gia kathe klika diaforetikh ths Head
			Temp=Temp->Next;
			if(L->Diffrend==NULL) L->Diffrend=CreateTList();	//Ean h L den exei Diffrend List dimiourgei
			if(FindTList(L->Diffrend,Temp->node)){	//Ean h paron CList pou einai diaforetikh me thn Head den einai diaforetikh ths L
				InsertTList(L->Diffrend,Temp->node);	//Vale thn stis diaforetikes ths L
				ReplaceTList(Temp->node->Diffrend,Head,L);	//Sthn Diffrend list ths parousas CList antikatestise thn Head me thn L
			}else{	//Ean h paron CList pou einai diaforetikh me thn Head einai diaforetikh ths L diegrapse apo thn Diffrend ths thn  Head
				RemoveTList(Temp->node->Diffrend,Head);
			}
		}
		FreeTList(Head->Diffrend);
	}

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
	if(L->Diffrend!=NULL){
		FreeTList(L->Diffrend);
		L->Diffrend=NULL;
	}
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

void InsertTList(TList* L,CList* node){//Isagogi mias CList sthn TList
		TList* N;
		N=(TList*)malloc(sizeof(TList));
		N->node=node;
	  N->Next=L->Next;
	  L->Next=N;
}

void RemoveTList(TList* L,CList* C){	//Diagrafi mias CList apo thn TList
	TList* T;
	while(L->Next!=NULL){
		if(L->Next->node==C){
			T=L->Next;
			L->Next=T->Next;
			T->node=NULL;
			free(T);
			break;
		}
		L=L->Next;
	}
}

int CountTList(TList* T){	//Ipologixei to posous komvous exei h TList
	int i=0;
	while(T->Next!=NULL){
		T=T->Next;
		i++;
	}
	return i;
}


void Print_Camera_Count_TList(TList* T)
{
	int i = 1;
	TList* tptr = T->Next;
	while(tptr != NULL)
	{
		printf("list %d has %d cameras\n", i, tptr->node->camera_count);
		tptr = tptr->Next;
		i++;
	}
}


void FreeTList(TList* L){//Apodesmevi thn TList
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

void ReplaceTList(TList* T,CList* P,CList* N){	//Antikathista mia CList sthn TList
	while(T->Next!=NULL){
		T=T->Next;
		if(T->node==P){
			T->node=N;
			break;
		}
	}
}

//--------------------Sinartiseis gia thn domh NList-----------------------------\\


NList* CreateNList(){ /*Dimiourgei mia nea WHash*/
	NList *L;
	L=(NList *)malloc(sizeof(NList));
	L->Next=NULL;
  L->camera=NULL;
	return L;
}

void InsertNList(NList* L,char* camera){//Eisagei ena neo product sthn arxh ths NList
	NList *N;
  N=(NList*)malloc(sizeof(NList));
  N->camera=strdup(camera);
	N->clique=NULL;
	N->Spear=NULL;
	N->vector=NULL;
  N->Next=L->Next;
  L->Next=N;
}


CList* ConectNList(NList* L,char* product,CList* clique){//Eisagei ena neo product sthn arxh ths CList
	while(L->Next!=NULL){	//Vrisko ton komvo ston opio einai apothikevmeno to product
		L=L->Next;
		if(!strcmp(L->camera,product)){
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
	printf("[");
  while(L->Next!=NULL){
    L=L->Next;
		printf(" %s ",L->camera );
	}
	printf("]\n");
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


void DiffNList(NList* L,TList* Transfered,FILE* csvfile){	//Metaferi ta dedomena  ths NList
  while(L->Next!=NULL){
    L=L->Next;
		if(L->clique!=NULL){
				if(FindTList(Transfered,L->clique)){//Ean h klika pou dixnei o komvos L den exei metaferthi
						DiffCList(L->clique,Transfered,csvfile);
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
		if(T->Spear!=NULL) FreeWHash(T->Spear);
		if(T->vector!=NULL)FreeHVector(T->vector);
    if(T->camera!=NULL) free(T->camera);
		if(T->clique!=NULL){
			if(FindTList(Deleted,T->clique)){//Ean h klika pou dixnei o komvos L den exei ektipothei
				InsertTList(Deleted,T->clique);//Vale thn klika pou dixnei o komvos L stis ektipomenes
			}
		}
		free(T);
  }
  free(L);
}


//--------------------Sinartiseis gia thn domh WHash-----------------------------\\


WHash* CreateWHash(int size){ //Dimiourgia enos neou WHash
	WHash *H;
	H=(WHash *)malloc(sizeof(WHash)*size);
	H->size=size;
	H->count=0;
	for(int i=0 ; i< size ; i++){//gia kathe bucket tou HashTable arxikopiise ta dedomena
		H[i].word=NULL;
		H[i].tfidf=0;
	}
	return H;

}

WHash* InsertWHash(WHash* H,char* word){	//Eisagei ena neo value stho WHash
	int j=0,index,exist=0,r=0;

  while(1){ //Evresi tou bucket pou tha paei to neo product
    if(!j){ //Sthn proth epanalipsh ipologizoume mono thn sinartish whashf
      index=whashf(word,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].word==NULL){  //Ean einai adio to bucket tha isagoume se afto thn leksh
      break;
    }else if(!strcmp(H[index].word,word)){  //Ean iparxi idi tha vgoume ap thn epanalipsh
      exist=1;
      break;
    }
    j++;
  }
  if(!exist){ //Ean h leksh den iparxei idi
    H->count++;
    H[index].word=strdup(word);
    H[index].tfidf=1;
    float n=(1.0*H->count)/H->size;
    if(n>=0.8){ //Ean h plirotita ftasei to 80% kanei rehash
      H=Wrehash(H);
    }
  }else{	//Ean iparxi afksise to tf kata ena
    H[index].tfidf=H[index].tfidf+1;
  }
  return H;
}

WHash* Wrehash(WHash* H){
  WHash*  Temp;
  int i,index,j,exist;
  Temp=(WHash* )malloc( (H->size*2) *  sizeof(WHash) ); //Dimiourgo ena neo HasTable me thn diplasia xoritikotita
  Temp->size=H->size * 2;
  Temp->count=H->count;
  for(i=0 ; i< Temp->size ; i++){
    Temp[i].word=NULL;
    Temp[i].tfidf=0.0;
  }
  for(i=0 ; i< H->size ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable

    if(H[i].word!=NULL){  //Ean to arxiko Hash exei dedomena se afthn thn thesh
      j=0;
      while(1){ //Evresi tou bucket pou tha paei to neo product
        if(!j){ //Sthn proth epanalipsh ipologizoume mono thn sinartish hash
          index=whashf(H[i].word,Temp->size);
        }else{
          index=(index+j)%(Temp->size);
        }
        if(Temp[index].word==NULL){  //Ean einai adio to bucket tha isagoume se afto thn camera
          break;
        }
        j++;
      }
			//Antigrafi twn dedomenwn
      Temp[index].word=strdup(H[i].word);
      Temp[index].tfidf=H[i].tfidf;
    }
  }
  FreeWHash(H);
  return Temp;
}


WHash* WHashTF(WHash* H ){
	int count=WHashCount(H);	//Apothikevi to sinolo olwn twn leksewn ths cameras
	for(int i=0 ; i<H->size ; i++){
		if(H[i].word!=NULL){	//Gia kathe bucket pou periexei mia leksh
			H[i].tfidf=(H[i].tfidf)/count;	//Ipologizw to tf gia thn antistih leksh
		}
	}
	return H;
}


int whashf(char *str,int size){ //Hash sinartisi gia strings
  int hash = 5381;
  int c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return abs(hash % size);;
}


int WHashCount(WHash* H){	//Ipologizei to sinoliko athrisma twn leksewn mias cameras
	int count=0;
	for(int i=0 ; i<H->size ; i++){
		if(H[i].word!=NULL){
			count+=H[i].tfidf;
		}
	}
	return count;
}


int WHashFind(WHash* H,char* word){ //Epistrefei 1 an iparxei  leksh sto hash allios 0
	int j=0,index,exist=0;

  while(1){
    if(!j){
      index=whashf(word,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].word==NULL){
      break;
    }else if(!strcmp(H[index].word,word)){
      exist=1;
      break;
    }
    j++;
    if(j==H->size) break;
  }
  return exist;
}



double GiveTfIdf(WHash* H,char* camera){ //Epistrefei thn timh tou tf an iparxei h leksh allios 0
	int j=0,index,exist=0;

  while(1){
    if(!j){
      index=whashf(camera,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].word==NULL){
      break;
    }else if(!strcmp(H[index].word,camera)){
      return H[index].tfidf;
      break;
    }
    j++;
    if(j==H->size) break;
  }
  return 0;
}


void FreeWHash(WHash* H){   //apodesmefsth tou Hash
  for(int i=0 ; i<H->size ; i++){
    if(H[i].word!=NULL) free(H[i].word);
  }
  free(H);
}


void PrintWHash(WHash* H){
	int c=0;
	for(int i=0 ; i<H->size ; i++){
		if(H[i].word!=NULL){
			c++;
			printf("	%dh:%s-%f\n",c,H[i].word,H[i].tfidf);
		}
	}
}


//--------------------Sinartiseis gia thn domh HVector-----------------------------\\

int HVhash(int id,int size){
  id^= (id << 13);
  id^= (id >> 17);
  id^= (id << 5);
  return abs(id % size);

}

HVector* CreateHVector(int size){ //Dimiourgia enos neou WHash
	HVector *H;
	H=(HVector *)malloc(sizeof(HVector)*size);
	H->size=size;
	H->count=0;
	for(int i=0 ; i< size ; i++){//gia kathe bucket tou HashTable arxikopiise ta dedomena
		H[i].key=-1;
		H[i].value=0;
	}
	return H;
}


HVector* InsertHVector(HVector* H,int key,double value){	//Eisagei ena neo value stho WHash
	int j=0,index,exist=0;

  while(1){ //Evresi tou bucket pou tha paei to neo product
    if(!j){ //Sthn proth epanalipsh ipologizoume mono thn sinartish whashf
      index=HVhash(key,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].key==-1){  //Ean einai adio to bucket tha isagoume se afto thn leksh
			H->count++;
			H[index].key=key;
			H[index].value=value;
      break;
    }
    j++;
  }
  float n=(1.0*H->count)/H->size;
  if(n>=0.8){ //Ean h plirotita ftasei to 80% kanei rehash
      H=HVrehash(H);
  }
  return H;
}

HVector* HVrehash(HVector* H){
  HVector*  Temp;
  int i,index,j,exist;
  Temp=(HVector* )malloc( (H->size*1.5) *  sizeof(WHash) ); //Dimiourgo ena neo HasTable me thn diplasia xoritikotita
  Temp->size=H->size * 1.5;
  Temp->count=0;
  for(i=0 ; i< Temp->size ; i++){
    Temp[i].key=-1;
    Temp[i].value=0;
  }
  for(i=0 ; i< H->size ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable

    if(H[i].key!=-1)  //Ean to arxiko Hash exei dedomena se afthn thn thesh
      Temp=InsertHVector(Temp,H[i].key,H[i].value);
  }

  FreeHVector(H);
  return Temp;
}


double HVSumValues(HVector* H){
		double sum=0;
		for(int i=0 ; i<H->size ; i++){
			if(H[i].key!=-1) sum+=H[i].value;
		}
		return sum;
}

double HVGetValue(HVector* H,int key){
	int j=0,index;

  while(1){ //Evresi tou bucket pou tha paei to neo product
    if(!j){ //Sthn proth epanalipsh ipologizoume mono thn sinartish whashf
      index=HVhash(key,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].key==key){  //Ean einai adio to bucket tha isagoume se afto thn leks
			return H[index].value;
      break;
    }
    j++;
  }
	return -1;
}

void PrintHVector(HVector* H){
	int c=0;
	printf("[");
	for(int i=0; i<H->size ;i++){
		if(H[i].key!=-1){
			c++;
			printf("(%d-%f),",H[i].key,H[i].value);
		}
		if(c==H->count) break;
	}
	printf("]\n");
}

void FreeHVector(HVector* H){
	free(H);
}
