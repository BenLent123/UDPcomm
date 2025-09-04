CC=gcc
CFLAGS=-Wall -O2 -Iinc

SRC=src/CLI.c src/socket.c src/UDP.c src/chat.c src/yggdrasil.c
OBJ=$(SRC:.c=.o)

comm: $(OBJ)
	$(CC) $(CFLAGS) -o comm $(OBJ)
	rm -f $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o comm
