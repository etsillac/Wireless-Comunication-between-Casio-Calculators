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

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int key;
    int count;

    char* buffer;

    while (1) {
        Bdisp_AllClr_VRAM();

        itoa(count, buffer);
        PrintMini(0, 0, buffer, 0);

        Bdisp_PutDisp_DD();

        if (GetKeyDown(1000, &key)) {
            if (key == KEY_CTRL_MENU) { return 1; }
            else if (key == KEY_CHAR_PLUS) { count++; }
            else if (key == KEY_CHAR_MINUS) { count--; }
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
