#include "acutest.h"
#include "../list.h"


void test_create(void){

  SList* S=CreateSList();
  TEST_ASSERT(S!=NULL);

  FreeSList(S);

}

void test_insert(void){
  SList* S=CreateSList();

  int i;
  char c;
  char* camera;
  char cam[10]="site";
  for(i=0 ; i<5 ; i++){ //eisagw 5 diaforetika site SList
    c=i+'0';
    camera=malloc(sizeof(char)*(strlen(cam)+5));  //Dimiourgo ena string "sitei//1" opou anti gia i o arithmos tou i se kathe epanalipsi
    strcpy(camera,cam);
    strncat(camera,&c,1);
    strcat(camera,"//1");
    Camera* ca=Camera_Init(camera);   //dimiourgo mia camera me onoma to periexomeno tou string camera
    InsertSList(S,ca);
    free(camera);
  }

  i=0;
  SList* T=S;
  while(T->Next!=NULL){ //Metrao tous komvous ton diaforetikwn site pou exei h SList
    T=T->Next;
    i++;
  }

  TEST_ASSERT(i==5); //Tsekaro an exoun isagxthei osa site evala

  FreeSList(S);
}


TEST_LIST={
  {"CreateSList",test_create},
  {"InsertSList",test_insert},
  {NULL,NULL}
};
