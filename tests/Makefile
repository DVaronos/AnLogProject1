test: test.o  ../hash.o ../list.o ../model.o
	gcc -Wall -o test test.o ../hash.o ../list.o ../model.o -lm

tect.o: test.c acutest.h ../model.h
	gcc -Wall -c test.c

run:
	./test

clean:
	rm test *.o
