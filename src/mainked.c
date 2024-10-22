#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    tcgetattr(STDIN_FILENO, &raw);
    /*
     * Flags turned _off_:
     *
     * ICRNL - allows Ctrl-M to be read as Carriage Return/New Line.
     *
     * IXON - input transmission on. With this turned off, no data is
     * transmitted to the terminal at all. Ctrl-S triggers XOFF (pause
     * transmission) while Ctrl-Q triggers XON (resume transmission).
     */
    raw.c_iflag &= ~(ICRNL | IXON);
    /*
     * Flags turned _off_:
     *
     * OPOST - terminals interpret the ENTER keystroke as a combination of
     * Carriage Return/New Line automatically. This is know as output
     * processing. With this on '\n' is also CR/NL. With this off, we have to
     * manually issue an accompanying '\r'.
     */
    raw.c_oflag &= ~(OPOST);
    /*
     * Flags turned _off_:
     *
     * ECHO - causes keystrokes to be printed (echoed) to the terminal in the
     * normal way (i.e., like you would expect a typewriter to work).
     *
     * ICANON - reads input in the canonical mode (line-by-line). With this off
     * we can read input byte-by-byte.
     *
     * IEXTEN - in some terminals, Ctrl-C allows you to "materialize" the next
     * input; try Ctrl-C then [ENTER] in bash (or any other non-alphanumeric
     * key). This flag handles that.
     *
     * ISIG - reads input signals. With this off, the program can ignore Ctrl-C
     * (SIGINT) and Ctrl-Z (SIGSTOP).
     */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("control char: %d\n", c);
        } else {
            printf("normal char: %d ('%c')\n", c, c);
        }
    }
    return 0;
}
