#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

/*
 * Used to compensate for ioctl failures.
 *
 * 999 argument for C (cursor forward) and B (cursor down) are just 
 * arbitrarily large values.
 */
#define CURSOR_MAX_MOVE "\x1b[999C\x1b[999B"

/*
 * `n` command (Device Status Report) returns status information. The argument
 * 6 corresponds ot a query for cursor position. Reply can be read from standard
 * input.
 *
 * Reply is as follows:
 *
 * 27       - The escape character
 * 91 ('[') - sentinel token
 * 53 ('5') -==============================================
 * 53 ('5') -
 * 59 (';') -
 * 50 ('2') - Actual response (Cursor Position Report)
 * 49 ('1') -
 * 49 ('1') -
 * 82 ('R') -==============================================
 */
#define QUERY_CURSOR_POSITION "\x1b[6n"

typedef struct editorConfig {
    int rows;
    int cols;
    struct termios orig_termios;
} inkedConfig;

inkedConfig conf;

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

int getCursorPosition(int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, QUERY_CURSOR_POSITION, 4) != 4) {
        return -1;
    }

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) {
            break;
        }
        if (buf[i] == 'R') {
            break;
        }
        i++;
    }
    buf[i] = '\0';
    
    // Make sure it responded with an escape sequence.
    if (buf[0] != '\x1b' || buf[1] != '[') {
        return -1;
    }

    // Parse the results into the buffer
    if (sscanf(&buf[2], "%d:%d", rows, cols) != 2) {
        return -1;
    }

    return 0;
}

int getTerminalSize(int *rows, int *cols) {
    struct winsize ws;
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0 || ws.ws_row == 0) {
        if (write(STDOUT_FILENO, CURSOR_MAX_MOVE, 12) != 12) {
            return -1;
        }
        return getCursorPosition(rows, cols);
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void init() {
    if (getTerminalSize(&conf.rows, &conf.cols) == -1) {
        die("init -> getTerminalSize");
    }
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conf.orig_termios) == -1) {
        die("disableRawMode -> tcsetattr");
    }
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &conf.orig_termios) == -1) {
        die("enableRawMode -> tcgetattr");
    }
    atexit(disableRawMode);
    struct termios raw = conf.orig_termios;
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
    for (row = 0; row < conf.rows; row++) {
        write(STDOUT_FILENO, "~", 3);

        if (row < conf.rows - 1) {
            // Amazing: try printing newline before carriage return. Hmm...
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

void editorRefreshScreen() {
    cls();
    editorDrows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}

int main() {
    enableRawMode();
    init();

    while (1) {
        editorRefreshScreen();
        editorProcessKeyPress();
    }

    return 0;
}
