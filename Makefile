CC=gcc
CFLAGS=-Wall -O2 -Iinc -Ilibqrencode

SRC=src/CLI.c src/socket.c src/UDP.c src/chat.c src/yggdrasil.c src/qr.c
OBJ=$(SRC:.c=.o)

yggcomm: $(OBJ)
	$(CC) $(CFLAGS) -o yggcomm $(OBJ)
	rm -f $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o yggcomm
