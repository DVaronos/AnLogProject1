sigmod : main.o logistic.o list.o hash.o
	gcc -o sigmod main.o logistic.o list.o hash.o  -lm

main.o : main.c logistic.h
	gcc -c main.c

logistic.o : logistic.c logistic.h
	gcc -c logistic.c

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

test-vrun:
	$(MAKE) -C tests vrun

test-clean:
	$(MAKE) -C tests	clean

clean :
	rm sigmod *.o Diffrend.csv Same.csv
