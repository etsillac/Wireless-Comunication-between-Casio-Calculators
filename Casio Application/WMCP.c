#include "fxlib.h"
#include "string.h"
#include "stdlib.h"
#include "sh4Compability.h"
#include "syscall.h"

const unsigned char filledActionButtonBg[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
const unsigned char actionButtonBg[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};
const unsigned char cancelIcon[] = {
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 1, 0,
    1, 0, 0, 0, 1
};
const unsigned char confirmIcon[] = {
    0, 0, 0, 0, 1,
    0, 0, 0, 1, 0,
    1, 0, 1, 0, 0,
    0, 1, 0, 0, 0
};
const unsigned char proceedIcon[] = {
    0, 0, 0, 0, 1, 0, 0,
    1, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 1, 0, 0
};
const unsigned char characterSelectionIcon[] = {
    0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
    0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1
};
const unsigned char uppercaseToLowercaseIcon[] = {
    0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1
};

unsigned int key;
unsigned int cooldown;
short size;

char readLength;
char* readBuffer;

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]) {
    int i, j, sign;
    char c;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void drawTitleBar(const char* menuTitle) {
    int x, xStart, y;

    for (x = 0; x < 128; x++) {
        for (y = 0; y < 11; y++) {
            Bdisp_SetPoint_VRAM(x, y, 1);
        }
    }

    xStart = 63 - strlen(menuTitle) * 3;
    PrintXY(xStart, 2, menuTitle, 1);
}
void drawPatern(int x, int y, int width, int height, const unsigned char* data, char reverse) {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            Bdisp_SetPoint_VRAM(x + i, y + j, reverse ? !data[j*width+i] : data[j*width+i]);
        }
    }
}

const char* handleKeyboard(unsigned int _key, int keyState, int uppercase) {
    if (keyState == 0) {
        if (_key == KEY_CHAR_0) { return "0"; }
        if (_key == KEY_CHAR_1) { return "1"; }
        if (_key == KEY_CHAR_2) { return "2"; }
        if (_key == KEY_CHAR_3) { return "3"; }
        if (_key == KEY_CHAR_4) { return "4"; }
        if (_key == KEY_CHAR_5) { return "5"; }
        if (_key == KEY_CHAR_6) { return "6"; }
        if (_key == KEY_CHAR_7) { return "7"; }
        if (_key == KEY_CHAR_8) { return "8"; }
        if (_key == KEY_CHAR_9) { return "9"; }
        if (_key == KEY_CHAR_DP) { return "."; }
        if (_key == KEY_CHAR_EXP) { return "\x0F"; }
        if (_key == KEY_CHAR_PMINUS) { return "\x87"; }
        if (_key == KEY_CHAR_PLUS) { return "+"; }
        if (_key == KEY_CHAR_MINUS) { return "-"; }
        if (_key == KEY_CHAR_MULT) { return "\xA9"; }
        if (_key == KEY_CHAR_DIV) { return "\xB9"; }
        if (_key == KEY_CHAR_FRAC) { return "/"; }
        if (_key == KEY_CHAR_LPAR) { return "("; }
        if (_key == KEY_CHAR_RPAR) { return ")"; }
        if (_key == KEY_CHAR_COMMA) { return ","; }
        if (_key == KEY_CHAR_STORE) { return "\xE6\x91"; }
        if (_key == KEY_CTRL_XTT) { return "X"; }
        if (_key == KEY_CHAR_SQUARE) { return "\xE5\xC2"; }
        if (_key == KEY_CHAR_POW) { return "^"; }
    } else if (keyState == 1) {
        if (_key == KEY_CHAR_0) { return "\x7F\x50"; }
        if (_key == KEY_CHAR_EQUAL) { return "="; }
        if (_key == KEY_CHAR_EXP) { return "\xE6\x4F"; }
        if (_key == KEY_CHAR_PLUS) { return "["; }
        if (_key == KEY_CHAR_MINUS) { return "]"; }
        if (_key == KEY_CHAR_MULT) { return "{"; }
        if (_key == KEY_CHAR_DIV) { return "}"; }
        if (_key == KEY_CHAR_RPAR) { return "\xE5\xCA"; }
        if (_key == KEY_CHAR_ANGLE) { return "\x7F\x54"; }
        if (_key == KEY_CHAR_LN) { return "\xE6\x65"; }
        if (_key == KEY_CHAR_SQUARE) { return "\x86"; }
    } else {
        if (_key == KEY_CHAR_0) { if (uppercase) { return "Z"; } else { return "z"; } }
        if (_key == KEY_CHAR_1) { if (uppercase) { return "U"; } else { return "u"; } }
        if (_key == KEY_CHAR_2) { if (uppercase) { return "V"; } else { return "v"; } }
        if (_key == KEY_CHAR_3) { if (uppercase) { return "W"; } else { return "w"; } }
        if (_key == KEY_CHAR_4) { if (uppercase) { return "P"; } else { return "p"; } }
        if (_key == KEY_CHAR_5) { if (uppercase) { return "Q"; } else { return "q"; } }
        if (_key == KEY_CHAR_6) { if (uppercase) { return "R"; } else { return "r"; } }
        if (_key == KEY_CHAR_7) { if (uppercase) { return "M"; } else { return "m"; } }
        if (_key == KEY_CHAR_8) { if (uppercase) { return "N"; } else { return "n"; } }
        if (_key == KEY_CHAR_9) { if (uppercase) { return "O"; } else { return "o"; } }
        if (_key == KEY_CHAR_DP) { return " "; }
        if (_key == KEY_CHAR_EXP) { return "\""; }
        if (_key == KEY_CHAR_PLUS) { if (uppercase) { return "X"; } else { return "x"; } }
        if (_key == KEY_CHAR_MINUS) { if (uppercase) { return "Y"; } else { return "y"; } }
        if (_key == KEY_CHAR_MULT) { if (uppercase) { return "S"; } else { return "s"; } }
        if (_key == KEY_CHAR_DIV) { if (uppercase) { return "T"; } else { return "t"; } }
        if (_key == KEY_CHAR_FRAC) { if (uppercase) { return "G"; } else { return "g"; } }
        if (_key == KEY_CTRL_FD) { if (uppercase) { return "H"; } else { return "h"; } }
        if (_key == KEY_CHAR_LPAR) { if (uppercase) { return "I"; } else { return "i"; } }
        if (_key == KEY_CHAR_RPAR) { if (uppercase) { return "J"; } else { return "j"; } }
        if (_key == KEY_CHAR_COMMA) { if (uppercase) { return "K"; } else { return "k"; } }
        if (_key == KEY_CHAR_STORE) { if (uppercase) { return "L"; } else { return "l"; } }
        if (_key == KEY_CHAR_ANGLE) { if (uppercase) { return "A"; } else { return "a"; } }
        if (_key == KEY_CHAR_LOG) { if (uppercase) { return "B"; } else { return "b"; } }
        if (_key == KEY_CHAR_LN) { if (uppercase) { return "C"; } else { return "c"; } }
        if (_key == KEY_CHAR_SIN) { if (uppercase) { return "D"; } else { return "d"; } }
        if (_key == KEY_CHAR_COS) { if (uppercase) { return "E"; } else { return "e"; } }
        if (_key == KEY_CHAR_TAN) { if (uppercase) { return "F"; } else { return "f"; } }
        if (_key == KEY_CHAR_SQUARE) { return "\xCD"; }
        if (_key == KEY_CHAR_POW) { return "\xE6\x47"; }
    }

    return "";
}
const char* handleCharacterSelectionMenu() {
    const unsigned char submenu1[] = {
        0, 1, 0,
        1, 1, 0,
        0, 1, 0,
        0, 1, 0,
        1, 1, 1
    };
    const unsigned char submenu2[] = {
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
        1, 0, 0,
        1, 1, 1
    };
    const unsigned char submenu3[] = {
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1
    };
    const unsigned char submenu4[] = {
        1, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 0, 1,
        0, 0, 1
    };

    const char* characters[4][120] = {
        {
            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
            "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
            "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7",
            "8", "9", " ", ",", ".", ";", ":", "!", "?", "\"", "\'", "#", "&", "$", "*", "/", "\xD8", "\xD8", "\xD8", "\xD8",
            "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8",
            "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8", "\xD8",
        }
    };

    int indexX = 0;
    int indexY = 0;
    int page = 0;

    int x, y;

    while (1) {
        Bdisp_AllClr_VRAM();

        PrintXY(6, 0, "Character selection", 0);

        for (x = 0; x < 20; x++) { for (y = 0; y < 6; y++) { PrintXY(3+x*6, 8+y*8, characters[page][x+y*20], (indexX == x && indexY == y) ? 1 : 0); } }

        drawPatern(2, 57, 19, 7, filledActionButtonBg, 0);
        drawPatern(9, 58, 5, 5, cancelIcon, 1);
        drawPatern(44, (page == 0) ? 62 : 57, 19, 7, actionButtonBg, 0);
        if (page != 0) { drawPatern(52, 59, 3, 5, submenu1, 0); }
        drawPatern(65, (page == 1) ? 62 : 57, 19, 7, actionButtonBg, 0);
        if (page != 1) { drawPatern(73, 59, 3, 5, submenu2, 0); }
        drawPatern(86, (page == 2) ? 62 : 57, 19, 7, actionButtonBg, 0);
        if (page != 2) { drawPatern(94, 59, 3, 5, submenu3, 0); }
        drawPatern(107, (page == 3) ? 62 : 57, 19, 7, actionButtonBg, 0);
        if (page != 3) { drawPatern(115, 59, 3, 5, submenu4, 0); }

        Bdisp_PutDisp_DD();

        GetKey(&key);
        if (key == KEY_CTRL_EXIT || key == KEY_CTRL_F1) { return ""; }
        else if (key == KEY_CTRL_EXE) { return characters[page][indexX+indexY*20]; }
        else if (key == KEY_CTRL_F3) { page = 0; }
        else if (key == KEY_CTRL_F4) { page = 1; }
        else if (key == KEY_CTRL_F5) { page = 2; }
        else if (key == KEY_CTRL_F6) { page = 3; }
        else if (key == KEY_CTRL_LEFT) { if (indexX > 0) { indexX--; } else { indexX = 19; } }
        else if (key == KEY_CTRL_RIGHT) { if (indexX < 19) { indexX++; } else { indexX = 0; } }
        else if (key == KEY_CTRL_UP) { if (indexY > 0) { indexY--; } else { indexY = 5; } }
        else if (key == KEY_CTRL_DOWN) { if (indexY < 5) { indexY++; } else { indexY = 0; } }
    }
}

int editStringMenu(const char* menuName, char* string, const unsigned int maxChar) {
    const unsigned char defaultCursorIcon[] = {
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1,
        1, 1
    };
    const unsigned char shiftCursorIcon[] = {
        0, 0, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 0, 0,
        1, 0, 0, 0, 1,
        1, 1, 0, 1, 1
    };
    const unsigned char uppercaseAlphaCursorIcon[] = {
        0, 0, 1, 0, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        1, 0, 0, 0, 1,
        1, 1, 0, 1, 1
    };
    const unsigned char lowercaseAlphaCursorIcon[] = {
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 0, 1, 1, 0,
        1, 0, 0, 0, 1,
        1, 1, 0, 1, 1
    };

    int keyState = 0;           // 0: default; 1: temp shift; 2: temp alpha; 3: perm alpha
    int uppercase = 1;

    int i, j;

    unsigned int cursor = 0;
    unsigned int contentCursor = 0;
    int xStart, width;

    char maxCharItoaBuffer[3];

    char* keyBuffer;
    char** stringEditBuffer;
    short* stringContent;
    int newCharLength;

    stringEditBuffer = malloc(2 * maxChar);
    stringContent = malloc(maxChar * sizeof(short));
    for (i = 0, j = 0; i < maxChar; i++, j++) {
        if (string[j] == 0x7F || string[j] == 0xE5 || string[j] == 0xE6) { stringContent[i] = 2; j++; }
        else if (j < strlen(string)) { stringContent[i] = 1; }
        else { stringContent[i] = 0; }
    }

    while (1) {
        Bdisp_AllClr_VRAM();

        drawTitleBar(menuName);
        
        width = maxChar * 6 + 5;
        xStart = 63 - (int)(width / 2);
        Bdisp_DrawLineVRAM(xStart + 1, 23, xStart + width - 1, 23);
        Bdisp_DrawLineVRAM(xStart + 1, 35, xStart + width - 1, 35);
        Bdisp_DrawLineVRAM(xStart, 24, xStart, 34);
        Bdisp_DrawLineVRAM(xStart + width, 24, xStart + width, 34);

        PrintXY(xStart + 2, 26, string, 0);
        if (keyState == 0) { drawPatern(xStart + 2 + cursor * 6, 26, 2, 7, defaultCursorIcon, 0); }
        else if (keyState == 1) { drawPatern(xStart + 3 + cursor * 6, 26, 5, 7, shiftCursorIcon, 0); }
        else if (uppercase) { drawPatern(xStart + 3 + cursor * 6, 26, 5, 7, uppercaseAlphaCursorIcon, 0); }
        else { drawPatern(xStart + 3 + cursor * 6, 26, 5, 7, lowercaseAlphaCursorIcon, 0); }

        itoa(maxChar, maxCharItoaBuffer);
        PrintMini(41, 37, "max    char", 0);
        PrintMini(58, 37, (const unsigned char*)maxCharItoaBuffer, 0);

        drawPatern(2, 57, 19, 7, filledActionButtonBg, 0);
        drawPatern(9, 58, 5, 5, cancelIcon, 1);
        drawPatern(44, 57, 19, 7, filledActionButtonBg, 0);
        drawPatern(46, 58, 15, 5, characterSelectionIcon, 1);
        drawPatern(65, 57, 19, 7, actionButtonBg, 0);
        drawPatern(67, 59, 15, 5, uppercaseToLowercaseIcon, 0);
        drawPatern(107, 57, 19, 7, filledActionButtonBg, 0);
        drawPatern(114, 59, 5, 4, confirmIcon, 1);

        Bdisp_PutDisp_DD();

        GetKey(&key);
        if (key == KEY_CTRL_EXIT || key == KEY_CTRL_F1) { return 0; }
        else if (key == KEY_CTRL_EXE || key == KEY_CTRL_F6) { return 1; }
        else if (key == KEY_CTRL_F3) { handleCharacterSelectionMenu(); }
        else if (key == KEY_CTRL_F4) { uppercase = !uppercase; }
        else if (key == KEY_CTRL_LEFT) { if (contentCursor > 0) { contentCursor--; cursor -= stringContent[contentCursor]; } }
        else if (key == KEY_CTRL_RIGHT) { if (contentCursor < maxChar && stringContent[contentCursor]) { cursor += stringContent[contentCursor]; contentCursor++; } }
        else if (key == KEY_CTRL_DEL) {
            if (contentCursor > 0) {
                contentCursor--;
                cursor -= stringContent[contentCursor];

                memmove(&string[cursor], &string[cursor + stringContent[contentCursor]], strlen(string) - cursor);

                for (i = contentCursor; i < maxChar - 1; i++) { stringContent[i] = stringContent[i+1]; }
                stringContent[maxChar - 1] = 0;
            }
        }
        else if (cursor < maxChar) {
            keyBuffer = handleKeyboard(key, keyState, uppercase);
            
        }

        if (key == KEY_CTRL_SHIFT) { if (keyState == 1) { keyState = 0; } else { keyState = 1; }}
        else if (key == KEY_CTRL_ALPHA) { if (keyState == 0) { keyState = 2; } else if (keyState == 1) { keyState = 3; } else { keyState = 0; } }
        else if (keyState != 3) { keyState = 0; }
    }
}

int handleMainMenu() {
    const unsigned char selectedGroupBg[] = {
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
    };
    const unsigned char noGroupFoundIcon[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
        1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
        1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1,
        1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    const unsigned char settingsIcon[] = {
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        1, 1, 0, 1, 1,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0
    };
    const unsigned char refreshIcon[] = {
        1, 1, 1, 0, 0,
        0, 1, 1, 0, 0,
        1, 0, 1, 0, 1,
        1, 0, 0, 0, 1,
        0, 1, 1, 1, 0
    };
    const unsigned char editIcon[] = {
        0, 0, 0, 1, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0,
        0, 1, 1, 1, 0, 0, 0,
        1, 0, 1, 0, 0, 0, 0,
        1, 1, 0, 1, 1, 1, 1
    };
    const unsigned char upArrow[] = {
        0, 0, 1, 1, 0, 0,
        0, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1
    };
    const unsigned char downArrow[] = {
        1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 0, 0
    };
    const unsigned char miniUpArrow[] = {
        0, 1, 0,
        1, 1, 1
    };
    const unsigned char miniDownArrow[] = {
        1, 1, 1,
        0, 1, 0
    };

    int i, j, k;
    int mode = 0;
    int hasGroupNameChanged = 0;
    
    char numOfGroups = 0;
    int selectedGroup = 0;
    int onPageCursor = 0;
    int currentPage = -1;

    char currentGroupNames[4][21] = {
        "Error #404",
        "Error #404",
        "Error #404",
        "Error #404"
    };

    while (1) {
        Serial_WriteByte(1);
        Serial_WriteByte(0x20);

        cooldown = 20;
        while (cooldown && Serial_ReadByte(&readLength) == 1) { cooldown--; Sleep(10); }
        while (cooldown && Serial_GetRxBufferSize() < readLength) { cooldown--; Sleep(10); }
        if (cooldown) {
            Serial_ReadBytes(readBuffer, readLength, &size);
            if (size == 2 && readBuffer[0] == 0x21) { numOfGroups = readBuffer[1]; }
        }

        if (numOfGroups == 0 || cooldown == 0) { mode = 0; selectedGroup = 0; }
        else if (numOfGroups <= 4 ) { mode = 1; currentPage = 0; }
        else { mode = 2; }

        if (currentPage != (int)(selectedGroup / 4)) {
            currentPage = (int)(selectedGroup / 4);
            for (i = 0; i < 4; i++) {
                if (i+currentPage*4 < numOfGroups) {
                    Serial_WriteByte(2);
                    Serial_WriteByte(0x22);
                    Serial_WriteByte(i+currentPage*4);

                    cooldown = 50;
                    while (cooldown && Serial_ReadByte(&readLength) == 1) { cooldown--; Sleep(10); }
                    if (readLength < 22) { i--; continue; }
                    while (cooldown && Serial_GetRxBufferSize() < readLength) { cooldown--; Sleep(10); }
                    if (cooldown) {
                        Serial_ReadBytes(readBuffer, readLength, &size);
                        if (readBuffer[0] == 0x23) { strncpy(currentGroupNames[i], readBuffer+1, 11); }
                    }
                }
            }
        }

        Bdisp_AllClr_VRAM();

        drawTitleBar("Messages");
        drawPatern(2, 57, 19, 7, filledActionButtonBg, 0);
        drawPatern(9, 58, 5, 5, cancelIcon, 1);
        if (mode) {
            if (mode == 2) {
                if (currentPage > 0) { drawPatern(61, 12, 6, 3, upArrow, 0); }
                if (currentPage < (int)((numOfGroups-1) / 4)) { drawPatern(61, 53, 6, 3, downArrow, 0); }
            }
            onPageCursor = selectedGroup % 4;
            Bdisp_SetPoint_VRAM(30, 20 + onPageCursor * 9, 1);
            drawPatern(33, 16 + onPageCursor * 9, 63, 9, selectedGroupBg, 0);

            if (selectedGroup > 0) { drawPatern(29, 17 + onPageCursor * 9, 3, 2, miniUpArrow, 0); }
            if (selectedGroup < (numOfGroups-1)) { drawPatern(29, 22 + onPageCursor * 9, 3, 2, miniDownArrow, 0); }

            for (i = 0; (i < numOfGroups-currentPage*4) && (i < 4); i++) {
                PrintXY(34, 17 + i * 9, currentGroupNames[i], (onPageCursor == i) ? 1 : 0);
            }

            drawPatern(44, 57, 19, 7, filledActionButtonBg, 0);
            drawPatern(51, 58, 5, 5, settingsIcon, 1);
            drawPatern(65, 57, 19, 7, actionButtonBg, 0);
            drawPatern(72, 59, 5, 5, refreshIcon, 0);
            drawPatern(86, 57, 19, 7, filledActionButtonBg, 0);
            drawPatern(92, 58, 7, 5, editIcon, 1);
            drawPatern(107, 57, 19, 7, filledActionButtonBg, 0);
            drawPatern(113, 58, 7, 5, proceedIcon, 1);
        } else {
            drawPatern(59, 21, 11, 14, noGroupFoundIcon, 0);
            PrintXY(21, 36, "No group found", 0);
            drawPatern(86, 57, 19, 7, filledActionButtonBg, 0);
            drawPatern(93, 58, 5, 5, settingsIcon, 1);
            drawPatern(107, 57, 19, 7, actionButtonBg, 0);
            drawPatern(114, 59, 5, 5, refreshIcon, 0);
        }

        Bdisp_PutDisp_DD();

        GetKey(&key);
        if (key == KEY_CTRL_F1 || key == KEY_CTRL_EXIT) { return -1; }
        else if (key == KEY_CTRL_UP && selectedGroup > 0) { selectedGroup--; }
        else if (key == KEY_CTRL_DOWN && selectedGroup < (numOfGroups-1)) { selectedGroup++; }
        else if ((key == KEY_CTRL_F6 || key == KEY_CTRL_EXE) && mode > 0) { return selectedGroup; }
        else if ((key == KEY_CTRL_F5 && mode == 0) || (key == KEY_CTRL_F3 && mode > 0)) {}    // go to settings
        else if ((key == KEY_CTRL_F6 && mode == 0) || (key == KEY_CTRL_F4 && mode > 0)) { currentPage = -1; }
        else if (key == KEY_CTRL_F5 && mode > 0) {
            hasGroupNameChanged = editStringMenu("Edit group name", currentGroupNames[onPageCursor], 10);
            if (hasGroupNameChanged) {
                Serial_WriteByte(13);
                Serial_WriteByte(0x24);
                Serial_WriteByte(selectedGroup);
                Serial_WriteBytes(currentGroupNames[onPageCursor], 11);
            } else { currentPage = -1; }
        }
    }
}

int AddIn_main(int isAppli, unsigned short OptionNum) {
    int selectedGroup;
    char* buffer;

    unsigned char conf[] = {0,5,0,0,0,0};

    Serial_Open(conf);

    initialization:
    Bdisp_AllClr_VRAM();
    PrintXY(12, 1,  "Wireless  Message", 0);
    PrintXY(0, 9,  "Communication Program", 0);
    Bdisp_DrawLineVRAM(0, 20, 127, 20);
    Bdisp_DrawLineVRAM(0, 52, 127, 52);
    SaveDisp(SAVEDISP_PAGE1);

    PrintXY(14, 33, "Initialization...", 0);
    PrintMini(33,56, "[EXIT] to cancel", 0);
    Bdisp_PutDisp_DD();

    cooldown = 500;
    while (cooldown && Serial_ReadByte(&readLength) == 1) {
        if (IsKeyDown(KEY_CTRL_EXIT)) { return; }
        cooldown--;
        Serial_WriteByte(1);
        Serial_WriteByte(0x10);
        Sleep(10);
    }
    if (cooldown) { Serial_ReadBytes(readBuffer, readLength, &size); }

    if (readBuffer[0] != 0x11) {
        while (1) {
            Bdisp_AllClr_VRAM();
            RestoreDisp(SAVEDISP_PAGE1);
            if (cooldown == 0) { PrintXY(30, 33, "No response", 0); }
            else {
                PrintXY(15, 33, "Error code :", 0);
                itoa(readBuffer[0], buffer);
                PrintXY(93, 33, (const unsigned char*)buffer, 0);
            }
            PrintMini(3,56, "[F1] retry", 0);
            PrintMini(74,56, "[EXIT] cancel", 0);
            Bdisp_PutDisp_DD();
            GetKey(&key);
            if (key == KEY_CTRL_EXIT) { break; }
            if (key == KEY_CTRL_F1) { goto initialization; }
        }
    }
    else {
        selectedGroup = handleMainMenu();
        if (selectedGroup >= 0) {
            while (1) {
                Bdisp_AllClr_VRAM();
                itoa(selectedGroup, buffer);
                PrintXY(0, 0, buffer, 0);
                Bdisp_PutDisp_DD();
                GetKey(&key);
                if (key == KEY_CTRL_EXIT) { break; }
            }
        }
    }

    Serial_Close(0);

    return;
}

//****************************************************************************
//**************                 Notice!                      ****************
//**************  Please do not change the following source.  ****************
//****************************************************************************

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section
#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum) { return INIT_ADDIN_APPLICATION(isAppli, OptionNum); }
#pragma section
