bmptool:	b-lib.o main.o
	$(CC) b-lib.o main.o -o $@

bmplib.o:	b-lib.c b-lib.h
	$(CC) -Wall -c $< -o $@

main.o:	main.c b-lib.h 
	$(CC) -Wall -c $< -o $@

clean:
	rm -f bmptool *.o core *~