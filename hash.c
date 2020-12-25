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


Hash* HashInsert(Hash* H,char* camera){ /*Eisagei enan neo product sto HashTable*/
  NList *L;
	int i=0,j=0,in;
	char name[20];
  H->count++;
	while(camera[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(camera)){
		name[j]=camera[i];
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
      Temp=HashInsert(Temp,N->camera);
      Temp=HashReplaceSpear(Temp,N->camera,N->Spear);
      N->Spear=NULL;
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
  FreeTList(Transfered);
}


void HashDiff(Hash* H,FILE* csvfile){//Metaferi ta dedomena pou den teriazoun se ena csv arxio
	NList *L;
  TList* Transfered=CreateTList();
	for(int i=0 ; i<H->size ; i++){
		L=(NList*)H[i].Head;
		DiffNList(L,Transfered,csvfile);
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


Hash* HashVectorts(Hash* H,LHash* Lek){ //Dimiourgei ena vector gia kathe camera
	NList *L;
  double tf,value;

	for(int i=0 ; i<H->size ; i++){  //Gia kathe bucket tou Hash
		L=(NList*)H[i].Head;
		while(L->Next!=NULL){ //Gia kathe komvo ths NList(ara gia kathe kameras)
      L=L->Next;
      L->vector=CreateHVector(200);
      for(int j=0 ; j<Lek->size ; j++){ //Gia kathe leksh tou Lek
        tf=GiveTfIdf(L->Spear,Lek[j].word);  //Pernw to tf ths lekshs an iparxei sthn camera alios to tf ginete 0
        if(tf){ //ean iparxi h leksh sthn camera
          value=tf*Lek[j].tfcount;  //h timh ginete tf*idf
          L->vector=InsertHVector(L->vector,j,value); //isagogi sto vector
        }
      }
      L->vec_size = Lek->size;
      FreeWHash(L->Spear);  //Apodemsevw to WHash ths cameras afou pleon den to xriazome
      L->Spear=NULL;
    }

	}
  return H;
}

WHash* HashFind(Hash* H,char* word){
  NList *L;
	int i=0,j=0,in;
	char name[20];
  while(word[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(word)){
		name[j]=word[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
  L=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
  while(L->Next!=NULL){
    L=L->Next;
    if(!strcmp(L->camera,word)) return L->Spear;
  }
  return NULL;
}

Hash* HashReplaceSpear(Hash* H,char* word,WHash* Spear){  //Antikathistw to WHash ths cameras me onoma word
  NList *L;
	int i=0,j=0,in;
	char name[20];
  while(word[i]!='/'){//Vrisko to simio pou teliwnei to onoma ths istoselidas tou proiontos
    i++;
  }
	i+=2;//pigeno to i sthn thesh pou arxizei o arithmos tou proiontos
	while(i<strlen(word)){
		name[j]=word[i];
		j++;
		i++;
	}
	name[j]='\0';
	in=atoi(name);//apothievo ton arithmo tou proiontos ston int in
	int index=hash(in,H->size);
  L=(NList*)H[index].Head;//pernw thn lista pou vriskete sto index bucket tou HashTable
  while(L->Next!=NULL){ //Antikathistw to WHash ths cameras me onoma word
    L=L->Next;
    if(!strcmp(L->camera,word)){
      if(L->Spear!=NULL) FreeWHash(L->Spear);
      L->Spear=Spear;
      return H;
    }
  }
}

//--------------------Sinartiseis gia thn domh LHash-----------------------------\\



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
  for(i=0 ; i< size ; i++){ //gia kathe bucket tou LHashTable arxikopiei ta dedomena tou
    H[i].word=NULL;
    H[i].idf=0.0;
    H[i].tfcount=0;
    H[i].wordperj=0;
	}
	return H;
}


LHash* LHashInsert(LHash* H,char* camera){ /*Eisagei enan neo product sto LHashTable*/
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
  if(!exist){ //Ean h camera den iparxei idi
    H->count++;
    H[index].word=strdup(camera);
    H[index].wordperj=1;
    float n=(1.0*H->count)/H->size;
    if(n>=0.8){ //Ean h plirotita ftasei to 80% kanei rehash
      H=Lrehash(H);
    }
  }else{
    H[index].wordperj=H[index].wordperj+1;
  }
  return H;
}

LHash* Lrehash(LHash* H){
  LHash*  Temp;
  int i,index,j,exist;
  Temp=(LHash* )malloc( (H->size*2) *  sizeof(LHash) ); //Dimiourgo ena neo HasTable me thn diplasia xoritikotita
  Temp->size=H->size * 2;
  Temp->count=H->count;
  for(i=0 ; i< Temp->size ; i++){
    Temp[i].word=NULL;
    Temp[i].idf=0.0;
    Temp[i].tfcount=0;
    Temp[i].wordperj=0;
  }
  for(i=0 ; i< H->size ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable

    if(H[i].word!=NULL){  //Ean to arxiko Hash exei dedomena se afthn thn thesh
      j=0;
      while(1){ //Evresi tou bucket pou tha paei to neo product
        if(!j){ //Sthn proth epanalipsh ipologizoume mono thn sinartish hash
          index=hash1(H[i].word,Temp->size);
        }else{
          index=(index+j*j)%(Temp->size);
        }
        if(Temp[index].word==NULL){  //Ean einai adio to bucket tha isagoume se afto thn camera
          break;
        }
        j++;
      }
      Temp[index].word=strdup(H[i].word);
      Temp[index].wordperj=H[i].wordperj;
      Temp[index].idf=H[i].idf;
      Temp[index].tfcount=H[i].tfcount;
    }
  }
  FreeLHash(H);
  H=Temp;
  return H;
}

LHash* LHashPrint(LHash* H){  //Ektiponei to Leksilogio
  int i,c=0;
  for(i=0 ; i<H->size ; i++){
    if(H[i].word!=NULL){
      c++;
      printf("%d.%s-%f-%f\n",c,H[i].word,H[i].idf,H[i].tfcount);
    }
  }
}

void LHashTfIdf(LHash* Lek,double count){ //Dimiourgei ena vector gia kathe camera
	NList *L;
  double t;
  int wc=0;
	for(int i=0 ; i<Lek->size ; i++){  //Gia kathe bucket tou leksilogiou
    if(Lek[i].word!=NULL){  //An iparxei leksh
        wc++;
        Lek[i].idf=log10(count/(Lek[i].wordperj));//Ipologizo to idf
        t=Lek[i].idf;
        Lek[i].idf=(Lek[i].idf*Lek[i].tfcount)/count;//Ipologizo to meso tf-idf
        Lek[i].tfcount=t; //krataw to idf sto tfcoun
      }
      if(wc==Lek->count) break;
    }
}


int LHashFind(LHash* H,char* camera){ //Epistrefei 1 an iparxei  leksh sto hash allios 0
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
  for(i=0 ; i<n ; i++){
    Temp[i].word=NULL;
    Temp[i].idf=0.0;
    Temp[i].tfcount=0.0;
  }
  for(i=0 ; i< n ; i++){  //Pernaw ta dedomena tou paliou sto neo HasTable
        Temp[i].word=H[i].word;
        Temp[i].idf=H[i].idf;
        Temp[i].tfcount=H[i].tfcount;
        H[i].word=NULL;
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
  char* tw;
  double ti,tic;

  tw=H[i].word;
  ti=H[i].idf;
  tic=H[i].tfcount;

  H[i].word=H[j].word;
  H[i].idf=H[j].idf;
  H[i].tfcount=H[j].tfcount;

  H[j].word=tw;
  H[j].idf=ti;
  H[j].tfcount=tic;
  return H;
}


void FreeLHash(LHash* H){   //apodesmefsth tou Hash
  for(int i=0 ; i<H->size ; i++){
    free(H[i].word);
  }
  free(H);
}


LHash* LHashIncreaseTf(LHash* H,char* word,double tf){
  int j=0,index,exist=0;

  while(1){
    if(!j){
      index=hash1(word,H->size);
    }else{
      index=(index+j*j)%(H->size);
    }
    if(H[index].word==NULL){
      break;
    }else if(!strcmp(H[index].word,word)){
      H[index].tfcount=H[index].tfcount+tf;
      break;
    }
    j++;
    if(j==H->size) break;
  }
  return H;
}

LHash* Readjson(char* filename,LHash* H,LHash* Common,WHash** L){ //Diavazei to Json kai apothikei tis leksis sto leksilogio kai se ena WHash
  FILE* fp;
  if( (fp=fopen(filename,"r"))==NULL){
    perror("Fopen");
  }

  char symbols[] =  " /,\\,\n,[,],(,),*,&,^,%,$,#,@,!,:,;,<,>,`,~,+,_,-,=,{,}|,\",.," " ";  //Ta simvolia ta opia tha xwrizoun tis lekseis
  (*L)=CreateWHash(200);
  char c;
  char line[250];
  char* word;
  int count=0;
  while(fgets(line, sizeof(line),fp)){  //Diavazei to json grami gami

			word=strtok(line,symbols);
	    while(word!=NULL){ //Gia kathe leksi tis gramhs
				count++;
				for(int j=0; j<=strlen(word);j++){  //Metatrepei ta kegalea se peza
					if(word[j]>=65 && word[j]<=90)	word[j]=word[j]+32;
  			}
        if(!LHashFind(Common,word)){  //Ean h leksh den einai mia apo tis common
          if(!WHashFind(*L,word))  H=LHashInsert(H,word); //Ean den exw isagi thn leksh allh fora sto stigekrimeno json
          *L=InsertWHash(*L,word);
        }
				word=strtok(NULL,symbols);
			}

	}
  fclose(fp);
  (*L)=WHashTF(*L); //Ipoligismos twn tf gia kathe leksh tou TF
  for(int i=0 ; i<(*L)->size ; i++){
    if((*L)[i].word!=NULL){
      H=LHashIncreaseTf(H,(*L)[i].word,(*L)[i].tfidf);
    }
  }
  return H;
}
