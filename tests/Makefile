all: CList_test	TList_test NList_test Hash_test SList_test

CList_test: CList_test.o ../list.o
	gcc -o CList_test CList_test.o ../list.o

TList_test: TList_test.o ../list.o
	gcc -o TList_test TList_test.o ../list.o

NList_test:	NList_test.o ../list.o
	gcc -o NList_test NList_test.o ../list.o

Hash_test:	Hash_test.o ../list.o
	gcc -o Hash_test Hash_test.o ../list.o

SList_test: SList_test.o ../list.o
	gcc -o SList_test SList_test.o ../list.o

CList_test.o: CList_test.c ../list.h
	gcc -c CList_test.c

TList_test.o: TList_test.c ../list.h
	gcc -c TList_test.c

Hash_test.o: Hash_test.c ../list.h
	gcc -c Hash_test.c

NList_test.o: NList_test.c ../list.h
	gcc -c NList_test.c

SList_test.o: SList_test.c ../list.h
	gcc -c SList_test.c


clean:
	rm CList_test TList_test NList_test Hash_test SList_test CList_test.o TList_test.o NList_test.o Hash_test.o SList_test.o
