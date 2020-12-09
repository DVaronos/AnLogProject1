#include "json_read.h"

typedef struct nlist NList;
typedef struct tlist TList;
//--------------------Domh CList-----------------------------\\


typedef struct clist{
  char* name;
  NList* Head;
  TList* Diffrend;
  struct clist* Next;
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
