CC = gcc
CFLAGS = -Wall
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

largest: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f main.o largest *~core