all: CList_test	TList_test NList_test Hash_test SList_test Camera_test run rm

CList_test: CList_test.o ../list.o ../cameras.o
	gcc -o CList_test CList_test.o ../list.o ../cameras.o

TList_test: TList_test.o ../list.o ../cameras.o
	gcc -o TList_test TList_test.o ../list.o ../cameras.o

NList_test:	NList_test.o ../list.o ../cameras.o
	gcc -o NList_test NList_test.o ../list.o ../cameras.o

Hash_test:	Hash_test.o ../list.o ../cameras.o
	gcc -o Hash_test Hash_test.o ../list.o ../cameras.o

SList_test: SList_test.o ../list.o ../cameras.o
	gcc -o SList_test SList_test.o ../list.o ../cameras.o

Camera_test: camera_testing.o ../cameras.o
	gcc -o Camera_test camera_testing.o ../cameras.o

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

camera_testing.o: camera_testing.c ../cameras.h
	gcc -c camera_testing.c

run:
	./CList_test
	./TList_test
	./NList_test
	./Hash_test
	./SList_test
	./Camera_test

rm:
	rm CList_test TList_test NList_test Hash_test SList_test Camera_test *.o
