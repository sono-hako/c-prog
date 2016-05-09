CC = gcc
CFLAGS = -Wall
DEPS = calc.h
OBJ = stack.o getop.o getch.o main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

rpn: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f getch.o getop.o main.o stack.o rpn *~core