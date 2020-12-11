#include "json_read.h"

typedef struct nlist NList;
typedef struct tlist TList;
//--------------------Domh CList-----------------------------\\


typedef struct clist{
  char* name;
  NList* Head;
  TList* Diffrend;
  struct clist* Next;
  //-------------------------
  int camera_count;     //-->metraei poses kameres einai mesa sthn qlique
  //-------------------------
}CList;


CList* CreateCList();
CList* AppendCList(CList* ,CList* );

void InsertCList(CList* ,char* ,NList* );
void PrintCList(CList* ,FILE* );
void FreeCList(CList* );
void Diff(CList* ,CList* );
void DiffCList(CList*,TList* ,FILE* );

//--------------------Domh TList-----------------------------\\


typedef struct tlist{
  CList* node;
  struct tlist* Next;
}TList;


TList* CreateTList();

void InsertTList(TList* ,CList*);
void FreeTList(TList* );
void FreeeTList(TList* );
void ReplaceTList(TList* ,CList* ,CList* );
void RemoveTList(TList* ,CList* );

int CountTList(TList* );
int FindTList(TList* ,CList*);
// ===========================================
void Print_Camera_Count_TList(TList* T);
//--------------------Domh NList-----------------------------\\


struct nlist{
  Camera* camera;
  CList* clique;
  struct nlist* Next;
};


NList* CreateNList();

CList* ConectNList(NList* ,char* ,CList* );

void TransferNList(NList* ,TList* ,FILE* );
void InsertNList(NList* ,Camera* );
void PrintNList(NList* );
void FreeNList(NList* ,TList* );
void DiffNList(NList* ,TList * ,FILE* );

//--------------------Domh LList-----------------------------\\

typedef struct llist{
  float value;
  char* camera;
  struct llist* Next;
  struct llist* Last;
}LList;

LList* CreateLList();

float ReturnLValue(LList* ,int );
float CountLList(LList* );

void InsertLList(LList* ,int );
void FreeLList(LList* );
void IncreaseLastValue(LList* );
void PrintLValue(LList* ,int );
void PrintLList(LList* );
void ReplaceLValue(LList* ,int ,float );


//--------------------Domh TrainSetList - Node -----------------------------\\

typedef struct TrainSetNode
{
  Camera* camera;
  struct TrainSetNode* next;
}TrainSetNode;

typedef struct TrainSetList
{
  int camera_count;
  TrainSetNode* first;
  TrainSetNode* last;
}TrainSetList;

TrainSetList* InitializeTrainSetList();
TrainSetList* InsertCameraToTrainSetList(TrainSetList* tsl, Camera* camera);
void PrintTrainSetList(TrainSetList* tsl);
void PrintTrainSetCameraCount(TrainSetList* tsl);
int FreeTrainSetList(TrainSetList* tsl);
// -----------------------------------------
TrainSetNode* InitializeTrainSetNode(Camera* camera);
void PrintTrainSetNode(TrainSetNode* tsn);
int FreeTrainSetNode(TrainSetNode* tsn);


//--------------------Domh TestSetList - Node -----------------------------\\

typedef struct TestSetNode
{
  Camera* camera;
  struct TestSetNode* next;
}TestSetNode;

typedef struct TestSetList
{
  int camera_count;
  TestSetNode* first;
  TestSetNode* last;
}TestSetList;

TestSetList* InitializeTestSetList();
TestSetList* InsertCameraToTestSetList(TestSetList* tsl, Camera* camera);
void PrintTestSetList(TestSetList* tsl);
void PrintTestSetCameraCount(TestSetList* tsl);
int FreeTestSetList(TestSetList* tsl);
// ---------------------------------------
TestSetNode* InitializeTestSetNode(Camera* camera);
void PrintTestSetNode(TestSetNode* tsn);
int FreeTestSetNode(TestSetNode* tsn);


//--------------------Domh ValidationSetList-----------------------------\\

typedef struct ValidationSetNode
{
  Camera* camera;
  struct ValidationSetNode* next;
}ValidationSetNode;

typedef struct ValidationSetList
{
  int camera_count;
  ValidationSetNode* first;
  ValidationSetNode* last;
}ValidationSetList;

ValidationSetList* InitializeValidationSetList();
ValidationSetList* InsertCameraToValidationSetList(ValidationSetList* vsl, Camera* camera);
void PrintValidationSetList(ValidationSetList* vsl);
void PrintValidationSetCameraCount(ValidationSetList* vsl);
int FreeValidationSetList(ValidationSetList* vsl);
// ---------------------------------------
ValidationSetNode* InitializeValidationSetNode(Camera* camera);
void PrintValidationSetNode(ValidationSetNode* vsn);
int FreeValidationSetNode(ValidationSetNode* vsn);

// =========================================================\\

void Percentage_Calculation(int camera_count, int* train_perc, int* test_perc);
void SplitToTrainTestValidationSet(TrainSetList** trainsl, TestSetList** testsl, ValidationSetList** validationsl,  TList* tl);

void PrintAllPairsTrainSet(TrainSetList* tsl);
