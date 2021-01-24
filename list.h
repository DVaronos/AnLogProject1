

typedef struct nlist NList;
typedef struct tlist TList;
//--------------------Domh CList-----------------------------//


typedef struct clist{
  char* name;
  NList* Head;
  TList* Diffrend;
  struct clist* Next;
  int camera_count;
}CList;


CList* CreateCList();
CList* AppendCList(CList* ,CList* );

void InsertCList(CList* ,char* ,NList* );
void FreeCList(CList* );
void Diff(CList* ,CList* );
void DiffCList(CList*,TList* ,FILE* );

int SearchDiffList(CList* , CList* );

//--------------------Domh TList-----------------------------//


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
void Print_Camera_Count_TList(TList* T);

//--------------------Domh WHash-----------------------------//

typedef struct whash{
  double tfidf;
  char* word;
  int count;
  int size;
}WHash;


WHash* CreateWHash(int );
WHash* InsertWHash(WHash* ,char* );
WHash* Wrehash(WHash* );
WHash* WHashTF(WHash* );
WHash* MulTfIdf(WHash* ,char* ,double );

int WHashCount(WHash* );
int whashf(char* ,int );
int WHashFind(WHash* ,char* );

double GiveTfIdf(WHash* ,char* );

void FreeWHash(WHash* );
void PrintWHash(WHash* );


//--------------------Domh HVector-----------------------------//

typedef struct hvector{
  int key;
  double value;
  double sum;
  int count;
  int size;
}HVector;

int HVhash(int ,int );
HVector* CreateHVector(int );
HVector* InsertHVector(HVector* ,int  ,double );
HVector* HVrehash(HVector* );

double HVGetValue(HVector* ,int );
double HVSumValues(HVector* );


void PrintHVector(HVector* );
void FreeHVector(HVector* );
//--------------------Domh NList-----------------------------//


struct nlist{
  char* camera;
  WHash* Spear;
  CList* clique;
  HVector* vector;
  struct nlist* Next;
  int vec_size;
};


NList* CreateNList();

CList* ConectNList(NList* ,char* ,CList* );

void TransferNList(NList* ,TList* ,FILE* );
void InsertNList(NList* ,char* );
void PrintNList(NList* );
void FreeNList(NList* ,TList* );
void DiffNList(NList* ,TList * ,FILE* );
