

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

void InsertTlist(TList* ,CList*);
void FreeTList(TList* );
void FreeeTList(TList* );

int FindTList(TList* ,CList*);

//--------------------Domh NList-----------------------------\\


struct nlist{
  char* name;
  CList* clique;
  struct nlist* Next;
};


NList* CreateNList();

CList* FindNListNode(NList* ,char* );
CList* InsertNList(NList* ,char* ,CList* );

void PrintNList(NList* ,TList* ,FILE* );
void FreeNList(NList* ,TList* );

//--------------------Domh Hash--------------------\\

typedef struct ht{
  int size;
  NList* Head;
}Hash;


CList* HashInsert(Hash* ,char*,CList* );

void HashPrint(Hash* ,TList* ,FILE* );
void HashDelete(Hash* ,int );
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

void InsertSList(SList* ,char* ,char* );
void PrintSList(SList* ,FILE* );
void FreeSList(SList* );
