test: test.o  ../hash.o ../list.o
	gcc -Wall -g -o test test.o ../hash.o ../list.o  -lm

tect.o: test.c acutest.h ../logistic.h
	gcc -Wall -g -c test.c

run:
	./test

vrun:
	valgrind ./test

clean:
	rm test *.o
