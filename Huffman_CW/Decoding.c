#pragma once
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>

//глоб. пременные
#define MAXWORD 512




char* OutByte(unsigned char byte, char bits[])//из чар символа в последовательность 0 и 1
{
    size_t i;
    for (i = 0; i < 8; i++)
    {
        bits[7 - i] = (byte & 1) ? '1' : '0';
        byte >>= 1;
    }

    return bits;
}

void DeCodding() {
    printf("Specify the path to the file to be decoded.\n");
    char symbols[MAXWORD]; //символы для декодирования
    char t_code[MAXWORD][MAXWORD]; //таблица кодов Хаффмана
    OPENFILENAME openFileDialog;
    char file[260];
    unsigned char fileName[260] = "";
    ZeroMemory(&openFileDialog, sizeof(openFileDialog));
    openFileDialog.lStructSize = sizeof(openFileDialog);
    openFileDialog.hwndOwner = NULL;
    openFileDialog.lpstrFile = file;
    openFileDialog.lpstrFile[0] = '\0';
    openFileDialog.nMaxFile = sizeof(file);
    openFileDialog.lpstrFilter = L"Bin files (*.bin)\0*.bin\0All files 2007\0*.*\0";
    openFileDialog.nFilterIndex = 1;
    openFileDialog.lpstrFileTitle = NULL;
    openFileDialog.nMaxFileTitle = 0;
    openFileDialog.lpstrInitialDir = NULL;
    openFileDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    if (GetOpenFileName(&openFileDialog)) {
        int i = 0;
        int j = 0;
        while (file[i] != -52) {
            if (file[i] == 0) {
                i++;
            }
            else {
                fileName[j] = file[i];

                if (file[i] == 92) {
                    j++;
                    fileName[j] = 92;
                }
                j++;
                i++;
            }
        }
    }
    FILE* output;
    output = fopen(fileName, "rb"); //файл куда записывается закодированный код
    char word[MAXWORD];

    int k1 = 0;

    int fl = 1;
    while (fl == 1) {

        if (fscanf(output, "%s", &word) != 0) {
            if (word[0] == '\0') { break; };
            symbols[k1] = word[0];
            fscanf(output, "%s", &word);
            strcpy(t_code[k1], word);
            k1++;
        }
        else { fl = 0; }

    }
    int n1 = k1;

    FILE* fw = fopen("output3.txt", "w");
    unsigned char byte;
    unsigned char out[65535] = "";
    fread(&byte, 1, 1, output); // очистка кэша файла
    int flag2 = 1;
    while (fread(&byte, 1, 1, output) == 1)
    {
        char bits[9] = { 0 };
        if (byte == 0) {
            unsigned char out2[MAXWORD] = "";
            for (int i = 0; i < strlen(out); i++) {

                if (out[i] == '0') {
                    strcat(out2, "0");
                }
                else { strcat(out2, "1"); }
                for (int j = 0; j < n1; ++j) {
                    if (!strcmp(out2, t_code[j])) {
                        if (symbols[j] == 1)
                            fprintf(fw, "%s", " ");
                        else
                            fprintf(fw, "%c", symbols[j]);
                        memset(out2, 0, sizeof(out2));
                        break;
                    }
                }
            }
            memset(out, 0, sizeof(out));
        }
        else {
            strcat(out, OutByte(byte, bits));
        }
    }
    fclose(fw);
    fclose(output);

}