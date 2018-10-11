collectM : helper.o collectM.o malloc.o
	gcc -o collectM helper.o collectM.o malloc.o

collectS : helper.o collectS.o malloc.o
	gcc -o collectS helper.o collectS.o malloc.o

helper.o : ./helper/helper.c ./helper/helper.h
	gcc -c -w ./helper/helper.c
malloc.o : ./malloc/malloc.c
	gcc -c -w ./malloc/malloc.c

collectS.o : collectS.c
	gcc -c -w collectS.c

collectM.o : collectM.c
	gcc -lpthread -c -w collectM.c

clean :
	rm collectM collectM.o helper.o malloc.o

cleanS:
	rm collectS collectS.o helper.o malloc.o
