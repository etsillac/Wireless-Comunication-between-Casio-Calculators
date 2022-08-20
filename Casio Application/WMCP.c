#include "fxlib.h"
#include "sh4Compability.h"
#include "syscall.h"

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

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key;
    int hasKeyBeenPressed = 0;

    char* buffer;

    while (1) {
        Bdisp_AllClr_VRAM();

        if (hasKeyBeenPressed) {
            itoa(key, buffer);
            PrintXY(0, 0, buffer, 0);
        }
        else { PrintXY(0, 0, "No key pressed", 0); }

        Bdisp_PutDisp_DD();

        hasKeyBeenPressed = GetKeyDown(1000, &key);
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
