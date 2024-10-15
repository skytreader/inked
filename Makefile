# vim: set noexpandtab:

mainked: src/mainked.c
	gcc -o bin/inked -Wall -Wextra -pedantic -std=c17 src/mainked.c
