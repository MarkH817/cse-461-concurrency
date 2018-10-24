sortS: sort.o helper.o malloc.o
					gcc -pg -lpthread -o sortS sort.o helper.o malloc.o 

sort.o : sort.c 
				  	gcc -pg -lpthread -c -w sort.c

malloc.o : ./malloc/malloc.c
					gcc -pg -c -w ./malloc/malloc.c

helper.o : ./helper/helper.c ./helper/helper.h
				  	gcc -pg -c -w ./helper/helper.c

run-timer:
					/usr/bin/time ./sortS

run-taskset:
					taskset 0xFF ./sortS
					gprof ./sortS > sortS.perf

clean :
					rm sortS sort.o helper.o malloc.o sortS.perf gmon.out
