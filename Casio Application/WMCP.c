#include "fxlib.h"
#include "string.h"
#include "stdlib.h"
#include "sh4Compability.h"
#include "syscall.h"

#define max(a, b) (a > b) ? (a) : (b)

#define MAX_GROUP_NAME_LENGTH 10
#define GROUP_NAME_PER_PAGE 4
#define MAX_GROUP_NAME 40

#define MAX_LOADED_MESSAGE 60
#define MAX_MESSAGE_LENGTH 18
#define MAX_GROUP_USERS 8
#define MESSAGE_EDIT_LENGTH 17
#define MESSAGES_PER_PAGE 6

typedef struct Message {
    int userIndex;
    char message[MAX_MESSAGE_LENGTH + 1];
} Message;

typedef struct DataPacket {
    char header;
    unsigned char byteCount;
    unsigned char data[128];
    unsigned char checksum;
} DataPacket;

const unsigned int GETKEY_SLEEP_TIME = 50;
const char DATA_PACKET_HEADER = 0xF0;

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
const unsigned char quitIcon[] = {
    1, 1, 1, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 0, 0, 1, 1, 0, 0,
    1, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0,
    1, 1, 1, 1, 0, 0, 1, 0, 0
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
const unsigned char navigateToBottomIcon[] = {
    1, 0, 0, 0, 1,
    0, 1, 0, 1, 0,
    0, 0, 1, 0, 0
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

unsigned int checkSum(unsigned char* data) {
    int i;
    unsigned int checksumResult, sumResult;

    for (i = 0; i < 128; i++) {
        sumResult = checksumResult + data[i];
        checksumResult = ((unsigned char*)(&sumResult))[0] + ((unsigned char*)(&sumResult))[1];
    }
}
int checkDataPacketValid(DataPacket* dataPacket) {
    if (!dataPacket) return 1;
    if (dataPacket->header != DATA_PACKET_HEADER) return 2;
    if (dataPacket->byteCount != strlen(dataPacket->data)) return 3;
    if (dataPacket->checksum != checkSum(dataPacket->data)) return 4;
    return 0;
}

DataPacket makeDataPacket(char* data) {
    int i;

    DataPacket dataPacket;
    dataPacket.header = DATA_PACKET_HEADER;
    dataPacket.byteCount = strlen(data);
    for (i = 0; i < 128; i++) {
        if (i < strlen(data)) {
            dataPacket.data[i] = data[i];
        } else {
            dataPacket.data[i] = 0;
        }
    }
    dataPacket.checksum = checkSum(dataPacket.data);

    return dataPacket;
}
int recieveDataPacket(char* dest, int cooldown, int interruptible) {
    int error = 0, sizeRead;
    DataPacket dataPacket;

    while (cooldown && Serial_GetRxBufferSize() < sizeof(DataPacket)) {
        if (IsKeyDown(KEY_CTRL_EXIT) && interruptible) { return -1; }
        cooldown--;
        Sleep(10);
    }

    if (cooldown == 0) error = 5;
    else {
        Serial_ReadBytes(&dataPacket, sizeof(DataPacket), &sizeRead);

        error = checkDataPacketValid(&dataPacket);
        if (error == 0) strncpy(dest, dataPacket.data, dataPacket.byteCount+1);
    }
    
    return error;
}
int sendDataPacket(char* data) {
    int error = 0;
    DataPacket dataPacket = makeDataPacket(data);

    if (checkDataPacketValid(&dataPacket) == 0) error = Serial_WriteBytes(&dataPacket, sizeof(DataPacket));
    else error = 1;

    return error;
}

int GetKeyDown(int time, unsigned int* key) {
    int i;
    int keyPressed = 0;
    int cooldown = time / GETKEY_SLEEP_TIME;

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
        Sleep(GETKEY_SLEEP_TIME);
    }

    return keyPressed;
}
int KeyCodeToChar(char* dest, unsigned int keyCode, int keyState, int uppercase) {
    if (keyState == 0) {
        if (keyCode == KEY_CHAR_0) { strncpy(dest, "0", 3); return 1; }
        if (keyCode == KEY_CHAR_1) { strncpy(dest, "1", 3); return 1; }
        if (keyCode == KEY_CHAR_2) { strncpy(dest, "2", 3); return 1; }
        if (keyCode == KEY_CHAR_3) { strncpy(dest, "3", 3); return 1; }
        if (keyCode == KEY_CHAR_4) { strncpy(dest, "4", 3); return 1; }
        if (keyCode == KEY_CHAR_5) { strncpy(dest, "5", 3); return 1; }
        if (keyCode == KEY_CHAR_6) { strncpy(dest, "6", 3); return 1; }
        if (keyCode == KEY_CHAR_7) { strncpy(dest, "7", 3); return 1; }
        if (keyCode == KEY_CHAR_8) { strncpy(dest, "8", 3); return 1; }
        if (keyCode == KEY_CHAR_9) { strncpy(dest, "9", 3); return 1; }
        if (keyCode == KEY_CHAR_DP) { strncpy(dest, ".", 3); return 1; }
        if (keyCode == KEY_CHAR_EXP) { strncpy(dest, "\x0F", 3); return 1; }
        if (keyCode == KEY_CHAR_PMINUS) { strncpy(dest, "\x87", 3); return 1; }
        if (keyCode == KEY_CHAR_PLUS) { strncpy(dest, "+", 3); return 1; }
        if (keyCode == KEY_CHAR_MINUS) { strncpy(dest, "-", 3); return 1; }
        if (keyCode == KEY_CHAR_MULT) { strncpy(dest, "\xA9", 3); return 1; }
        if (keyCode == KEY_CHAR_DIV) { strncpy(dest, "\xB9", 3); return 1; }
        if (keyCode == KEY_CHAR_FRAC) { strncpy(dest, "/", 3); return 1; }
        if (keyCode == KEY_CHAR_LPAR) { strncpy(dest, "(", 3); return 1; }
        if (keyCode == KEY_CHAR_RPAR) { strncpy(dest, ")", 3); return 1; }
        if (keyCode == KEY_CHAR_COMMA) { strncpy(dest, ",", 3); return 1; }
        //if (keyCode == KEY_CHAR_STORE) { return "\xE6\x91"; }
        if (keyCode == KEY_CHAR_ANGLE) { strncpy(dest, "X", 3); return 1; }
        //if (keyCode == KEY_CHAR_SQUARE) { return "\xE5\xC2"; }
        if (keyCode == KEY_CHAR_POW) { strncpy(dest, "^", 3); return 1; }
    }
    else if (keyState == 1) {
        //if (keyCode == KEY_CHAR_0) { return "\x7F\x50"; }
        if (keyCode == KEY_CHAR_EQUAL) { strncpy(dest, "=", 3); return 1; }
        //if (keyCode == KEY_CHAR_EXP) { return "\xE6\x4F"; }
        if (keyCode == KEY_CHAR_PLUS) { strncpy(dest, "[", 3); return 1; }
        if (keyCode == KEY_CHAR_MINUS) { strncpy(dest, "]", 3); return 1; }
        if (keyCode == KEY_CHAR_MULT) { strncpy(dest, "{", 3); return 1; }
        if (keyCode == KEY_CHAR_DIV) { strncpy(dest, "}", 3); return 1; }
        //if (keyCode == KEY_CHAR_RPAR) { return "\xE5\xCA"; }
        //if (keyCode == KEY_CHAR_ANGLE) { return "\x7F\x54"; }
        //if (keyCode == KEY_CHAR_LN) { return "\xE6\x65"; }
        if (keyCode == KEY_CHAR_SQUARE) { strncpy(dest, "\x86", 3); return 1; }
    }
    else {
        if (keyCode == KEY_CHAR_0) { if (uppercase) { strncpy(dest, "Z", 3); return 1; } else { strncpy(dest, "z", 3); return 1; } }
        if (keyCode == KEY_CHAR_1) { if (uppercase) { strncpy(dest, "U", 3); return 1; } else { strncpy(dest, "u", 3); return 1; } }
        if (keyCode == KEY_CHAR_2) { if (uppercase) { strncpy(dest, "V", 3); return 1; } else { strncpy(dest, "v", 3); return 1; } }
        if (keyCode == KEY_CHAR_3) { if (uppercase) { strncpy(dest, "W", 3); return 1; } else { strncpy(dest, "w", 3); return 1; } }
        if (keyCode == KEY_CHAR_4) { if (uppercase) { strncpy(dest, "P", 3); return 1; } else { strncpy(dest, "p", 3); return 1; } }
        if (keyCode == KEY_CHAR_5) { if (uppercase) { strncpy(dest, "Q", 3); return 1; } else { strncpy(dest, "q", 3); return 1; } }
        if (keyCode == KEY_CHAR_6) { if (uppercase) { strncpy(dest, "R", 3); return 1; } else { strncpy(dest, "r", 3); return 1; } }
        if (keyCode == KEY_CHAR_7) { if (uppercase) { strncpy(dest, "M", 3); return 1; } else { strncpy(dest, "m", 3); return 1; } }
        if (keyCode == KEY_CHAR_8) { if (uppercase) { strncpy(dest, "N", 3); return 1; } else { strncpy(dest, "n", 3); return 1; } }
        if (keyCode == KEY_CHAR_9) { if (uppercase) { strncpy(dest, "O", 3); return 1; } else { strncpy(dest, "o", 3); return 1; } }
        if (keyCode == KEY_CHAR_DP) { strncpy(dest, " ", 3); return 1; }
        if (keyCode == KEY_CHAR_EXP) { strncpy(dest, "\"", 3); return 1; }
        if (keyCode == KEY_CHAR_PLUS) { if (uppercase) { strncpy(dest, "X", 3); return 1; } else { strncpy(dest, "x", 3); return 1; } }
        if (keyCode == KEY_CHAR_MINUS) { if (uppercase) { strncpy(dest, "Y", 3); return 1; } else { strncpy(dest, "y", 3); return 1; } }
        if (keyCode == KEY_CHAR_MULT) { if (uppercase) { strncpy(dest, "S", 3); return 1; } else { strncpy(dest, "s", 3); return 1; } }
        if (keyCode == KEY_CHAR_DIV) { if (uppercase) { strncpy(dest, "T", 3); return 1; } else { strncpy(dest, "t", 3); return 1; } }
        if (keyCode == KEY_CHAR_FRAC) { if (uppercase) { strncpy(dest, "G", 3); return 1; } else { strncpy(dest, "g", 3); return 1; } }
        if (keyCode == KEY_CTRL_FD) { if (uppercase) { strncpy(dest, "H", 3); return 1; } else { strncpy(dest, "h", 3); return 1; } }
        if (keyCode == KEY_CHAR_LPAR) { if (uppercase) { strncpy(dest, "I", 3); return 1; } else { strncpy(dest, "i", 3); return 1; } }
        if (keyCode == KEY_CHAR_RPAR) { if (uppercase) { strncpy(dest, "J", 3); return 1; } else { strncpy(dest, "j", 3); return 1; } }
        if (keyCode == KEY_CHAR_COMMA) { if (uppercase) { strncpy(dest, "K", 3); return 1; } else { strncpy(dest, "k", 3); return 1; } }
        if (keyCode == KEY_CHAR_STORE) { if (uppercase) { strncpy(dest, "L", 3); return 1; } else { strncpy(dest, "l", 3); return 1; } }
        if (keyCode == KEY_CHAR_ANGLE) { if (uppercase) { strncpy(dest, "A", 3); return 1; } else { strncpy(dest, "a", 3); return 1; } }
        if (keyCode == KEY_CHAR_LOG) { if (uppercase) { strncpy(dest, "B", 3); return 1; } else { strncpy(dest, "b", 3); return 1; } }
        if (keyCode == KEY_CHAR_LN) { if (uppercase) { strncpy(dest, "C", 3); return 1; } else { strncpy(dest, "c", 3); return 1; } }
        if (keyCode == KEY_CHAR_SIN) { if (uppercase) { strncpy(dest, "D", 3); return 1; } else { strncpy(dest, "d", 3); return 1; } }
        if (keyCode == KEY_CHAR_COS) { if (uppercase) { strncpy(dest, "E", 3); return 1; } else { strncpy(dest, "e", 3); return 1; } }
        if (keyCode == KEY_CHAR_TAN) { if (uppercase) { strncpy(dest, "F", 3); return 1; } else { strncpy(dest, "f", 3); return 1; } }
        if (keyCode == KEY_CHAR_SQUARE) { strncpy(dest, "\xCD", 3); return 1; }
        //if (keyCode == KEY_CHAR_POW) { return "\xE6\x47"; }
    }

    return 0;
}

void drawBox(int x1, int y1, int x2, int y2, int color) {
    int x, y;
    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            Bdisp_SetPoint_VRAM(x, y, color);
        }
    }
}
void drawTitleBar(const char* menuTitle) {
    int xStart;

    drawBox(0, 0, 127, 7, 1);
    xStart = 63 - strlen(menuTitle) * 3;
    PrintXY(xStart, 0, menuTitle, 1);
}
void drawPatern(int x, int y, int width, int height, const unsigned char* data, char inversed) {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            Bdisp_SetPoint_VRAM(x + i, y + j, inversed ? !data[j*width+i] : data[j*width+i]);
        }
    }
}
void drawButton(int column, int inversed) {
    drawPatern(2+21*column, 57, 19, 7, inversed ? filledActionButtonBg : actionButtonBg, 0);
}

int showErrorCode(int errorCode) {
    unsigned int key;
    char* buffer;

    while (1) {
        Bdisp_AllClr_VRAM();
        RestoreDisp(SAVEDISP_PAGE1);

        PrintXY(15, 33, "Error code : ", 0);
        itoa(errorCode, buffer);
        PrintXY(93, 33, (const unsigned char*)buffer, 0);

        PrintMini(3,56, "[F1] retry", 0);
        PrintMini(74,56, "[EXIT] cancel", 0);
        Bdisp_PutDisp_DD();

        GetKey(&key);
        if (key == KEY_CTRL_EXIT) { return 1; }
        if (key == KEY_CTRL_F1) { return 0; }
    }
}

int handleGroupChat(int groupIndex) {
    unsigned int key;
    int i;
    int loopCount;
    int error, errorResult, result;

    char sendData[128];
    char recieveData[128];
    char groupName[MAX_GROUP_NAME_LENGTH+1];

    int cursor = 0;         // cursor position in string
    int displayCursor = 0;  // displayed cursor position on screen
    int cursorStart = 0;    // cursor offset
    
    int editMode = 1;       // bool
    int keyState = 0;       // 0: default; 1: temp shift; 2: temp alpha; 3: perm alpha
    int uppercase = 1;      // bool
    int showCursor = 0;     // bool

    char* buffer;
    char charToAdd[3];
    char writeMessage[MESSAGES_PER_PAGE * MAX_MESSAGE_LENGTH + 1];

    int numberOfUser;
    char users[MAX_GROUP_USERS][3];

    Message messages[MAX_LOADED_MESSAGE];
    int messageStartIndex = 0;
    int messageCount = 0;
    int displayedMessageCursor = 0;

    ConnectToCurrentGroup:
    strncpy(sendData, "WMCP Casio - connect to group of index ", 128);
    itoa(groupIndex, buffer);
    strcat(sendData, buffer);
    error = sendDataPacket(sendData);
    if (error != 0) {
        errorResult = showErrorCode(error+100);
        if (errorResult == 0) { goto ConnectToCurrentGroup; }
        else return 0;
    }

    error = recieveDataPacket(recieveData, 50, 0);
    if (error != 0) {
        errorResult = showErrorCode(error+200);
        if (errorResult == 0) { goto ConnectToCurrentGroup; }
        else return 0;
    }
    strncpy(groupName, recieveData, MAX_GROUP_NAME_LENGTH+1);

    GetNumberOfUser:
    error = sendDataPacket("WMCP Casio - get number of user");
    if (error != 0) {
        errorResult = showErrorCode(error+100);
        if (errorResult == 0) { goto GetNumberOfUser; }
        else return 0;
    }

    error = recieveDataPacket(recieveData, 50, 0);
    if (error != 0) {
        errorResult = showErrorCode(error+200);
        if (errorResult == 0) { goto GetNumberOfUser; }
        else return 0;
    }

    if (strcmp(recieveData, "No group connected to") == 0) {
        errorResult = showErrorCode(301);
        if (errorResult == 0) { goto ConnectToCurrentGroup; }
        else return 0;
    }
    else numberOfUser = recieveData[0];
    for (i = 0; (i < numberOfUser) && (i < MAX_GROUP_USERS); i++) {
        GetUserName:
        strncpy(sendData, "WMCP Casio - get user name of index ", 128);
        itoa(i, buffer);
        strcat(sendData, buffer);
        error = sendDataPacket(sendData);
        if (error != 0) {
            errorResult = showErrorCode(error+100);
            if (errorResult == 0) { goto GetUserName; }
            else return 0;
        }

        error = recieveDataPacket(recieveData, 50, 0);
        if (error != 0) {
            errorResult = showErrorCode(error+200);
            if (errorResult == 0) { goto GetUserName; }
            else return 0;
        }
        
        if (strcmp(recieveData, "No group connected to") == 0) {
            errorResult = showErrorCode(301);
            if (errorResult == 0) { goto ConnectToCurrentGroup; }
            else return 0;
        }
        else strncpy(users[i], recieveData, 3);
    }

    while (1) {
        if (loopCount == 0) {
            error = sendDataPacket("WMCP Casio - test");
            if (error != 0) {
                errorResult = showErrorCode(error+100);
                if (errorResult == 0) { goto SkipAskingForMessages; }
                else return 0;
            }

            error = recieveDataPacket(recieveData, 50, 0);
            if (error != 0) {
                errorResult = showErrorCode(error+200);
                if (errorResult == 0) { goto SkipAskingForMessages; }
                else return 0;
            }

            if (strcmp(recieveData, "No group connected to") == 0) {
                errorResult = showErrorCode(301);
                if (errorResult == 0) { goto ConnectToCurrentGroup; }
                else return 0;
            }
            else if (strncmp(recieveData, "New message: ", 13) == 0) {
                messages[messageCount % MAX_LOADED_MESSAGE].userIndex = recieveData[13];
                strncpy(messages[messageCount % MAX_LOADED_MESSAGE].message, "Test n+0", MESSAGES_PER_PAGE*MAX_MESSAGE_LENGTH);

                messageCount++;
                if (messageCount >= MAX_LOADED_MESSAGE) messageStartIndex = (messageCount % MAX_LOADED_MESSAGE + 1) % MAX_LOADED_MESSAGE;
                displayedMessageCursor = max(max(messageCount, MAX_LOADED_MESSAGE) - MESSAGES_PER_PAGE, 0);
            }
        }
        //loopCount = (loopCount + 1) % 20;

        SkipAskingForMessages:
        Bdisp_AllClr_VRAM();
        drawTitleBar(groupName);

        for (i = 0; i < messageCount && i < MESSAGES_PER_PAGE; i++) {
            PrintXY(0, 9+i*8, users[messages[(messageStartIndex+i+displayedMessageCursor)%MAX_LOADED_MESSAGE].userIndex], 0);
            PrintXY(13, 9+i*8, ":", 0);
            PrintXY(19, 9+i*8, messages[(messageStartIndex+i+displayedMessageCursor)%MAX_LOADED_MESSAGE].message, 0);
        }

        if (editMode) {
            PrintXY(0, 57, writeMessage+cursorStart, 0);

            if (showCursor) {
                if (keyState == 0) { drawPatern(0 + displayCursor * 6, 57, 2, 7, defaultCursorIcon, 0); }
                else if (keyState == 1) { drawPatern(1 + displayCursor * 6, 57, 5, 7, shiftCursorIcon, 0); }
                else if (uppercase) { drawPatern(1 + displayCursor * 6, 57, 5, 7, uppercaseAlphaCursorIcon, 0); }
                else { drawPatern(1 + displayCursor * 6, 57, 5, 7, lowercaseAlphaCursorIcon, 0); }
            }

            drawBox(106, 56, 127, 63, 0);
            drawButton(5, 0);
            drawPatern(113, 59, 7, 5, proceedIcon, 0);
        } else {
            drawButton(0, 1);
            drawPatern(7, 58, 9, 5, quitIcon, 1);
            drawButton(1, 0);
            drawPatern(30, 59, 5, 5, cancelIcon, 0);
            drawButton(3, 0);
            drawPatern(72, 60, 5, 3, navigateToBottomIcon, 0);
            drawButton(4, 1);
            drawPatern(88, 58, 15, 5, characterSelectionIcon, 1);
            drawButton(5, 0);
            drawPatern(109, 59, 15, 5, uppercaseToLowercaseIcon, 0);
        }

        itoa(key, buffer);
        PrintXY(0, 30, buffer, 0);
        Bdisp_PutDisp_DD();

        if (GetKeyWait(KEYWAIT_HALTON_TIMERON, 1, 0, &key) == KEYREP_KEYEVENT) {
            //if (key == KEY_CTRL_MENU) return -1;
            if (key == KEY_CTRL_F1 && !editMode) { PrintXY(0, 57, "Loading...           ", 0); Bdisp_PutDisp_DD(); return 0; }
            else if (key == KEY_CTRL_F2 && !editMode) editMode = 1;
            else if (key == KEY_CTRL_F4 && !editMode) { /* Go to bottom */ }
            else if (key == KEY_CTRL_F5 && !editMode) /* Go to character selection menu */ editMode = 1;
            else if (key == KEY_CTRL_F6) {
                if (editMode) {
                    /* Send message */
                    cursor = 0; displayCursor = 0; cursorStart = 0;
                    memset(writeMessage, 0, MESSAGES_PER_PAGE * MAX_MESSAGE_LENGTH + 1);
                }
                else { uppercase = !uppercase; editMode = 1; }
            }
            else if (key == KEY_CTRL_EXE) {
                if (editMode) {
                    /* Send message */
                    cursor = 0; displayCursor = 0; cursorStart = 0;
                    memset(writeMessage, 0, MESSAGES_PER_PAGE * MAX_MESSAGE_LENGTH + 1);
                }
                else editMode = 1;
            }
            else if (key == KEY_CTRL_OPTN) editMode = 0;
            else if (key == KEY_CTRL_EXIT) if (editMode) { PrintXY(0, 57, "Loading...           ", 0); Bdisp_PutDisp_DD(); return 0; } else editMode = 1;
            else if (key == KEY_CTRL_SHIFT) if (keyState == 1) keyState = 0; else keyState = 1;
            else if (key == KEY_CTRL_ALPHA) if (keyState == 0) keyState = 2; else if (keyState == 1) keyState = 3; else keyState = 0;
            else if (key == KEY_CTRL_UP && displayedMessageCursor > 0) displayedMessageCursor--;
            else if (key == KEY_CTRL_DOWN && displayedMessageCursor < (max(messageCount, MAX_LOADED_MESSAGE) - MESSAGES_PER_PAGE)) displayedMessageCursor++;
            else if ((key == KEY_CTRL_LEFT || key == KEY_CTRL_DEL)) {
                if (cursor > 0) {
                    cursor--;
                    if ((displayCursor < MESSAGE_EDIT_LENGTH) && cursorStart > 0) cursorStart--;
                    else displayCursor--;

                    if (key == KEY_CTRL_DEL) strncpy(writeMessage+cursor, writeMessage+cursor+1, strlen(writeMessage) - cursor);
                }
            }
            else if (key == KEY_CTRL_RIGHT) {
                if (cursor < strlen(writeMessage)) {
                    cursor++;
                    if ((cursorStart == 0 && displayCursor < MESSAGE_EDIT_LENGTH - 1) || cursorStart == strlen(writeMessage) - MESSAGE_EDIT_LENGTH) displayCursor++;
                    else cursorStart++;
                }
            }
            else if (cursor < MESSAGES_PER_PAGE * MAX_MESSAGE_LENGTH) {
                result = KeyCodeToChar(charToAdd, key, keyState, uppercase);

                if (result != 0) {
                    strncpy(writeMessage+cursor+strlen(charToAdd), writeMessage+cursor, MESSAGES_PER_PAGE*MAX_MESSAGE_LENGTH-cursor-strlen(charToAdd));
                    strncpy(writeMessage+cursor, charToAdd, strlen(charToAdd));

                    cursor++;
                    if ((cursorStart == 0 && displayCursor < MESSAGE_EDIT_LENGTH - 1) || cursorStart == strlen(writeMessage) - MESSAGE_EDIT_LENGTH) displayCursor++;
                    else cursorStart++;

                    if (keyState != 3) keyState = 0;
                }
            }
            showCursor = 1;
        }
        else showCursor = !showCursor;
    }
}
int handleDiscussionGroupsMenu() {
    unsigned int key;
    int i;
    char* buffer;

    int sendError, recieveError;
    int errorResult;

    char sendData[128];
    char recieveData[128];

    int mode;                   // 0: no group  1: 4 or less group  2: more than 4 group
    int forceRefresh = 1;
    char numberOfGroups = 0;
    int selectedGroup = 0;
    int currentPage = 0;
    int onPageCursor;

    char groupNames[MAX_GROUP_NAME][MAX_GROUP_NAME_LENGTH+1];

    while (1) {
        if (forceRefresh) {
            forceRefresh = 0;

            GetNumberOfGroups:
            sendError = sendDataPacket("WMCP Casio - get group number");
            if (sendError != 0) {
                errorResult = showErrorCode(sendError+100);
                if (errorResult == 0) { goto GetNumberOfGroups; }
                else return -1;
            }

            recieveError = recieveDataPacket(recieveData, 50, 0);
            if (recieveError != 0) {
                errorResult = showErrorCode(recieveError+200);
                if (errorResult == 0) { goto GetNumberOfGroups; }
                else return -1;
            }

            if (recieveData[0] != numberOfGroups) {
                numberOfGroups = recieveData[0];

                for (i = 0; (i < numberOfGroups) && (i < MAX_GROUP_NAME); i++) {
                    GetGroupName:
                    strncpy(sendData, "WMCP Casio - get group name of index ", 128);
                    itoa(i, buffer);
                    strcat(sendData, buffer);
                    sendError = sendDataPacket(sendData);
                    if (sendError != 0) {
                        errorResult = showErrorCode(sendError+100);
                        if (errorResult == 0) { goto GetGroupName; }
                        else return -1;
                    }

                    recieveError = recieveDataPacket(recieveData, 50, 0);
                    if (recieveError != 0) {
                        errorResult = showErrorCode(recieveError+200);
                        if (errorResult == 0) { goto GetGroupName; }
                        else return -1;
                    }

                    strncpy(groupNames[i], recieveData, MAX_GROUP_NAME_LENGTH+1);
                }
            }
        }

        if (numberOfGroups == 0) mode = 0;
        else if (numberOfGroups <= 4) mode = 1;
        else mode = 2;

        Bdisp_AllClr_VRAM();
        drawTitleBar("Discussion Groups");
        drawButton(0, 1);
        drawPatern(7, 58, 9, 5, quitIcon, 1);

        if (mode != 0) {
            currentPage = (int)(selectedGroup / GROUP_NAME_PER_PAGE);

            if (mode == 2) {
                if (currentPage > 0) { drawPatern(61, 12, 6, 3, upArrow, 0); }
                if (currentPage < (int)((numberOfGroups-1) / GROUP_NAME_PER_PAGE)) { drawPatern(61, 53, 6, 3, downArrow, 0); }
            }

            onPageCursor = selectedGroup % GROUP_NAME_PER_PAGE;
            Bdisp_SetPoint_VRAM(30, 20 + onPageCursor * 9, 1);
            drawPatern(33, 16 + onPageCursor * 9, 63, 9, selectedGroupBg, 0);

            if (selectedGroup > 0) drawPatern(29, 17 + onPageCursor * 9, 3, 2, miniUpArrow, 0);
            if (selectedGroup < (numberOfGroups-1)) drawPatern(29, 22 + onPageCursor * 9, 3, 2, miniDownArrow, 0);

            for (i = 0; (i < numberOfGroups-currentPage*4) && (i < 4); i++) PrintXY(34, 17 + i * 9, groupNames[i+currentPage*GROUP_NAME_PER_PAGE], (onPageCursor == i) ? 1 : 0);

            drawButton(2, 1);
            drawPatern(51, 58, 5, 5, settingsIcon, 1);
            drawButton(3, 0);
            drawPatern(72, 59, 5, 5, refreshIcon, 0);
            drawButton(4, 1);
            drawPatern(92, 58, 7, 5, editIcon, 1);
            drawButton(5, 1);
            drawPatern(113, 58, 7, 5, proceedIcon, 1);
        } else {
            drawPatern(59, 21, 11, 14, noGroupFoundIcon, 0);
            PrintXY(21, 36, "No group found", 0);
            drawButton(4, 1);
            drawPatern(93, 58, 5, 5, settingsIcon, 1);
            drawButton(5, 0);
            drawPatern(114, 59, 5, 5, refreshIcon, 0);
        }

        Bdisp_PutDisp_DD();

        GetKey(&key);
        if (key == KEY_CTRL_F1 || key == KEY_CTRL_EXIT) return -1;
        else if (key == KEY_CTRL_UP && selectedGroup > 0) selectedGroup--;
        else if (key == KEY_CTRL_DOWN && selectedGroup < (numberOfGroups-1)) selectedGroup++;
        else if ((mode == 0 && (key == KEY_CTRL_F5 || key == KEY_CTRL_OPTN)) || (mode != 0 && (key == KEY_CTRL_F3 || key == KEY_CTRL_OPTN))) {} // open settings
        else if ((mode == 0 && key == KEY_CTRL_F6) || (mode != 0 && key == KEY_CTRL_F4)) forceRefresh = 1;
        else if (mode != 0 && key == KEY_CTRL_F5) {}                                                                                            // edit selected group name
        else if (mode != 0 && (key == KEY_CTRL_F6 || key == KEY_CTRL_EXE)) return selectedGroup;
    }
}

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key;
    char* buffer;

    int sendError, recieveError;
    int errorResult;

    char recieveData[128];

    int selectedGroupIndex;

    unsigned char conf[] = {0,5,0,0,0,0};

    Serial_Open(conf);

    SendInitialisation:
    Bdisp_AllClr_VRAM();
    PrintXY(12, 1,  "Wireless  Message", 0);
    PrintXY(0, 9,  "Communication Program", 0);
    Bdisp_DrawLineVRAM(0, 20, 127, 20);
    Bdisp_DrawLineVRAM(0, 52, 127, 52);
    SaveDisp(SAVEDISP_PAGE1);

    PrintXY(14, 33, "Initialisation...", 0);
    PrintMini(33,56, "[EXIT] to cancel", 0);
    Bdisp_PutDisp_DD();

    sendError = sendDataPacket("WMCP Casio - initialisation");
    if (sendError != 0) {
        errorResult = showErrorCode(sendError+100);
        if (errorResult == 0) goto SendInitialisation;
        else goto CloseApplication;
    }

    recieveError = recieveDataPacket(recieveData, 500, 1);
    if (recieveError == 0 && strcmp(recieveData, "WMCP Transmition Module - ready") != 0) recieveError = 6;
    if (recieveError != 0) {
        errorResult = showErrorCode(recieveError+200);
        if (errorResult == 0) goto SendInitialisation;
        else goto CloseApplication;
    }

    while (1) {
        selectedGroupIndex = handleDiscussionGroupsMenu();
        if (selectedGroupIndex == -1) goto CloseApplication;

        if (handleGroupChat(selectedGroupIndex) == -1) goto CloseApplication;
    }

    CloseApplication:
    Serial_Close(0);
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
