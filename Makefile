sigmod : main.o list.o hash.o
	gcc -o sigmod main.o list.o hash.o  -lm

main.o : main.c hash.h
	gcc -c main.c

hash.o : hash.c hash.h
	gcc -c hash.c

list.o : list.c list.h
	gcc -c list.c

run:
	./sigmod $(ARGS)

v-run:
		valgrind ./sigmod $(ARGS)
test:
	$(MAKE) -C tests

test-run:
	$(MAKE) -C tests run

test-clean:
	$(MAKE) -C tests	clean

clean :
	rm sigmod *.o Diffrend.csv Same.csv
