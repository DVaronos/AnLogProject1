sigmod : main.o list.o hash.o cameras.o json_read.o
	gcc -o sigmod main.o list.o hash.o cameras.o json_read.o

main.o : main.c hash.h
	gcc -c main.c

hash.o : hash.c hash.h
	gcc -c hash.c

list.o : list.c list.h
	gcc -c list.c

cameras.o : cameras.c cameras.h
	gcc -c cameras.c

json_read.o : json_read.c json_read.h
	gcc -c json_read.c

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
	rm sigmod *.o
