#include "acutest.h"
#include "../hash.h"


//----------Tests for TList----------\\

void test_Tcreate(void){
  printf("\n   //----------Tests for TList----------\\\\ \n");
  TList* T=CreateTList();
  TEST_ASSERT(T!=NULL);

  FreeTList(T);

}

void test_Tinsert(void){
  TList* T=CreateTList();

  int i;
  char c;
  char* test;
  char t[10]="test";
  for(i=0 ; i<5 ; i++){ //eisagw 5 komvous sthn TList
    c=i+'0';

    test=malloc(sizeof(char)*(strlen(t)+2));  //Dimiourgo ena string "testi" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(test,t);
    strncat(test,&c,1);
    CList* C=CreateCList();
    InsertCList(C,test,NULL);
    InsertTList(T,C);

    free(test);
  }
  i=0;
  TList* P=T;
  while(P->Next!=NULL){ //Metrao tous komvous pou exoun isagxthei sthn TList
    P=P->Next;
    i++;
  }
  TEST_ASSERT(i==5); //Tsekaro an exoun isagxthei osoi komvoi evala
  FreeeTList(T);
}

void test_Tfind(void){
  TList* T=CreateTList();
  CList* C=CreateCList();
  InsertCList(C,"test",NULL);
  InsertTList(T,C);

  int i=FindTList(T,C);
  TEST_ASSERT(i==0);

  FreeeTList(T);
}


//----------Tests for CList----------\\

void test_Ccreate(void){
  printf("\n   //----------Tests for CList----------\\\\ \n");
  CList* C=CreateCList();

  TEST_ASSERT(C!=NULL);

  FreeCList(C);

}

void test_Cinsert(void){
  CList* C=CreateCList();

  int i;
  char c;
  char* test;
  char t[10]="test";
  for(i=0 ; i<5 ; i++){ //eisagw 5 komvous CList
    c=i+'0';

    test=malloc(sizeof(char)*(strlen(t)+2));  //Dimiourgo ena string "testi" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(test,t);
    strncat(test,&c,1);

    InsertCList(C,test,NULL);

    free(test);
  }

  i=0;
  CList* T=C;
  while(T->Next!=NULL){ //Metrao tous komvous pou exoun isagxthei sthn CList
    T=T->Next;
    i++;
  }

  TEST_ASSERT(i==5); //Tsekaro an exoun isagxthei osoi komvoi evala

  FreeCList(C);
}


//----------Tests for Hash----------\\


void test_Hcreate(void){
  printf("\n   //----------Tests for Hash----------\\\\ \n");
  Hash* H=HashCreate(20);

  TEST_ASSERT(H!=NULL);

  FreeHash(H);
}

void test_Hinsert(void){
  Hash* H=HashCreate(20);

  int i;
  char c;
  char* camera;
  char cam[10]="site//7";
  for(i=0 ; i<5 ; i++){ //eisagw 5 domes camera sthn NList
    c=i+'0';
    camera=malloc(sizeof(char)*(strlen(cam)+2));  //Dimiourgo ena string "site//7i" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(camera,cam);
    strncat(camera,&c,1);
    H=HashInsert(H,camera);
    free(camera);
  }

  i=0;
  NList* N;
  for(int j=0 ; j<H->size ; j++){
      N=(NList*)H[j].Head;
      while(N->Next!=NULL){ //Metrao tis domes camera pou exoun isagxthei sthn NList
        N=N->Next;
        i++;
      }
  }

  TEST_ASSERT(i==5); //Tsekaro an exoun isagxthei oses evala
  FreeHash(H);
}


//----------Tests for NList----------\\


void test_Ncreate(void){
  printf("\n   //----------Tests for NList----------\\\\ \n");
  NList* N=CreateNList();  //Dimiourgo mia NList
  TList* D=CreateTList();   //Thn TList tin xriazome gia to Fee ths Nlist

  TEST_ASSERT(N!=NULL); //Elegxo oti ontos h NList dimiourgithike

  FreeNList(N,D);
  FreeTList(D);
}

void test_Ninsert(void){
  NList* N=CreateNList();
  TList* D=CreateTList();

  int i;
  char c;
  char* camera;
  char cam[10]="site//7";
  for(i=0 ; i<5 ; i++){ //eisagw 5 domes camera sthn NList
    c=i+'0';
    camera=malloc(sizeof(char)*(strlen(cam)+2));  //Dimiourgo ena string "site//7i" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(camera,cam);
    strncat(camera,&c,1);
    InsertNList(N,camera);
    free(camera);
  }

  i=0;
  NList* T=N;
  while(T->Next!=NULL){ //Metrao tis domes camera pou exoun isagxthei sthn NList
    T=T->Next;
    i++;
  }

  TEST_ASSERT(i==5); //Tsekaro an exoun isagxthei oses evala

  FreeNList(N,D);
  FreeeTList(D);
}


//----------Tests for LHash----------\\

void test_LHcreate(void){
  printf("\n   //----------Tests for LHash----------\\\\ \n");
  LHash* H=LHashCreate(20);

  TEST_ASSERT(H!=NULL);

  FreeLHash(H);
}

void test_LHinsert(void){
  LHash* H=LHashCreate(20);

  int i;
  char c;
  char* camera;
  char cam[10]="site//7";
  for(i=0 ; i<5 ; i++){ //eisagw 5 domes camera sthn NList
    c=i+'0';
    camera=malloc(sizeof(char)*(strlen(cam)+2));  //Dimiourgo ena string "site//7i" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(camera,cam);
    strncat(camera,&c,1);
    H=LHashInsert(H,camera);
    free(camera);
  }
  i=0;
  for(int j=0 ; j<H->size ; j++){
    if(H[j].word!=NULL) i++;
  }
  TEST_ASSERT(i==5); //Tsekaro an exoun isagxthei oses evala
  FreeLHash(H);
}

void test_LHFind(void){
  LHash* H=LHashCreate(20);

  int i;
  char c;
  char* camera;
  char cam[10]="site//7";
  for(i=0 ; i<5 ; i++){ //eisagw 5 domes camera sthn NList
    c=i+'0';
    camera=malloc(sizeof(char)*(strlen(cam)+2));  //Dimiourgo ena string "site//7i" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(camera,cam);
    strncat(camera,&c,1);
    H=LHashInsert(H,camera);
    free(camera);
  }
  i=LHashFind(H,"site//74");
  TEST_ASSERT(i==1); //Tsekaro an exoun isagxthei oses evala
  FreeLHash(H);
}




TEST_LIST={
  {"CreateTList",test_Tcreate},
  {"InsertTList",test_Tinsert},
  {"FindTList",test_Tfind},
  {"CreateCList",test_Ccreate},
  {"InsertCList",test_Cinsert},
  {"CreateNList",test_Ncreate},
  {"InsertNList",test_Ninsert},
  {"HashCreate",test_Hcreate},
  {"HashInsert",test_Hinsert},
  {"LHashCreate",test_LHcreate},
  {"LHashInsert",test_LHinsert},
  {"LHashFind",test_LHFind},
  {NULL,NULL}
};
