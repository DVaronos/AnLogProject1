#include "acutest.h"
#include "../hash.h"


//----------Tests for TList----------\\

void test_Tcreate(void){
  printf("\n   //----------Tests for TList----------\\\\ \n");
  TList* T=CreateTList();
  TEST_ASSERT(T!=NULL);

  FreeTList(T);

}

void test_Tinsert(void){
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

void test_Tfind(void){
  TList* T=CreateTList();
  CList* C=CreateCList();
  InsertCList(C,"test",NULL);
  InsertTList(T,C);

  int i=FindTList(T,C);
  TEST_ASSERT(i==0);

  FreeeTList(T);
}


//----------Tests for CList----------\\

void test_Ccreate(void){
  printf("\n   //----------Tests for CList----------\\\\ \n");
  CList* C=CreateCList();

  TEST_ASSERT(C!=NULL);

  FreeCList(C);

}

void test_Cinsert(void){
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


//----------Tests for Hash----------\\


void test_Hcreate(void){
  printf("\n   //----------Tests for Hash----------\\\\ \n");
  Hash* H=HashCreate(20);

  TEST_ASSERT(H!=NULL);

  FreeHash(H);
}

void test_Hinsert(void){
  Hash* H=HashCreate(20);

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
    H=HashInsert(H,ca);
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
  FreeHash(H);
}


//----------Tests for NList----------\\


void test_Ncreate(void){
  printf("\n   //----------Tests for NList----------\\\\ \n");
  NList* N=CreateNList();  //Dimiourgo mia NList
  TList* D=CreateTList();   //Thn TList tin xriazome gia to Fee ths Nlist

  TEST_ASSERT(N!=NULL); //Elegxo oti ontos h NList dimiourgithike

  FreeNList(N,D);
  FreeTList(D);
}

void test_Ninsert(void){
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


//----------Tests for Camera----------\\


void test_Value_Node_Init(void)
{
  printf("\n   //----------Tests for Camera----------\\\\ \n");
	char value_str[] = "test_value";
	value_node* vn =  Value_Node_Init(value_str);

	TEST_ASSERT(vn != NULL);
	TEST_ASSERT(strcmp(vn->value,value_str) == 0);

	Delete_Value_Node(vn);
}

void test_Value_List_Init(void)
{
	value_list* vl = Value_List_Init();

	TEST_ASSERT(vl != NULL);

	Delete_Value_List(vl);
}

void test_Add_to_value_list(void)
{
	value_list* vl = Value_List_Init();
	char value_str[] = "test_value";
	value_node* vn =  Value_Node_Init(value_str);

	Add_to_value_list(vl, vn);

	// testing first add
	TEST_ASSERT(vl->first == vn);
	TEST_ASSERT(vl->last == vn);

	value_node* vn_ptr = vl->first;

	// testing multiple adds
	for(int i=0; i<5; i++)
	{
		value_node* vn =  Value_Node_Init(value_str);
		Add_to_value_list(vl, vn);
		// this last node is not added at the start of the list
		TEST_ASSERT(vl->first != vn);
		// its added at the end
		TEST_ASSERT(vl->last == vn);
		// its connected to the previous last node of the list
		TEST_ASSERT(vn_ptr->next == vn);
		vn_ptr = vn_ptr->next;
	}

	Delete_Value_List(vl);
}

void test_Spec_node_Init(void)
{
	char* key = "test_key";
	value_list* vl = Value_List_Init();
	Spec_node* sn = Spec_node_Init(key, vl);

	TEST_ASSERT(sn != NULL);
	TEST_ASSERT(strcmp(sn->key, key) == 0);
	TEST_ASSERT(sn->valuelist == vl);
	TEST_ASSERT(sn->next == NULL);

	Delete_Spec_node(sn);
}

void test_Spec_List_Init(void)
{
	Spec_List* sl = Spec_List_Init();
	TEST_ASSERT(sl != NULL);

	Delete_Spec_List(sl);
}

void test_Specs_Add_Node(void)
{
	Spec_List* sl = Spec_List_Init();

	char* key = "test_key";
	value_list* vl = Value_List_Init();
	Spec_node* sn = Spec_node_Init(key, vl);

	Specs_Add_Node(sl, sn);

	// testing first add
	TEST_ASSERT(sl->first == sn);
	TEST_ASSERT(sl->last == sn);

	Spec_node* sn_ptr = sl->first;

	// testing multiple adds
	for(int i=0; i<5; i++)
	{
		value_list* vl = Value_List_Init();
		Spec_node* sn = Spec_node_Init(key, vl);

		Specs_Add_Node(sl, sn);

		// this last node is not added at the start of the list
		TEST_ASSERT(sl->first != sn);
		// its added at the end
		TEST_ASSERT(sl->last == sn);
		// its connected to the previous last node of the list
		TEST_ASSERT(sn_ptr->next == sn);
		sn_ptr = sn_ptr->next;
	}

	Delete_Spec_List(sl);
}

void test_Camera_Init(void)
{
	char* name = "test name";
	Camera* c = Camera_Init(name);

	TEST_ASSERT(c != NULL);
	TEST_ASSERT(strcmp(c->id, name) == 0);
	TEST_ASSERT(c->spec_List != NULL);

	Delete_Camera(c);
}


TEST_LIST={
  {"CreateTList",test_Tcreate},
  {"InsertTList",test_Tinsert},
  {"FindTList",test_Tfind},
  {"CreateCList",test_Ccreate},
  {"InsertCList",test_Cinsert},
  {"CreateNList",test_Ncreate},
  {"InsertNList",test_Ninsert},
  {"HashCreate",test_Hcreate},
  {"HashInsert",test_Hinsert},
  {"Value_Node_Init", test_Value_Node_Init},
	{"Value_List_Init", test_Value_List_Init},
	{"Add_to_value_list", test_Add_to_value_list},
	{"Spec_node_Init", test_Spec_node_Init},
	{"Spec_List_Init", test_Spec_List_Init},
	{"Specs_Add_Node", test_Specs_Add_Node},
	{"Camera_Init", test_Camera_Init},
  {NULL,NULL}
};
