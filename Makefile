sigmod : main.o list.o cameras.o json_read.o
	gcc -o sigmod main.o list.o cameras.o json_read.o

main.o : main.c list.h 
	gcc -c main.c

list.o : list.c list.h
	gcc -c list.c

cameras.o : cameras.c cameras.h
	gcc -c cameras.c

json_read.o : json_read.c json_read.h
	gcc -c json_read.c

clean :
	rm sigmod *.o
