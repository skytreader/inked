# vim: set noexpandtab:

mainked: src/mainked.c
	gcc -o bin/inked -Wall -Wextra -pedantic -std=c17 src/mainked.c

keys: src/keys.c
	gcc -o bin/keys -Wall -Wextra -pendantic -stc=c117 src/keys.c
