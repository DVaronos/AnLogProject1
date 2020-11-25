test: test.o  ../hash.o ../list.o ../cameras.o
	gcc -Wall -o test test.o ../hash.o ../list.o ../cameras.o

tect.o: test.c acutest.h ../hash.h
	gcc -Wall -c test.c

run:
	./test

clean:
	rm test *.o
