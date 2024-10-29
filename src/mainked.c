#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios;

void cls() {
    // Clear screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // Position cursor to the top of screen
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void die(const char *s) {
    cls();
    perror(s);
    exit(1);
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        die("disableRawMode -> tcsetattr");
    }
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        die("enableRawMode -> tcgetattr");
    }
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    tcgetattr(STDIN_FILENO, &raw);
    /*
     * Flags turned _off_:
     *
     * BRKINT[1] - Break conditions cause SIGINT (aka Ctrl-C).
     *
     * ICRNL - allows Ctrl-M to be read as Carriage Return/New Line.
     *
     * INPCK[1] - parity checking.
     *
     * ISTRIP[1] - zeroes out the 8th bit of each byte (i.e., "strips" it).
     *
     * IXON - input transmission on. With this turned off, no data is
     * transmitted to the terminal at all. Ctrl-S triggers XOFF (pause
     * transmission) while Ctrl-Q triggers XON (resume transmission).
     *
     * [1] Traditional flags. Probably ignored by modern terminal emulators.
     */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
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
     * CS8 is a bit mask. (Are we turning it on?). It sets the character size
     * (CS) to 8 bits per byte. That is already mostly the default.
     */
    raw.c_cflag |= (CS8);
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

    /*
     * Set timeout for read().
     *
     * VMIN - minimum number of input bytes before read() can return.
     *
     * VTIME - maximum wait time before read() returns.
     */
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("enableRawMode -> tcsetattr");
    }
}

char editorReadKey() {
    int nread;
    char c = '\0';
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        // EAGAIN check mostly for Cygwin compatibility.
        if (nread == -1 && errno != EAGAIN) {
            die("editorReadKey -> read");
        }
    }

    return c;
}

void editorProcessKeyPress() {
    char c = editorReadKey();

    switch (c) {
        case CTRL_KEY('q'):
            cls();
            exit(0);
            break;
    }
}

/*
 * Assuming that the screen has been previously refreshed, draw the UI.
 */
void editorDrows() {
    int row;
    for (row = 0; row < 24; row++) {
        write(STDOUT_FILENO, "~\n\r", 3);
    }
}

void editorRefreshScreen() {
    cls();
    editorDrows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}

int main() {
    enableRawMode();

    while (1) {
        editorRefreshScreen();
        editorProcessKeyPress();
    }

    return 0;
}
