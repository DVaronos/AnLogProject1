#include "acutest.h"
#include "../list.h"


void test_create(void){

  SList* S=CreateSList();
  TEST_ASSERT(S!=NULL);

  FreeSList(S);

}

void test_insert(void){
  SList* S=CreateSList();

  InsertSList(S,"test//10","test//11");
  TEST_ASSERT(S->Next!=NULL);

  FreeSList(S);
}


TEST_LIST={
  {"CreateSList",test_create},
  {"InsertSList",test_insert},
  {NULL,NULL}
};
