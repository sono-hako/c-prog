CC = gcc
OBJ = tail.c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

rep: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f tail.o rep *~core