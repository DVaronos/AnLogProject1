#include "list.h"

//--------------------Domh Hash--------------------\\

typedef struct ht{
  int size;
  int count;
  NList* Head;
}Hash;

void HashConect(Hash* ,char*,char* ,int );
void HashTransfer(Hash* ,FILE* );
void HashPrint(Hash* );
void FreeHash(Hash* );
void HashDiff(Hash* ,FILE* );

int hash(int ,int );

Hash* HashCreate(int );
Hash* HashInsert(Hash* ,Camera*  );
Hash* rehash(Hash* );


//--------------------Domh LHash--------------------\\


typedef struct lh{
  char* word;
  float idf;
  int size;
  int count;
  int height;
  LList* List;
}LHash;

int hash1(char* ,int);
int LHashFind(LHash* ,char* );
int LHashPartition(LHash** ,int ,int );

void FreeLHash(LHash* );
void LHashIncreaseHeight(LHash* );
void LHashTF(LHash* );

LHash* LHashCreate(int );
LHash* LHashInsert(LHash* ,char*  );
LHash* LHashPrint(LHash* );
LHash* Lrehash(LHash* );
LHash* LHashSort(LHash* ,int ,int );
LHash* LHashSwap(LHash* ,int ,int );
LHash* NMostLHash(LHash* ,int);
LHash* Camera_to_string(Camera* ,LHash* );
