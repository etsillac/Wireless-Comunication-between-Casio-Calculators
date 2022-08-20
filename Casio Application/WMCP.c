#include "fxlib.h"
#include "string.h"
#include "stdlib.h"
#include "sh4Compability.h"
#include "syscall.h"

#define MAX_MESSAGE_LENGTH 18
#define MAX_GROUP_USERS 8
#define MESSAGE_EDIT_LENGTH 17
#define MESSAGES_PER_PAGE 6

#define DrawButton(column, inversed) drawPatern(2+21*column, 57, 19, 7, inversed ? filledActionButtonBg : actionButtonBg, 0)

typedef struct Message {
    char user;
    char message[MAX_MESSAGE_LENGTH + 1];
} Message;

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

int GetKeyDown(int time, unsigned int* key) {
    int i;
    int keyPressed = 0;
    int cooldown = time / 50;

    while (cooldown && !keyPressed) {
        for (i = 0; i < 80; i++) {
            if (IsKeyDown(i)) {
                while (IsKeyDown(i)) { Sleep(10); }
                *key = i;
                keyPressed = 1;
                break;
            }
        }
        cooldown--;
        Sleep(50);
    }

    return keyPressed;
}
const char* KeyCodeToChar(unsigned int keyCode, int keyState, int uppercase) {
    if (keyState == 0) {
        if (keyCode == KEY_CHAR_0) { return "0"; }
        if (keyCode == KEY_CHAR_1) { return "1"; }
        if (keyCode == KEY_CHAR_2) { return "2"; }
        if (keyCode == KEY_CHAR_3) { return "3"; }
        if (keyCode == KEY_CHAR_4) { return "4"; }
        if (keyCode == KEY_CHAR_5) { return "5"; }
        if (keyCode == KEY_CHAR_6) { return "6"; }
        if (keyCode == KEY_CHAR_7) { return "7"; }
        if (keyCode == KEY_CHAR_8) { return "8"; }
        if (keyCode == KEY_CHAR_9) { return "9"; }
        if (keyCode == KEY_CHAR_DP) { return "."; }
        if (keyCode == KEY_CHAR_EXP) { return "\x0F"; }
        if (keyCode == KEY_CHAR_PMINUS) { return "\x87"; }
        if (keyCode == KEY_CHAR_PLUS) { return "+"; }
        if (keyCode == KEY_CHAR_MINUS) { return "-"; }
        if (keyCode == KEY_CHAR_MULT) { return "\xA9"; }
        if (keyCode == KEY_CHAR_DIV) { return "\xB9"; }
        if (keyCode == KEY_CHAR_FRAC) { return "/"; }
        if (keyCode == KEY_CHAR_LPAR) { return "("; }
        if (keyCode == KEY_CHAR_RPAR) { return ")"; }
        if (keyCode == KEY_CHAR_COMMA) { return ","; }
        if (keyCode == KEY_CHAR_STORE) { return "\xE6\x91"; }
        if (keyCode == KEY_CTRL_XTT) { return "X"; }
        if (keyCode == KEY_CHAR_SQUARE) { return "\xE5\xC2"; }
        if (keyCode == KEY_CHAR_POW) { return "^"; }
    }
    else if (keyState == 1) {
        if (keyCode == KEY_CHAR_0) { return "\x7F\x50"; }
        if (keyCode == KEY_CHAR_EQUAL) { return "="; }
        if (keyCode == KEY_CHAR_EXP) { return "\xE6\x4F"; }
        if (keyCode == KEY_CHAR_PLUS) { return "["; }
        if (keyCode == KEY_CHAR_MINUS) { return "]"; }
        if (keyCode == KEY_CHAR_MULT) { return "{"; }
        if (keyCode == KEY_CHAR_DIV) { return "}"; }
        if (keyCode == KEY_CHAR_RPAR) { return "\xE5\xCA"; }
        if (keyCode == KEY_CHAR_ANGLE) { return "\x7F\x54"; }
        if (keyCode == KEY_CHAR_LN) { return "\xE6\x65"; }
        if (keyCode == KEY_CHAR_SQUARE) { return "\x86"; }
    }
    else {
        if (keyCode == KEY_CHAR_0) { if (uppercase) { return "Z"; } else { return "z"; } }
        if (keyCode == KEY_CHAR_1) { if (uppercase) { return "U"; } else { return "u"; } }
        if (keyCode == KEY_CHAR_2) { if (uppercase) { return "V"; } else { return "v"; } }
        if (keyCode == KEY_CHAR_3) { if (uppercase) { return "W"; } else { return "w"; } }
        if (keyCode == KEY_CHAR_4) { if (uppercase) { return "P"; } else { return "p"; } }
        if (keyCode == KEY_CHAR_5) { if (uppercase) { return "Q"; } else { return "q"; } }
        if (keyCode == KEY_CHAR_6) { if (uppercase) { return "R"; } else { return "r"; } }
        if (keyCode == KEY_CHAR_7) { if (uppercase) { return "M"; } else { return "m"; } }
        if (keyCode == KEY_CHAR_8) { if (uppercase) { return "N"; } else { return "n"; } }
        if (keyCode == KEY_CHAR_9) { if (uppercase) { return "O"; } else { return "o"; } }
        if (keyCode == KEY_CHAR_DP) { return " "; }
        if (keyCode == KEY_CHAR_EXP) { return "\""; }
        if (keyCode == KEY_CHAR_PLUS) { if (uppercase) { return "X"; } else { return "x"; } }
        if (keyCode == KEY_CHAR_MINUS) { if (uppercase) { return "Y"; } else { return "y"; } }
        if (keyCode == KEY_CHAR_MULT) { if (uppercase) { return "S"; } else { return "s"; } }
        if (keyCode == KEY_CHAR_DIV) { if (uppercase) { return "T"; } else { return "t"; } }
        if (keyCode == KEY_CHAR_FRAC) { if (uppercase) { return "G"; } else { return "g"; } }
        if (keyCode == KEY_CTRL_FD) { if (uppercase) { return "H"; } else { return "h"; } }
        if (keyCode == KEY_CHAR_LPAR) { if (uppercase) { return "I"; } else { return "i"; } }
        if (keyCode == KEY_CHAR_RPAR) { if (uppercase) { return "J"; } else { return "j"; } }
        if (keyCode == KEY_CHAR_COMMA) { if (uppercase) { return "K"; } else { return "k"; } }
        if (keyCode == KEY_CHAR_STORE) { if (uppercase) { return "L"; } else { return "l"; } }
        if (keyCode == KEY_CHAR_ANGLE) { if (uppercase) { return "A"; } else { return "a"; } }
        if (keyCode == KEY_CHAR_LOG) { if (uppercase) { return "B"; } else { return "b"; } }
        if (keyCode == KEY_CHAR_LN) { if (uppercase) { return "C"; } else { return "c"; } }
        if (keyCode == KEY_CHAR_SIN) { if (uppercase) { return "D"; } else { return "d"; } }
        if (keyCode == KEY_CHAR_COS) { if (uppercase) { return "E"; } else { return "e"; } }
        if (keyCode == KEY_CHAR_TAN) { if (uppercase) { return "F"; } else { return "f"; } }
        if (keyCode == KEY_CHAR_SQUARE) { return "\xCD"; }
        if (keyCode == KEY_CHAR_POW) { return "\xE6\x47"; }
    }

    return "";
}

void drawBox(int x1, int y1, int x2, int y2, int color) {
    int x, y;
    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            Bdisp_SetPoint_VRAM(x, y, color);
        }
    }
}
void drawTitleBar(const char* menuTitle, int smallBar) {
    int xStart;

    drawBox(0, 0, 127, smallBar ? 7 : 10, 1);

    xStart = 63 - strlen(menuTitle) * 3;
    PrintXY(xStart, smallBar ? 0 : 2, menuTitle, 1);
}
void drawPatern(int x, int y, int width, int height, const unsigned char* data, char inversed) {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            Bdisp_SetPoint_VRAM(x + i, y + j, inversed ? !data[j*width+i] : data[j*width+i]);
        }
    }
}

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key;
    int i;
    
    int cursor = 0;
    int displayCursor = 0;
    int cursorStart = 0;
    
    int editMode = 1;
    int keyState = 0;           // 0: default; 1: temp shift; 2: temp alpha; 3: perm alpha
    int uppercase = 1;

    char* buffer;
    char messageWriting[MESSAGES_PER_PAGE * MAX_MESSAGE_LENGTH + 1] = "Hello world, i'm Etsillac Gabriel";
    char currentEditMessage[MESSAGE_EDIT_LENGTH + 2];

    Message currentDisplayedMessages[MESSAGES_PER_PAGE];
    char userNames[MAX_GROUP_USERS][3] = { "U1", "U2", "U3", "U4", "U5", "U6", "U7", "U8" };

    for (i = 0; i < MESSAGES_PER_PAGE; i++) {
        currentDisplayedMessages[i].user = (char)i;
        strncpy(currentDisplayedMessages[i].message, "Nullam vitae orci,", 19);
    }

    while (1) {
        Bdisp_AllClr_VRAM();

        drawTitleBar("Group name", 1);

        for (i = 0; i < MESSAGES_PER_PAGE; i++) {
            PrintXY(0, 9+i*8, userNames[currentDisplayedMessages[i].user], 0);
            PrintXY(13, 9+i*8, ":", 0);
            PrintXY(19, 9+i*8, currentDisplayedMessages[i].message, 0);
        }

        if (editMode) {
            strncpy(currentEditMessage, messageWriting + cursorStart, MESSAGE_EDIT_LENGTH+1);
            currentEditMessage[MESSAGE_EDIT_LENGTH+1] = 0;      // add a final null terminator

            PrintXY(0, 57, currentEditMessage, 0);
            if (keyState == 0) { drawPatern(0 + displayCursor * 6, 57, 2, 7, defaultCursorIcon, 0); }
            else if (keyState == 1) { drawPatern(1 + displayCursor * 6, 57, 5, 7, shiftCursorIcon, 0); }
            else if (uppercase) { drawPatern(1 + displayCursor * 6, 57, 5, 7, uppercaseAlphaCursorIcon, 0); }
            else { drawPatern(1 + displayCursor * 6, 57, 5, 7, lowercaseAlphaCursorIcon, 0); }

            drawBox(106, 56, 127, 63, 0);
            DrawButton(5, 0);
            drawPatern(113, 59, 7, 5, proceedIcon, 0);
        } else {
            DrawButton(0, 1);
            drawPatern(9, 58, 5, 5, cancelIcon, 1);
        }

        Bdisp_PutDisp_DD();

        if (GetKeyDown(1000, &key)) {
            if (key == KEY_CTRL_MENU) { break; }
            else if (key == KEY_CTRL_F1 && !editMode) {  }      // cancel (decide whether it quits the group or it goes back to edit mode)
            else if (key == KEY_CTRL_F4 && !editMode) {  }      // go to bottom
            else if (key == KEY_CTRL_F5 && !editMode) {  }      // go to character selection menu
            else if (key == KEY_CTRL_F6) {
                if (editMode) {  }                              // send message
                else { uppercase = !uppercase; }
            }
            else if (key == KEY_CTRL_OPTN) { editMode = 0; }
            else if (key == KEY_CTRL_EXIT) {
                if (editMode) {  }                              // quit group
                else { editMode = 1; }
            }
            else if (key == KEY_CTRL_LEFT) {
                if ((displayCursor > 0 && cursorStart == 0) || displayCursor > 1) { displayCursor--; }
                else if (cursorStart > 0) { cursorStart--; }
            }
            else if (key == KEY_CTRL_RIGHT) {
                if ((displayCursor < MESSAGE_EDIT_LENGTH && cursorStart == strlen(messageWriting) - MESSAGE_EDIT_LENGTH) || displayCursor < MESSAGE_EDIT_LENGTH - 1) { displayCursor++; }
                else if (cursorStart < strlen(messageWriting) - MESSAGE_EDIT_LENGTH) { cursorStart++; }
            }

            if (key == KEY_CTRL_SHIFT) { if (keyState == 1) { keyState = 0; } else { keyState = 1; }}
            else if (key == KEY_CTRL_ALPHA) { if (keyState == 0) { keyState = 2; } else if (keyState == 1) { keyState = 3; } else { keyState = 0; } }
            else if (keyState != 3) { keyState = 0; }
        }
    }

    return 1;
}

//****************************************************************************
//**************                 Notice!                      ****************
//**************  Please do not change the following source.  ****************
//****************************************************************************

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section
#pragma section _TOP

int InitializeSystem(int isAppli, unsigned short OptionNum) {
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section
