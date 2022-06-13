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
    unsigned char main_file[260] = "";
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
                main_file[j] = file[i];

                if (file[i] == 92) {
                    j++;
                    main_file[j] = 92;
                }
                j++;
                i++;
            }
        }
    }
    FILE* output;
    output = fopen(main_file, "rb"); //файл куда записывается закодированный код
    char word[MAXWORD];

    int k = 0;

    int flag = 1;
    while (flag == 1) {

        if (fscanf(output, "%s", &word) != 0) {
            if (word[0] == '\0') { break; };
            symbols[k] = word[0];
            fscanf(output, "%s", &word);
            strcpy(t_code[k], word);
            k++;
        }
        else { flag = 0; }

    }
    int n = k;

    FILE* fw = fopen("output3.txt", "w");
    unsigned char byte;
    unsigned char out_str[65535] = "";
    fread(&byte, 1, 1, output); // очистка кэша файла
 
    while (fread(&byte, 1, 1, output) == 1)
    {
        char bits[9] = { 0 };
        if (byte == 0) {
            unsigned char out_second_str[MAXWORD] = "";
            for (int i = 0; i < strlen(out_str); i++) {

                if (out_str[i] == '0') {
                    strcat(out_second_str, "0");
                }
                else { strcat(out_second_str, "1"); }
                for (int j = 0; j < n; ++j) {
                    if (!strcmp(out_second_str, t_code[j])) {
                        if (symbols[j] == 1)
                            fprintf(fw, "%s", " ");
                        else
                            fprintf(fw, "%c", symbols[j]);
                        memset(out_second_str, 0, sizeof(out_second_str));
                        break;
                    }
                }
            }
            memset(out_str, 0, sizeof(out_str));
        }
        else {
            strcat(out_str, OutByte(byte, bits));
        }
    }
    fclose(fw);
    fclose(output);

}