CC = gcc
CFLAGS = -Wall
OBJ = complex.c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

question2_3: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f complex.o complex *~core