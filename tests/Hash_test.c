#include "acutest.h"
#include "../list.h"


void test_create(void){
  Hash* H=HashCreate(20);
  TList* D=CreateTList();

  TEST_ASSERT(H!=NULL);

  FreeHash(H,D);
  FreeTList(D);

}

void test_insert(void){
  Hash* H=HashCreate(20);
  TList* D=CreateTList();

  CList* C;
  C=HashInsert(H,"test//15",NULL);

  TEST_ASSERT(C!=NULL);

  FreeHash(H,D);


  FreeeTList(D);
}


TEST_LIST={
  {"HashCreate",test_create},
  {"HashInsert",test_insert},
  {NULL,NULL}
};
