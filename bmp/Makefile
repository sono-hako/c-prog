bmptool:	bmplib.o main.o
	gcc -Wall bmplib.o main.o -o bmptool

bmplib.o:	bmplib.c bmplib.h
	gcc -Wall -c bmplib.c bmplib.h

main.o:	main.c bmplib.h 
	gcc -Wall -c main.c bmplib.h

clean:
	rm -f bmptool *.o *.gch core *~
