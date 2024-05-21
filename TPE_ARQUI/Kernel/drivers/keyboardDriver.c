#include <keyboardDriver.h>

char buffer[BUFFER_DIM] = {0};
int previous = 0;
int current = 0;

static char keyboardMap[KEY_COUNT][2] = {
    {0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'},
    {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'},
    {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'},
    {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'},
    {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}, {0, 0}
};

static int shift = 0;
static int blockMayus = 0;
static int ctrlPressed = 0;

int specialKey(int scanCode) {
    return ( scanCode == CAPS_LOCK || scanCode == ESC || scanCode == CTRL|| scanCode == ALT  || scanCode == LEFT_SHIFT || scanCode == RIGHT_SHIFT);
}

void keyboardHandler(uint64_t *rsp) {
    int scan_code = getPressedKey();

    switch (scan_code) {
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            shift = 1;
            break;
        case LEFT_SHIFT + RELEASED_KEY:
        case RIGHT_SHIFT + RELEASED_KEY:
            shift = 0;
            break;
        case CAPS_LOCK:
            blockMayus = blockMayus ? 0 : 1;
            break;
        case CTRL:
            ctrlPressed = 1;
            break;
        case CTRL + RELEASED_KEY:
            ctrlPressed = 0;
            break;
        case ALT:
            break;
        default:
            if (scan_code <= MAX_PRESSED_KEY && !specialKey(scan_code)) {
                int secondChar = shift;
                if (IS_ALPHA(keyboardMap[scan_code][0])) {
                    secondChar = blockMayus ? 1 - shift : shift;
                }
                if (ctrlPressed) {
                    if (keyboardMap[scan_code][secondChar] == 'R' || keyboardMap[scan_code][secondChar] == 'r') {
                        saveRegistersState(rsp);
                    }
                    return;
                }
                buffer[current++] = keyboardMap[scan_code][secondChar];
                current %= BUFFER_DIM;
        }
    }
}

int read = 0;

void readKeyboard(char * toWrite, int length){
    for (int i = 0; i < length; i++) {
        toWrite[i] = buffer[read];
        buffer[read] = 0;
        read = (read + 1) % BUFFER_DIM;
    }
}