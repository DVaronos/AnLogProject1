#include "acutest.h"
#include "../list.h"


void test_create(void){
  TList* T=CreateTList();
  TEST_ASSERT(T!=NULL);

  FreeTList(T);

}

void test_insert(void){
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

void test_find(void){
  TList* T=CreateTList();
  CList* C=CreateCList();
  InsertCList(C,"test",NULL);
  InsertTList(T,C);

  int i=FindTList(T,C);
  TEST_ASSERT(i==0);

  FreeeTList(T);
}




TEST_LIST={
  {"CreateTList",test_create},
  {"InsertTList",test_insert},
  {"FindTList",test_find},
  {NULL,NULL}
};
