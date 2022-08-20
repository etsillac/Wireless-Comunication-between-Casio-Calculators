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

int AddIn_main(int isAppli, unsigned short OptionNum) {
    unsigned int i;
    unsigned int count = 0;
    char* buffer;

    while (1) {
        Bdisp_AllClr_VRAM();

        itoa(count, buffer);
        PrintXY(0, 0, buffer, 0);

        Bdisp_PutDisp_DD();

        i = 100;
        while (i) {
            if (IsKeyDown(KEY_CTRL_EXE)) { count++; }
            i--;
            Sleep(10);
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
