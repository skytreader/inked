# vim: set noexpandtab:

DEFAULT_FLAGS = -fstack-protector-strong -Wall -Wextra -pedantic -std=c17

mainked: src/mainked.c bin/appbuff.o bin/textbuff.o
	gcc $(DEFAULT_FLAGS) -o bin/inked $^

%.o: src/%.c src/%.h
	gcc $(DEFAULT_FLAGS) -c src/$*.c -o bin/$@

%-tests.o: src/tests/%-tests.c
	gcc $(DEFAULT_FLAGS) -c $^ -o bin/$@

%-tests: %-tests.o %.o
	gcc $(DEFAULT_FLAGS) -o bin/$@ bin/$*-tests.o bin/$*.o -lcriterion

keys: src/keys.c
	gcc -o bin/keys $(DEFAULT_FLAGS) src/keys.c
