CC = gcc -pthread
OBJ = main.c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

merge: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f main.o merge *~core