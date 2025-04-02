# vim: set noexpandtab:

mainked: src/mainked.c bin/appbuff.o
	gcc -o bin/inked -Wall -Wextra -pedantic -std=c17 $^

appbuff.o: src/appbuff.c src/appbuff.h
	gcc -c src/appbuff.c -o bin/appbuff.o

keys: src/keys.c
	gcc -o bin/keys -Wall -Wextra -pedantic -std=c17 src/keys.c
