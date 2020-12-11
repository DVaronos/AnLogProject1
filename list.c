#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define TRAIN_PERC 60
// #define TRAIN_PERC 67
// #define TRAIN_PERC 50
#define TEST_PERC 20
// #define TEST_PERC 20
// #define TEST_PERC 25

//--------------------Sinartiseis gia thn domh CList-----------------------------\\


CList* CreateCList(){ //Dimiourgei mia nea CList
	CList *L;
	L=(CList *)malloc(sizeof(CList));
	L->Next=NULL;
  	L->name=NULL;
	L->Diffrend=NULL;
	//--------------------------------------------------
	L->camera_count = 0;
	//--------------------------------------------------
	return L;
}


void InsertCList(CList* L,char* product,NList* Head){//Eisagei ena neo product sthn arxh ths CList
  CList *N;
  N=(CList*)malloc(sizeof(CList));
  N->name=malloc(sizeof(char)*(strlen(product)+1));
  strcpy(N->name,product);
	N->Head=Head;
  N->Next=L->Next;
  L->Next=N;
  //--------------------------------------------------
  L->camera_count++;
  //--------------------------------------------------
}


void TransferCList(CList* L,FILE* csvfile){	//Eisagi  ta proionta sto csv file
  CList* N;
  char* current;
  while(L->Next!=NULL){
    L=L->Next;
    N=L;
    current=malloc(sizeof(char)*(strlen(L->name)+1));//apothikefsi tou trexon komvou
    strcpy(current,L->name);
    while(N->Next!=NULL){	//Eisagogi tou trexon komvou mazi me ton kathe ena ap tous epomenous
      N=N->Next;
      fprintf(csvfile,"%s, %s\n",current,N->name);

    }
    free(current);
  }
}

void DiffCList(CList* L,TList* Transfered,FILE* csvfile){	//Eisagi  ta proionta sto csv file
  TList* N;
	TList* C;
	CList* T;
  char* current;
	int i=0;
	if(L->Diffrend!=NULL){//Ean yparxoun klikes pou diaferoun me thn L
			C=L->Diffrend;	//Krataw thn TList me oles tis klikes pou diaferoun me thn L
			while(L->Next!=NULL){ //Gia kathe proion ths L
				L=L->Next;
				current=malloc(sizeof(char)*(strlen(L->name)+1));//apothikefsi tou trexon komvou
		    strcpy(current,L->name);
				N=C;
				while(N->Next!=NULL){ //Gia kathe lista pou diaferei
					N=N->Next;
					T=N->node;

					if(FindTList(Transfered,T)){ //Ean den exw elegksei thn T pio prin
						while(T->Next!=NULL){ //Gia kathe proion ths T klikas
							T=T->Next;
							fprintf(csvfile,"%s, %s\n",current,T->name);
							i++;
						}
					}

				}
				free(current);
			}
	}
}

void Diff(CList* F,CList* S){
	if(F->Diffrend==NULL){	//Ean h Diffrend ths F einai NULL dimiourgisethn kai vale mesa thn S
		F->Diffrend=CreateTList();
		InsertTList(F->Diffrend,S);
	}else{ //Allios an h S den einai mesa sthn Diffrend ths F vale thn mesa
		if(FindTList(F->Diffrend,S)) InsertTList(F->Diffrend,S);
	}

	if(S->Diffrend==NULL){//Ean h Diffrend ths S einai NULL dimiourgisethn kai vale mesa thn F
		S->Diffrend=CreateTList();
		InsertTList(S->Diffrend,F);
	}else{//Allios an h F den einai mesa sthn Diffrend ths S vale thn mesa
		if(FindTList(S->Diffrend,F)) InsertTList(S->Diffrend,F);
	}
}


CList* AppendCList(CList* L ,CList* N){
	CList* T=L;
	CList* Head;
	Head=N; //To head einai h kefali ths listas N
	int ncount = N->camera_count;
	L->camera_count += ncount; 	//enhmerwnoume to count
	N=N->Next; //O diktis N pleon dixnei ston porto komvo me dedomena ths listas N kai oxi stin kefali ths

	if(Head->Diffrend!=NULL){ //	Oles oi klikes pou htan diaforetikes ap to head twra tha einai diaforetikes me thn L
		TList* Temp=Head->Diffrend;
		while(Temp->Next!=NULL){	//Gia kathe klika diaforetikh ths Head
			Temp=Temp->Next;
			if(L->Diffrend==NULL) L->Diffrend=CreateTList();	//Ean h L den exei Diffrend List dimiourgei
			if(FindTList(L->Diffrend,Temp->node)){	//Ean h paron CList pou einai diaforetikh me thn Head den einai diaforetikh ths L
				InsertTList(L->Diffrend,Temp->node);	//Vale thn stis diaforetikes ths L
				ReplaceTList(Temp->node->Diffrend,Head,L);	//Sthn Diffrend list ths parousas CList antikatestise thn Head me thn L
			}else{	//Ean h paron CList pou einai diaforetikh me thn Head einai diaforetikh ths L diegrapse apo thn Diffrend ths thn  Head
				RemoveTList(Temp->node->Diffrend,Head);
			}
		}
		FreeTList(Head->Diffrend);
	}

	Head->Next=NULL;
	free(Head);	//Apodesmevo thn kefali ths listas N
	while(L->Next!=NULL){//Pigene ston telefteo komvo ths listas L
		L=L->Next;
	}
	L->Next=N;//Valethn lista N sto telos ths listas L
	while(L->Next!=NULL){//O diktis tou komvou pou einai apothikevmeno to kathe neo product na deixnei se afthn thn klika(diladi thn lista L)
		L=L->Next;
		L->Head->clique=T;
	}
	return T;//Epestrepse thn L
}


void FreeCList(CList* L){//Apodesmevi thn CList
  CList* T;
	if(L->Diffrend!=NULL){
		FreeTList(L->Diffrend);
		L->Diffrend=NULL;
	}
  while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    free(T->name);
    free(T);
  }
	free(L->name);
  free(L);
}


//--------------------Sinartiseis gia thn domh TList-----------------------------\\


TList* CreateTList(){ //Dimiourgei mia nea TList
	TList *L;
	L=(TList *)malloc(sizeof(TList));
	L->Next=NULL;
  L->node=NULL;
	return L;
}

int FindTList(TList* L,CList* node){//Epistrefei 0 an iparxi to node sthn lista allios epistrefei 1
	while(L->Next!=NULL){
		L=L->Next;
		if(L->node==node) return 0;
	}
	return 1;
}

void InsertTList(TList* L,CList* node){
		TList* N;
		N=(TList*)malloc(sizeof(TList));
		N->node=node;
	  N->Next=L->Next;
	  L->Next=N;
}

void RemoveTList(TList* L,CList* C){
	TList* T;
	while(L->Next!=NULL){
		if(L->Next->node==C){
			T=L->Next;
			L->Next=T->Next;
			T->node=NULL;
			free(T);
			break;
		}
		L=L->Next;
	}
}

int CountTList(TList* T){
	int i=0;
	while(T->Next!=NULL){
		T=T->Next;
		i++;
	}
	return i;
}

void FreeTList(TList* L){//Apodesmevi thn CList
  TList* T;
  while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    T->node=NULL;
    free(T);
  }
  free(L);
}

// ===============================================================================
void Print_Camera_Count_TList(TList* T)
{
	int i = 1;
	TList* tptr = T->Next;
	while(tptr != NULL)
	{
		printf("list %d has %d cameras\n", i, tptr->node->camera_count);
		tptr = tptr->Next;
		i++;
	}
}
// ===============================================================================



void FreeeTList(TList* L){  //Apodesmevi thn TList kai tis CList pou exei os dedomena
	TList* T;
	while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    FreeCList(T->node);
    free(T);
  }
  free(L);
}

void ReplaceTList(TList* T,CList* P,CList* N){
	while(T->Next!=NULL){
		T=T->Next;
		if(T->node==P){
			T->node=N;
			break;
		}
	}
}

//--------------------Sinartiseis gia thn domh NList-----------------------------\\


NList* CreateNList(){ /*Dimiourgei mia nea SList*/
	NList *L;
	L=(NList *)malloc(sizeof(NList));
	L->Next=NULL;
  L->camera=NULL;
	return L;
}

void InsertNList(NList* L,Camera* camera){//Eisagei ena neo product sthn arxh ths NList
	NList *N;
  N=(NList*)malloc(sizeof(NList));
  N->camera=camera;
	N->clique=NULL;
  N->Next=L->Next;
  L->Next=N;
}


CList* ConectNList(NList* L,char* product,CList* clique){//Eisagei ena neo product sthn arxh ths CList
	while(L->Next!=NULL){	//Vrisko ton komvo ston opio einai apothikevmeno to product
		L=L->Next;
		if(!strcmp(L->camera->id,product)){
				break;
		}
	}

	if(L->clique==NULL){	//Ean den einai to product idi se kapia klika

		if(clique==NULL){	//Ean den exei dwthei kapia klika wste na mpei ekei to product
			L->clique=CreateCList();	//Dimiourgise mia nea klika
			InsertCList(L->clique,product,L);	//Vale to product sthn nea klika
		}else{	//Ean exei dothei mia klika sthn opia prepei na valw afto kai tous komvous ths klikas tou
			L->clique=clique;	//O komvos pou einai apothikevmeno to product dixnei sthn klika pou dothike
			InsertCList(L->clique,product,L);	//vale to product sthn klika pou dothike
		}

	}else{	//Ean to product einai idi se kapia klika

		if(clique!=NULL){ //Ean exei dwthei kapia klika wste na mpei ekei to product
			if(L->clique!=clique){	//ean oi dio klikes den einai idies
					L->clique=AppendCList(clique,L->clique);	//sigxonevw tis dio klikes
			}
		}

	}
		return L->clique; //Epistrefo thn klika sthn opia evala to product
}


void PrintNList(NList* L){//Ektiponei thn NList
  while(L->Next!=NULL){
    L=L->Next;
		Camera_Print_Specs(L->camera);
	}
}

void TransferNList(NList* L,TList* Transfered,FILE* csvfile){	//Metaferi ta dedomena  ths NList
  while(L->Next!=NULL){
    L=L->Next;
		if(L->clique!=NULL){
				if(FindTList(Transfered,L->clique)){//Ean h klika pou dixnei o komvos L den exei metaferthi
						TransferCList(L->clique,csvfile);
						InsertTList(Transfered,L->clique);//Vale thn klika pou dixnei o komvos L se aftes pou exoun metaferthei
				}
  		}
		}
}

void DiffNList(NList* L,TList* Transfered,FILE* csvfile){	//Metaferi ta dedomena  ths NList
  while(L->Next!=NULL){
    L=L->Next;
		if(L->clique!=NULL){
				if(FindTList(Transfered,L->clique)){//Ean h klika pou dixnei o komvos L den exei metaferthi
						DiffCList(L->clique,Transfered,csvfile);
						InsertTList(Transfered,L->clique);//Vale thn klika pou dixnei o komvos L se aftes pou exoun metaferthei
				}
  		}

		}
}

void FreeNList(NList* L,TList* Deleted){//Apodesmevi thn NList
	NList* T;
  while(L->Next!=NULL){
    T=L->Next;
    L->Next=T->Next;
    if(T->camera!=NULL) Delete_Camera(T->camera);
		if(T->clique!=NULL){
			if(FindTList(Deleted,T->clique)){//Ean h klika pou dixnei o komvos L den exei ektipothei
				InsertTList(Deleted,T->clique);//Vale thn klika pou dixnei o komvos L stis ektipomenes
			}
		}
		free(T);
  }
  free(L);
}

//--------------------Sinartiseis gia thn domh LList-----------------------------\\


LList* CreateLList(){ //Dimiourgia mias neas LList
	LList *L;
	L=(LList *)malloc(sizeof(LList));
	L->Next=NULL;
	L->Last=NULL;
  L->camera=NULL;
	return L;

}

void InsertLList(LList* L,int value){	//Eisagei ena neo value stho telos ths LList
	LList *N;
  N=(LList*)malloc(sizeof(LList));
	N->value=value;
	N->Next=NULL;
	if(L==NULL) printf("oxx\n");
	if(L->Last==NULL){
		L->Next=N;
		L->Last=N;
	}else{
		L->Last->Next=N;
		L->Last=N;

	}
}

void FreeLList(LList* L){ //apodesmefsh ths listas
	LList* T;
	while(L->Next!=NULL){
		T=L->Next;
		L->Next=T->Next;
		free(T);
	}
	free(L);
}

void IncreaseLastValue(LList* L){ //Afkasmh thn timh tou telefteou komvou ths listas Kata ena
  L->Last->value++;
}

void PrintLValue(LList* L,int h){
  int i=0;
  while(L->Next!=NULL){
    L=L->Next;
    i++;
    if(i==h){
      printf(" %f ",L->value);
      break;
    }
  }
}

float ReturnLValue(LList* L,int h){	//Epistefh thn timh ths listas gia ipsos h
  int i=0;
  while(L->Next!=NULL){
    L=L->Next;
    i++;
    if(i==h){
      return L->value;
    }
  }
}

void ReplaceLValue(LList* L,int h,float value){ //Antikathista thn timh ths listas sto h ipsos me value
  int i=0;
  while(L->Next!=NULL){
    L=L->Next;
    i++;
    if(i==h){
      L->value=value;
			break;
    }
  }
}

float CountLList(LList* L){		//Episteh to athrisma twn timwn ths listas
  float sum=0;
  while(L->Next!=NULL){
    L=L->Next;
    if(L->value){
      sum++;
    }
  }
	return sum;
}

void PrintLList(LList* L){
	printf("[");
  while(L->Next!=NULL){
    L=L->Next;
    printf(" %f ",L->value);
  }
	printf("]\n");
}

//--------------------Sinartiseis gia thn domh TrainSetList-----------------------------\\

TrainSetList* InitializeTrainSetList()
{
	TrainSetList* tsl;
	tsl = malloc(sizeof(TrainSetList));
	tsl->camera_count = 0;
	tsl->first = NULL;
	tsl->last = NULL;
	return tsl;
}

TrainSetList* InsertCameraToTrainSetList(TrainSetList* tsl, Camera* camera)
{
	TrainSetNode* tsn = InitializeTrainSetNode(camera);
	if(tsl->first == NULL)
		tsl->first = tsn;
	if(tsl->last != NULL)
		tsl->last->next = tsn;
	tsl->last = tsn;
	tsl->camera_count++;
	return tsl;
}

void PrintTrainSetList(TrainSetList* tsl)
{
	TrainSetNode* tsn = tsl->first;
	printf("---------------Training Set--------------- \n");
	if(tsl != NULL)
		while(tsn != NULL)
		{
			PrintTrainSetNode(tsn);
			tsn = tsn->next;
		}
	else
		printf("Empty Train Set List\n");
}


void PrintTrainSetCameraCount(TrainSetList* tsl)
{
	printf("Train Set contains %d cameras\n", tsl->camera_count);
}

int FreeTrainSetList(TrainSetList* tsl)
{
	if(tsl == NULL)
	{
		printf("Empty Train Set List\n");
		return 0;
	}
	TrainSetNode* tsn = tsl->first;
	TrainSetNode* tsn2;
	while(tsn != NULL)
	{
		tsn2 = tsn->next;
		FreeTrainSetNode(tsn);
		tsn = tsn2;
	}
	free(tsl);
	return 1;
}

//--------------------Sinartiseis gia thn domh TrainSetNode-----------------------------\\

TrainSetNode* InitializeTrainSetNode(Camera* camera)
{
	TrainSetNode* tsn;
	tsn = malloc(sizeof(TrainSetNode));
	tsn->camera = camera;
	tsn->next = NULL;
	return tsn;
}

void PrintTrainSetNode(TrainSetNode* tsn)
{
	if(tsn != NULL)
		printf("camera's id %s\n", tsn->camera->id);
	else
		printf("Train Set node is NULL\n");
}

int FreeTrainSetNode(TrainSetNode* tsn)
{
	if(tsn == NULL)
	{
		// empty node
		return 0;
	}
	free(tsn);
	return 1;
}

//--------------------Sinartiseis gia thn domh TestSetList-----------------------------\\

TestSetList* InitializeTestSetList()
{
	TestSetList* tsl;
	tsl = malloc(sizeof(TestSetList));
	tsl->camera_count = 0;
	tsl->first = NULL;
	tsl->last = NULL;
	return tsl;
}

TestSetList* InsertCameraToTestSetList(TestSetList* tsl, Camera* camera)
{
	TestSetNode* tsn = InitializeTestSetNode(camera);
	if(tsl->first == NULL)
		tsl->first = tsn;
	if(tsl->last != NULL)
		tsl->last->next = tsn;
	tsl->last = tsn;
	tsl->camera_count++;
	return tsl;
}

void PrintTestSetList(TestSetList* tsl)
{
	TestSetNode* tsn = tsl->first;
	printf("---------------Training Set--------------- \n");

	if(tsl != NULL)
		while(tsn != NULL)
		{
			PrintTestSetNode(tsn);
			tsn = tsn->next;
		}
	else
		printf("Empty Test Set List\n");
}

void PrintTestSetCameraCount(TestSetList* tsl)
{
	printf("Test Set contains %d cameras\n", tsl->camera_count);
}

int FreeTestSetList(TestSetList* tsl)
{
	if(tsl == NULL)
	{
		printf("Empty Test Set List\n");
		return 0;
	}
	TestSetNode* tsn = tsl->first;
	TestSetNode* tsn2;
	while(tsn != NULL)
	{
		tsn2 = tsn->next;
		FreeTestSetNode(tsn);
		tsn = tsn2;
	}
	free(tsl);
	return 1;
}


//--------------------Sinartiseis gia thn domh TestSetNode-----------------------------\\

TestSetNode* InitializeTestSetNode(Camera* camera)
{
	TestSetNode* tsn;
	tsn = malloc(sizeof(TestSetNode));
	tsn->camera = camera;
	tsn->next = NULL;
	return tsn;
}

void PrintTestSetNode(TestSetNode* tsn)
{
	if(tsn != NULL)
		printf("camera's id %s\n", tsn->camera->id);
	else
		printf("Test Set node is NULL\n");
}

int FreeTestSetNode(TestSetNode* tsn)
{
	if(tsn == NULL)
	{
		// empty node
		return 0;
	}
	free(tsn);
	return 1;
}

// --------------------------------------------------------------------------------------\\

void Percentage_Calculation(int camera_count, int* train_perc, int* test_perc)
{
	*train_perc = (camera_count * TRAIN_PERC) / 100;
	*test_perc = (camera_count * TEST_PERC) / 100;
}

// --------------------------------------------------------------------------------------\\


void SplitToTrainTestValidationSet(TrainSetList** trainsl, TestSetList** testsl, ValidationSetList** validationsl,  TList* tl)
{
	int set_count, i, train_perc, test_perc;	//i-> poses cameres exoume diavasei (kai valei sto set train) apo thn ka8e klika
	TList* tlptr = tl->Next; 	//Tlist node ptr (twra deixnei sto prwto node me dedomena (epomeno apo thn kefalida))
	// CList* cnptr = tl->node;	//clist node ptr (twra deixnei sthn clist gia thn prwth klika?)
	CList* cnptr;

	*trainsl = InitializeTrainSetList();
	*testsl = InitializeTestSetList();
	*validationsl = InitializeValidationSetList();

	while(tlptr != NULL)
	{
		cnptr = tlptr->node;
		Percentage_Calculation(cnptr->camera_count, &train_perc, &test_perc);
		i = 0;
		cnptr = cnptr->Next;
		while(cnptr != NULL)
		{
			if(i < train_perc)
				*trainsl = InsertCameraToTrainSetList(*trainsl, cnptr->Head->camera);	//apo thn klika pisw sto ->  antistoixo node tou hash -> camera
			else if ( i <= train_perc + test_perc)
				*testsl = InsertCameraToTestSetList(*testsl, cnptr->Head->camera);
			else
				*validationsl = InsertCameraToValidationSetList(*validationsl, cnptr->Head->camera);
			i++;
			cnptr = cnptr->Next;
		}

		tlptr = tlptr->Next;
	}
}

//--------------------Sinartiseis gia thn domh ValidationSetList-----------------------------\\

ValidationSetList* InitializeValidationSetList()
{
	ValidationSetList* vsl;
	vsl = malloc(sizeof(ValidationSetList));
	vsl->camera_count = 0;
	vsl->first = NULL;
	vsl->last = NULL;
	return vsl;
}

ValidationSetList* InsertCameraToValidationSetList(ValidationSetList* vsl, Camera* camera)
{
	ValidationSetNode* vsn = InitializeValidationSetNode(camera);
	if(vsl->first == NULL)
		vsl->first = vsn;
	if(vsl->last != NULL)
		vsl->last->next = vsn;
	vsl->last = vsn;
	vsl->camera_count++;
	return vsl;
}

void PrintValidationSetList(ValidationSetList* vsl)
{
	ValidationSetNode* vsn = vsl->first;
	printf("---------------Training Set--------------- \n");

	if(vsl != NULL)
		while(vsn != NULL)
		{
			PrintValidationSetNode(vsn);
			vsn = vsn->next;
		}
	else
		printf("Empty Validation Set List\n");
}

void PrintValidationSetCameraCount(ValidationSetList* vsl)
{
	printf("Validation Set contains %d cameras\n", vsl->camera_count);
}

int FreeValidationSetList(ValidationSetList* vsl)
{
	if(vsl == NULL)
	{
		printf("Empty Test Set List\n");
		return 0;
	}
	ValidationSetNode* vsn = vsl->first;
	ValidationSetNode* vsn2;
	while(vsn != NULL)
	{
		vsn2 = vsn->next;
		FreeValidationSetNode(vsn);
		vsn = vsn2;
	}
	free(vsl);
	return 1;
}

//--------------------Sinartiseis gia thn domh ValidationSetNode-----------------------------\\

ValidationSetNode* InitializeValidationSetNode(Camera* camera)
{
	ValidationSetNode* vsn;
	vsn = malloc(sizeof(ValidationSetNode));
	vsn->camera = camera;
	vsn->next = NULL;
	return vsn;
}

void PrintValidationSetNode(ValidationSetNode* vsn)
{
	if(vsn != NULL)
		printf("camera's id %s\n", vsn->camera->id);
	else
		printf("Validation Set node is NULL\n");
}

int FreeValidationSetNode(ValidationSetNode* vsn)
{
	if(vsn == NULL)
	{
		// empty node
		return 0;
	}
	free(vsn);
	return 1;
}

//-------------------------------------------------------------------------------------------\\

void PrintAllPairsTrainSet(TrainSetList* tsl)
{
	TrainSetNode* tsn1_ptr, *tsn2_ptr;
	tsn1_ptr = tsl->first;
	while(tsn1_ptr != NULL)
	{
		tsn2_ptr = tsn1_ptr->next;
		while(tsn2_ptr != NULL)
		{
			printf("\n");
			PrintTrainSetNode(tsn1_ptr);
			PrintTrainSetNode(tsn2_ptr);
			printf("==============================\n");
			tsn2_ptr = tsn2_ptr->next;
		}
		tsn1_ptr = tsn1_ptr->next;
	}

}

