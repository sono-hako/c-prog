all:	pipe

pipe:	main.o
	gcc main.o -o pipe

main.o:	main.c
	gcc -c main.c -o main.o

clean:
	rm -f pipe main.o core *~
