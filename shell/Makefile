all:	shell

shell:	shell.o
	gcc shell.o -o shell

myshell.o:	shell.c
	gcc -c shell.c -o shell.o

clean:
	rm -f shell shell.o core *~
