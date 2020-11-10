#include "acutest.h"
#include "../list.h"


void test_create(void){
  CList* C=CreateCList();

  TEST_ASSERT(C!=NULL);

  FreeCList(C);

}

void test_insert(void){
  CList* C=CreateCList();

  InsertCList(C,"test",NULL);
  TEST_ASSERT(C->Next!=NULL);

  FreeCList(C);
}


TEST_LIST={
  {"CreateCList",test_create},
  {"InsertCList",test_insert},
  {NULL,NULL}
};
