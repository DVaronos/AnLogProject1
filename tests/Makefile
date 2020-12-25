test: test.o  ../hash.o ../list.o
	gcc -Wall -o test test.o ../hash.o ../list.o  -lm

tect.o: test.c acutest.h ../logistic.h
	gcc -Wall -c test.c

run:
	./test

vrun:
	valgrind ./test

clean:
	rm test *.o
