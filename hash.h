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
