#include "list.h"

//--------------------Domh Hash--------------------\\

typedef struct ht{
  int size;
  int count;
  NList* Head;
}Hash;

void HashConect(Hash* ,char*,char* ,int );

void HashTransfer(Hash* ,FILE* );
Hash* HashInsert(Hash* ,Camera*  );
void HashPrint(Hash* );
void FreeHash(Hash* );
void HashDiff(Hash* );

int hash(int ,int );

Hash* HashCreate(int );
Hash* rehash(Hash* );
