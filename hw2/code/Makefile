collectS : helper.o collectS.o malloc.o 
				gcc -o collectS helper.o collectS.o malloc.o 

helper.o : ./helper/helper.c ./helper/helper.h
				  gcc -c -w ./helper/helper.c
malloc.o : ./malloc/malloc.c 
				  gcc -c -w ./malloc/malloc.c
collectS.o : collectS.c 
				  gcc -c -w collectS.c

clean :
			rm collectS collectS.o helper.o malloc.o 
