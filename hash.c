#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
  if(n>=0.9){ //Ean h plirotita ftasei to 90% kanei rehash
    H=rehash(H);
  }
  return H;
}


Hash* rehash(Hash *H){
  NList* N;
  Hash*  Temp;
  int i;
  Temp=(Hash* )malloc( (H->size*1.5) *  sizeof(Hash) ); //Dimiourgo ena neo HasTable me 150% xoritikotita
  Temp->size=H->size * 1.5;
  Temp->count=0;
  for(i=0 ; i< Temp->size ; i++){
    Temp[i].Head=CreateNList();
  }

  for(i=0 ; i< H->size ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable
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


  if(match){ //Ean match ==1 ta proionta teriazoun
    CList* clique;
    clique=ConectNList(F,first,NULL);
    ConectNList(S,second,clique);
  }else{ //Ean match==0 ta proionta den teriazoun
    CList* CF;
    CList* CS;
    CF=ConectNList(F,first,NULL);
    CS=ConectNList(S,second,NULL);
    Diff(CF,CS);
  }

}


void HashTransfer(Hash* H,FILE* csvfile){//Metaferi ta proionta poy teriazoun se ena csv arxio
	NList *L;
  TList* Transfered=CreateTList();
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		TransferNList(L,Transfered,csvfile);
	}
//  printf("EINAI %d KLIKES\n",CountTList(Transfered));
  FreeTList(Transfered);
}


void HashDiff(Hash* H,FILE* csvfile){//Metaferi ta dedomena pou den teriazoun se ena csv arxio
	NList *L;
  TList* Transfered=CreateTList();
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		DiffNList(L,Transfered,csvfile);
	}
  //printf("EINAI %d KLIKES\n",CountTList(Transfered));
  FreeTList(Transfered);
}


void HashPrint(Hash* H){//Ektiponei to HashTable
	NList *L;
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		PrintNList(L);
	}
}


//--------------------Sinartiseis gia thn domh Hash-----------------------------\\



int hash1(char *str,int size){
  int hash = 5381;
  int c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return abs(hash % size);;
}


LHash* LHashCreate(int size){ //Dimiourgei ena neo LHashTable megetous size
  int i;
	LHash *H;
	H=(LHash* )malloc(size *  sizeof(LHash) );
	H->size=size;
  H->count=0;
  H->height=0;
  H->idf=0;
  for(i=0 ; i< size ; i++){ //gia kathe bucket tou LHashTable arxikopiei ta dedomena tou
		H[i].List=CreateLList();
    H[i].word=NULL;
    H[i].idf=0.0;
	}
	return H;
}

void LHashIncreaseHeight(LHash* H){ //Gia kathe bucket tou LHash table afksanei kata ena to megethos ths LList tou,oi neoi komvoi exoun thn timh 0
  H->height++;
  int c=0;
  for(int i=0 ; i<H->size ; i++){
    if(H[i].word!=NULL){
      InsertLList(H[i].List,0);
      c++;
    }
    if(c==H->count) break;  //Ean episkeftikes ola ta bucket pou exoun periexomeno vges ap thn for
  }
}

LHash* LHashInsert(LHash* H,char* camera){ /*Eisagei enan neo product sto LHashTable*/
  LList *L;
	int j=0,index,exist=0,r=0;

  while(1){ //Evresi tou bucket pou tha paei to neo product
    if(!j){ //Sthn proth epanalipsh ipologizoume mono thn sinartish hash
      index=hash1(camera,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].word==NULL){  //Ean einai adio to bucket tha isagoume se afto thn camera
      break;
    }else if(!strcmp(H[index].word,camera)){  //Ean iparxi idi tha vgoume ap thn epanalipsh
      exist=1;
      break;
    }
    j++;
  }

    L=(LList*)H[index].List;//pernw thn lista pou vriskete sto index bucket tou LHashTable

    if(!exist){ //Ean h camera den iparxei idi
      H->count++;
      H[index].word=strdup(camera);
      for(int k=0 ; k<(H->height-1) ; k++){
        InsertLList(L,0);
      }
      InsertLList(L,1);
    }else{
      IncreaseLastValue(L);
    }
  float n=(1.0*H->count)/H->size;
  if(n>=0.8){ //Ean h plirotita ftasei to 80% kanei rehash
    H=Lrehash(H);
  }
  return H;
}

LHash* Lrehash(LHash* H){
  LHash*  Temp;
  int i,index,j,exist;
  Temp=(LHash* )malloc( (H->size*2) *  sizeof(LHash) ); //Dimiourgo ena neo HasTable me thn diplasia xoritikotita
  Temp->size=H->size * 2;
  Temp->count=H->count;
  Temp->height=H->height;
  for(i=0 ; i< Temp->size ; i++){
    Temp[i].word=NULL;
    Temp[i].idf=0.0;
  }

  for(i=0 ; i< H->size ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable

    if(H[i].word!=NULL){  //Ean to arxiko Hash exei dedomena se afthn thn thesh
      j=0;
      exist=0;
      while(1){ //Vrisko thn thesh ths lekshs gia to neo Hash
        if(!j){
          index=hash1(H[i].word,Temp->size);
        }else{
          index=(index+j*j)%(Temp->size);
        }
        if(Temp[index].word==NULL){
          break;
        }
        j++;
      } //Metafero ta dedomena sto kenourio Hash
      Temp[index].word=H[i].word;
      Temp[index].idf=H[i].idf;
      Temp[index].List=H[i].List;
      H[i].word=NULL;
      H[i].List=NULL;
    }
  }

  for(i=0 ; i< Temp->size ; i++){ //Se oles tis theseis tou neou hash pou den iparxoun dedomena arxikopio thn LList
    if(Temp[i].word==NULL) Temp[i].List=CreateLList();
  }

  FreeLHash(H);
  H=Temp;
  return H;
}


LHash* LHashPrint(LHash* H){
  int i;
  for(i=0 ; i<H->size ; i++){
    if(H[i].word!=NULL) printf(" %d.%s-%.3f |",i,H[i].word,H[i].idf);
  }
  printf("\n");
  for(i=1 ; i < H->height ; i++ ){
    printf("%d",i);
    for(int j=0 ; j< H->size ; j++){
      if(H[j].word!=NULL) PrintLValue(H[j].List,i);
    }
    printf("\n");
  }
}


void LHashTF(LHash* H ){
  int i,j;
  float kati,sum,new,ok;
  for(i=1 ;i<H->height ; i++){ //Gia kathe protash
      sum=0.0;
      for(j=0 ; j< H->size ; j++){  //Gia kathe leksh ths protashs
          if(H[j].word!=NULL){  //Ean sthn thesh afth tou Hash iparxoun dedomena
              if(i==1){ //Ean einai h proth epanalhpsh ipologise to idf ths lekshs
                ok=CountLList(H[j].List);
                H[j].idf=log10((H->height-1)/ok);
              }
              kati=ReturnLValue(H[j].List,i); //H timh ths lekshs se afth thn protash
              if(kati) sum+=kati; //afksise to sinoliko athrisma twn leksewn sthn protash
          }
      }

      for(j=0 ; j< H->size ; j++){  //Gia kathe leksh ths protashs
        if(H[j].word!=NULL){
            kati=ReturnLValue(H[j].List,i);
            if(kati){ //Ean h timh ths lekshs den einai 0 ipologise thn timh ths * tou idf ths lekshs
              new=kati/sum;
              new=new*H[j].idf;
              ReplaceLValue(H[j].List,i,new);
            }
        }
      }

  }
}


int LHashFind(LHash* H,char* camera){ //Epistrefei 1 an iparxei  leksh sto hash allios 0
  LList *L;
	int j=0,index,exist=0;

  while(1){
    if(!j){
      index=hash1(camera,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].word==NULL){
      break;
    }else if(!strcmp(H[index].word,camera)){
      exist=1;
      break;
    }
    j++;
    if(j==H->size) break;
  }
  return exist;
}


LHash* NMostLHash(LHash* H,int n){ //Epistrefei ena Hash pou periexei mono tis n simantikoteres lekseis tou hash
  if(n>H->size) n=H->size;
  H=LHashSort(H,0,H->size);
  LHash*  Temp;
  int i,index,j,exist;
  Temp=(LHash* )malloc( n *  sizeof(LHash) ); //Dimiourgo ena neo HasTable me n size
  Temp->size=n;
  Temp->count=n;
  Temp->height=H->height;
  for(i=0 ; i<n ; i++){
    Temp[i].word=NULL;
    Temp[i].idf=0.0;
  }
  for(i=0 ; i< n ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable
        Temp[i].word=H[i].word;
        Temp[i].idf=H[i].idf;
        Temp[i].List=H[i].List;
        H[i].word=NULL;
        H[i].List=NULL;
  }
  FreeLHash(H);
  H=Temp;
  return H;
}


LHash* LHashSort(LHash* H,int lo,int hi){ //Taksinomisi tou Hash me ton algorithmo Quick sort vasi tou idf
  int j;
  if(lo<hi){
    j=LHashPartition(&H,lo,hi); //Orismos tou pivot
    H=LHashSort(H,lo,j);
    H=LHashSort(H,j+1,hi);
  }
  return H;
}


int LHashPartition(LHash** H,int lo,int hi){ //Evresh tou pivot kai metathesh ton katalilwn thesewn sto Hash

  float pivot=(float)(*H)[lo].idf,ch;
  int i=lo,j=hi;
  while(i<j){
    do{
      i++;
      ch=(float)(*H)[i].idf;
    }while(ch>=pivot && (i<(j-1)));
    do{
       j--;
       ch=(float)(*H)[j].idf;
    }while(ch<pivot);
    if(i<j) (*H)=LHashSwap(*H,i,j);
  }
  (*H)=LHashSwap(*H,lo,j);
  return j;
}


LHash* LHashSwap(LHash* H,int i,int j){ //Metathesh twn thesewn i kai j sto Hash
  LList *T;
  char* tw;
  float ti;

  T=H[i].List;
  tw=H[i].word;
  ti=H[i].idf;

  H[i].word=H[j].word;
  H[i].List=H[j].List;
  H[i].idf=H[j].idf;

  H[j].word=tw;
  H[j].List=T;
  H[j].idf=ti;
  return H;
}

LHash* Camera_to_string(Camera *camera,LHash* H){	 //return camera vocabulary (hash table)
  LHashIncreaseHeight(H);
	Spec_node* sn_ptr = camera->spec_List->first;
	char* key = sn_ptr->key;
	value_list* vl_ptr = sn_ptr->valuelist;
	value_node* vn_ptr = vl_ptr->first;
	char* value = vn_ptr->value;
	char* key_vec, *value_vec;
	int i=0;
	char symbols[] =  " /,\\,[,],(,),*,&,^,%,$,#,@,!,:,;,<,>,`,~,+,_,-,=,|,.," " ";

	while(sn_ptr != NULL){

			key = sn_ptr->key;
			key_vec = strtok(key, symbols);
			i++;

					while(key_vec != NULL){

							key_vec = strtok(NULL, symbols);
							if(key_vec != NULL){
									i++;
									for(int j=0; j<=strlen(key_vec);j++)
      							if(key_vec[j]>=65 && key_vec[j]<=90)	key_vec[j]=key_vec[j]+32;
									H=LHashInsert(H,key_vec);
							}
							vl_ptr = sn_ptr->valuelist;
							vn_ptr = vl_ptr->first;

									while(vn_ptr != NULL){

											value = vn_ptr->value;
											value_vec = strtok(value, symbols);
											if(value_vec!=NULL){
												for(int j=0; j<=strlen(value_vec);j++)
		      								if(value_vec[j]>=65 && value_vec[j]<=90)	value_vec[j]=value_vec[j]+32;
												i++;
												H=LHashInsert(H,value_vec);
											}

												while(value_vec != NULL){

														value_vec = strtok(NULL, symbols);
														if(value_vec != NULL){
															for(int j=0; j<=strlen(value_vec);j++)
																if(value_vec[j]>=65 && value_vec[j]<=90)	value_vec[j]=value_vec[j]+32;
																i++;
												      H=LHashInsert(H,value_vec);
														}
												}
												vn_ptr = vn_ptr->next;
										}
					}
					sn_ptr = sn_ptr->next;
			}
      return H;
}


void FreeLHash(LHash* H){   //apodesmefsth tou Hash
  for(int i=0 ; i<H->size ; i++){
    if(H[i].List!=NULL){
      FreeLList(H[i].List);
    }
    free(H[i].word);
  }
  free(H);
}

//==============================================================\\

CList* FindClique(Camera* camera, Hash* H)
{
  if(camera == NULL)
    return NULL;
  NList* spec_ptr;
  NList *nl = H->Head;
  int i = 0, j = 0, in;
  char name[20];
    H->count++;
  while(camera->id[i]!='/') //Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
      i++;
    i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
  while(i<strlen(camera->id))
  {
    name[j]=camera->id[i];
    j++;
    i++;
  }
  name[j]='\0';
  in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
  int index=hash(in,H->size);
  nl = H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
  spec_ptr = nl->Next;  //spec_ptr deixnei sthn prwth camera ths listas nl
  while(spec_ptr->camera != camera) //anazhthsh mexri na ve8eoun to c1 sto collision list
  {
    spec_ptr = spec_ptr->Next;
    if(spec_ptr == NULL)  //h c1 den uparxei sth lista (oute sto hash)
      return NULL;
  }
  return spec_ptr->clique;
}

int IsAMatch(Camera* c1, Camera* c2, Hash* H)
{
  if((c1 == NULL) || (c2 == NULL))
    return -1;

  Camera* c1_ptr, c2_ptr;
  CList* clique1, *clique2;

  clique1 = FindClique(c1, H);
  clique2 = FindClique(c2, H);

  if(clique1 == clique2)
    return 1;
  else
    return 0;
}
