#include <stdio.h>
#include <Windows.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>

//#include <getopt.h>
//#include <unistd.h>
//Ввиду того, что я работаю на Windows, у меня нету getopt и протестировать я его не могу, 
//поэтому я просто решил считывать параметры с консоли при старте
int main() {
    char cons[10];
    printf("If you enter -d then there will be only encoding\n");
    printf("if +d then both encoding and decoding, if something else, then only decoding.\n");
    scanf("%s", cons);

    if (!strcmp(cons, "-d")) {

        Codding();
    }
    else if (!strcmp(cons, "+d")) {
        Codding();
        DeCodding();
    }
    else {
        DeCodding();
    }
    printf("The end!\n");
    return 0;
}