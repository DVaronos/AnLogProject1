#include "acutest.h"
#include "../list.h"


void test_create(void){
  NList* N=CreateNList();
  TList* D=CreateTList();
  TEST_ASSERT(N!=NULL);

  FreeNList(N,D);
  FreeTList(D);
}

void test_insert(void){
  NList* N=CreateNList();
  TList* D=CreateTList();

  InsertNList(N,"test",NULL);
  TEST_ASSERT(N->Next!=NULL);

  FreeNList(N,D);
  FreeeTList(D);
}


TEST_LIST={
  {"CreateNList",test_create},
  {"InsertNList",test_insert},
  {NULL,NULL}
};
