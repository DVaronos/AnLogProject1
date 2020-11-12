#include "acutest.h"
#include "../list.h"


void test_create(void){
  CList* C=CreateCList();

  TEST_ASSERT(C!=NULL);

  FreeCList(C);

}

void test_insert(void){
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


TEST_LIST={
  {"CreateCList",test_create},
  {"InsertCList",test_insert},
  {NULL,NULL}
};
