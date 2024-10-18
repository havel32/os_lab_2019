CC=gcc
CFLAGS=-I.

all: sequential_min_max parallel_min_max exec_sequential

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

exec_sequetial.0 : exec_sequetial.c
	$(CC) -o utils.o -c utils.c $(CFLAGS)

exec_sequetial : exec_sequetial.o
	$(CC) -o exec_sequetial exec_sequetial.o $(CFLAGS)
clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max exec_sequential
