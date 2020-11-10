#include "acutest.h"
#include "../list.h"


void test_create(void){
  TList* T=CreateTList();
  TEST_ASSERT(T!=NULL);

  FreeTList(T);

}

void test_insert(void){
  TList* T=CreateTList();

  CList* C=CreateCList();
  InsertCList(C,"test",NULL);

  InsertTList(T,C);
  TEST_ASSERT(T->Next!=NULL);

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
