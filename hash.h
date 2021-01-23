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

void HashPrintP(Hash* );

int hash(int ,int );

WHash* HashFind(Hash* ,char* );

Hash* HashCreate(int );
Hash* HashInsert(Hash* ,char* );
Hash* rehash(Hash* );
Hash* HashReplaceSpear(Hash* ,char* ,WHash* );

//dexetai duo cameres kai elegxei an h mia vrisketai sth lista me ta anti8eta ths allhs
int CheckIfOpposite (Hash* H, char* first, char* second);



//--------------------Domh LHash--------------------\\


typedef struct lh{
  char* word;
  double idf;
  double tfcount;
  int size;
  int count;
  int wordperj;

}LHash;

int hash1(char* ,int);
int LHashFind(LHash* ,char* );
int LHashPartition(LHash** ,int ,int );

void FreeLHash(LHash* );


void LHashTfIdf(LHash* ,double );
LHash* LHashIncreaseTf(LHash* ,char* ,double );
LHash* Readjson(char* ,LHash* ,LHash* ,WHash** );
LHash* LHashCreate(int );
LHash* LHashInsert(LHash* ,char* );
LHash* LHashPrint(LHash* );
LHash* Lrehash(LHash* );
LHash* LHashSort(LHash* ,int ,int );
LHash* LHashSwap(LHash* ,int ,int );
LHash* NMostLHash(LHash* ,int);

Hash* HashVectorts(Hash* ,LHash* );
