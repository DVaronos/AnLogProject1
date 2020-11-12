#include "acutest.h"
#include "../list.h"


void test_create(void){
  NList* N=CreateNList();  //Dimiourgo mia NList
  TList* D=CreateTList();   //Thn TList tin xriazome gia to Fee ths Nlist

  TEST_ASSERT(N!=NULL); //Elegxo oti ontos h NList dimiourgithike

  FreeNList(N,D);
  FreeTList(D);
}

void test_insert(void){
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
    Camera* ca=Camera_Init(camera);   //dimiourgo mia camera me onoma to periexomeno tou string camera
    InsertNList(N,ca);
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


TEST_LIST={
  {"CreateNList",test_create},
  {"InsertNList",test_insert},
  {NULL,NULL}
};
