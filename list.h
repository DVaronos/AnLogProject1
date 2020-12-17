

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


//--------------------Domh WHash-----------------------------\\

typedef struct whash{
  double tf;
  char* word;
  int count;
  int size;
}WHash;


WHash* CreateWHash(int );
WHash* InsertWHash(WHash* ,char* );
WHash* Wrehash(WHash* );
WHash* WHashTF(WHash* );

int WHashCount(WHash* );
int whashf(char* ,int );
int WHashFind(WHash* ,char* );

double GiveTF(WHash* ,char* );

void FreeWHash(WHash* );
void PrintWHash(WHash* );



//--------------------Domh NList-----------------------------\\


struct nlist{
  char* camera;
  WHash* Spear;
  CList* clique;
  double* vector;
  struct nlist* Next;
};


NList* CreateNList();

CList* ConectNList(NList* ,char* ,CList* );

int NListHmt(NList* ,char* );

void TransferNList(NList* ,TList* ,FILE* );
void InsertNList(NList* ,char* );
void PrintNList(NList* );
void FreeNList(NList* ,TList* );
void DiffNList(NList* ,TList * ,FILE* );
