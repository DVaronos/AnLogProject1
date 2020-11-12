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

  int i;
  char c;
  char* camera;
  char cam[10]="site//7";
  for(i=0 ; i<5 ; i++){ //eisagw 5 domes camera sthn NList
    c=i+'0';
    camera=malloc(sizeof(char)*(strlen(cam)+2));  //Dimiourgo ena string "site//7i" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(camera,cam);
    strncat(camera,&c,1);
    Camera* ca=Camera_Init(camera);   //dimiourgo mia camera me onoma to periexomeno tou string camera
    HashInsert(H,ca);
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

  FreeHash(H,D);


  FreeeTList(D);
}


TEST_LIST={
  {"HashCreate",test_create},
  {"HashInsert",test_insert},
  {NULL,NULL}
};
