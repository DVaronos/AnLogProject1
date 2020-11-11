#include "json_read.h"

typedef struct nlist NList;
//--------------------Domh CList-----------------------------\\


typedef struct clist{
  char* name;
  NList* Head;
  struct clist* Next;
}CList;


CList* CreateCList();
CList* AppendCList(CList* ,CList* );

void InsertCList(CList* ,char* ,NList* );
void PrintCList(CList* ,FILE* );
void FreeCList(CList* );


//--------------------Domh TList-----------------------------\\


typedef struct tlist{
  CList* node;
  struct tlist* Next;
}TList;


TList* CreateTList();

void InsertTList(TList* ,CList*);
void FreeTList(TList* );
void FreeeTList(TList* );

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

//--------------------Domh Hash--------------------\\

typedef struct ht{
  int size;
  NList* Head;
}Hash;

CList* HashConect(Hash* ,char*,CList* );

void HashTransfer(Hash* ,TList* ,FILE* );
void HashInsert(Hash* ,Camera*  );
void HashPrint(Hash* );
void FreeHash(Hash* ,TList* );

int hash(int ,int );

Hash* HashCreate(int );

//--------------------Domh SList-----------------------------\\


typedef struct slist{
  char* name;
  Hash* products;
  struct slist* Next;
}SList;


SList* CreateSList();

void TransferSList(SList* ,FILE* );
void InsertSList(SList* ,Camera* );
void ConectSList(SList* ,char* ,char* );
void PrintSList(SList* );
void FreeSList(SList* );
