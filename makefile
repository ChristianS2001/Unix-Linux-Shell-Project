CC=gcc
CFLAGS=-I. -Wall -Werror
DEPS = external.h internal.h parser.h
OBJ = parser.o myshell.o external.o internal.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

myshell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ./*.o *~ core 
