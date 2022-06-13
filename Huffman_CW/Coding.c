#pragma once
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>

#define MAXWORD 512
int cnt_str[MAXWORD]; //кол-во появления символа в строке
char str[MAXWORD]; //сам символ
int cnt; // кол-во символов
typedef char** h_code; // массив таблицы Хаффмана


struct tnode { //структура для определения кол-ва символов в тексте.      
    char word;
    int count;
    struct tnode* left;
    struct tnode* right;
};

// Функция добавления узла
struct tnode* make_tree(struct tnode* p, char w) {
    int cond;

    if (p == NULL) {
        p = (struct tnode*)malloc(sizeof(struct tnode));
        p->word = w;
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = (w - p->word)) == 0) {

        p->count++;
    }
    else if (cond < 0) {

        p->left = make_tree(p->left, w);
    }
    else {

        p->right = make_tree(p->right, w);
    }
    return p;
}

// Функция сохранения глобальных переменных
void treereturn(struct tnode* p) {
    if (p != NULL) {
        treereturn(p->right);
        cnt_str[cnt] = p->count;
        str[cnt] = p->word;
        cnt++;
        treereturn(p->left);
    }
}



typedef struct {
    unsigned int weight;
    unsigned int parent;
    unsigned int left;
    unsigned int right;
}node, * h_tree;    // Динамическое размещение массива для хранения дерева Хаффмана

void add_Tree(h_tree h_t, int i, int* s_left, int* s_right, int k_start) {
    //создание узла дерева Хаффмана, совмещая два соседних узла под 1 новый узел
    int w_left = 0;
    int w_right = 0;
    int j = 0;
    for (int k = k_start; k <= i - 1; k++) {
        if (h_t[k].parent == 0) {
            if (j == 0) {
                w_left = h_t[k].weight;
                *s_left = k;
            }
            else if (j == 1) {
                if (h_t[k].weight < w_left) {
                    //этот if заходит тогда, когда у нас созданный узел имеет вес больше, чем узел, который мы уже создали
                    //например узел k = n имеет вес 4, а узел k = n + 1, который уже имеет у себя левые и правые узлы имеет вес меньший узла k=n (1)
                    w_right = w_left;
                    *s_right = *s_left;
                    w_left = h_t[k].weight;
                    *s_left = k;
                }
                else {  //этот if создает пару для 1 узла
                    w_right = h_t[k].weight;
                    *s_right = k;
                }
            }
            else {
                if (h_t[k].weight < w_left) {
                    w_right = w_left;
                    *s_right = *s_left;
                    w_left = h_t[k].weight;
                    *s_left = k;
                    break;
                }
                else
                if (h_t[k].weight >= w_left && h_t[k].weight < w_right) {   // этот if заходит тогда, когда вылетел if (1), чтобы проверить, есть ли еще пара
                    w_right = h_t[k].weight;
                    *s_right = k;
                    break;
                }
            }
            j++;
        }
    }
}
void HuffmanCoding(h_tree* h_t, h_code* h_c, int* w, int n) {
    if (n <= 1) return;

    int m = 2 * n - 1; //узлы дерева
    *h_t = (h_tree)malloc(m * sizeof(node));
    h_tree p = *h_t;

    for (int i = 0; i < n; i++) {
        // записываем все веса cnt_str, хранящиеся в *w в p, создавая новые узлы
        p->weight = *w;
        p->parent = 0;
        p->left = -1;
        p->right = -1;
        w++;
        p++;
    }

    for (int i = n; i < m; i++) {
        //оставшиеся узлы заполняем нулями 
        p->weight = 0;
        p->parent = 0;
        p->left = 0;
        p->right = 0;
        p++;
    }
    int k_start = 0;
    for (int i = n; i < m; ++i) {
        // Строим дерево Хаффмана 
        // берем 2 узла и объединем под 1
        unsigned int s1, s2;
        add_Tree(*h_t, i, &s1, &s2, k_start);
        (*h_t)[s1].parent = i;
        (*h_t)[s2].parent = i;
        (*h_t)[i].left = s1;
        (*h_t)[i].right = s2;
        (*h_t)[i].weight = (*h_t)[s1].weight + (*h_t)[s2].weight;//узел - родитель у 2 узло имеет сумму их весов
        k_start++;
    }


    (*h_c) = (h_code)malloc(n * sizeof(char*));// выделяем память под сам код хаффмана в обратном порядке
    char* cd = (char*)malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    int start, f, c;
    for (int i = 0; i < n; ++i) {
        start = n - 1;
        for (c = i, f = (*h_t)[i].parent; f != 0; c = f, f = (*h_t)[f].parent) {
            if ((*h_t)[f].left == c)
                cd[--start] = '0';
            else cd[--start] = '1';
        }
        (*h_c)[i] = (char*)malloc((n - start) * sizeof(char));
        strcpy((*h_c)[i], &cd[start]);
    }
    free(cd);
}




// я решил использовать быструю сортировку, для сортировки исходных элементов, 
// так как если веса будут отсортированы, то алгоритм хаффмана будет быстрее проходить 
void sort(int f, int l) { //быстрая сортировка
    int  i, j;
    int L;
    i = f;
    j = l;
    L = cnt_str[(f + l) / 2];
    while (i <= j)
    {
        while ((cnt_str[i] < L) && (i < l))
        {
            i++;
        }
        while ((L < cnt_str[j]) && (j > f))
        {
            j--;
        }
        if (i <= j)
        {
            int b = cnt_str[j];
            cnt_str[j] = cnt_str[i];
            cnt_str[i] = b;
            char b1 = str[j];
            str[j] = str[i];
            str[i] = b1;
            i++;
            j--;
        }
    }
    if (f < j)
    {
        sort(f, j);
    }
    if (i < l)
    {
        sort(i, l);
    }

}


unsigned char ToBits(const char data[], int n)//функция которая записывает последовательность 0 и 1 в биты в виде чар символа
{
    unsigned char byte = 0;
    const char* end = data + n;

    for (; *data && (data < end); ++data)
    {
        byte <<= 1;

        byte |= (*data == '1') ? 1 : 0;
    }

    return byte;
}


void Codding() {

    FILE* fp, * fw;
    printf("Specify the path to the file to be encoded.\n");
    //Код открытия файла я брал из методического пособия Миронова Сергея Владимировича
    OPENFILENAME openFileDialog;
    char file[260];
    unsigned char main_file[260] = "";
    ZeroMemory(&openFileDialog, sizeof(openFileDialog));
    openFileDialog.lStructSize = sizeof(openFileDialog);
    openFileDialog.hwndOwner = NULL;
    openFileDialog.lpstrFile = file;
    openFileDialog.lpstrFile[0] = '\0';
    openFileDialog.nMaxFile = sizeof(file);
    openFileDialog.lpstrFilter = L"Text files (*.txt)\0*.txt\0All files 2007\0*.*\0";
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


    fp = fopen(main_file, "r");


    struct tnode* root;
    char word[MAXWORD];
    root = NULL;
    while (fscanf(fp, "%s", &word) != EOF) { //определяем кол-во символов в строке.
        for (int i = 0; i < strlen(word); i++) {
            root = make_tree(root, word[i]);
        }
        root = make_tree(root, ' ');
    }
    cnt = 0;
    treereturn(root);
    fclose(fp);
    int n = cnt;
    sort(0, cnt - 1);

    for (int i = 0; i < n; i++) {
        printf("%d %c\n", cnt_str[i], str[i]);

    }
    printf("%s\n", "");


    h_tree h_t;
    h_code h_c;
    HuffmanCoding(&h_t, &h_c, cnt_str, n);

    FILE* output;



    fp = fopen(main_file, "r"); //исходный файл, который будем кодировать

    printf("Specify the path to the file to which you want to encode the information.\n");
    OPENFILENAME openSecondFileDialog;
    char file_s[260];
    unsigned char second_file[260] = "";
    ZeroMemory(&openSecondFileDialog, sizeof(openSecondFileDialog));
    openSecondFileDialog.lStructSize = sizeof(openSecondFileDialog);
    openSecondFileDialog.hwndOwner = NULL;
    openSecondFileDialog.lpstrFile = file_s;
    openSecondFileDialog.lpstrFile[0] = '\0';
    openSecondFileDialog.nMaxFile = sizeof(file_s);
    openSecondFileDialog.lpstrFilter = L"Text files (*.txt)\0*.txt\0All files 2007\0*.*\0";
    openSecondFileDialog.nFilterIndex = 1;
    openSecondFileDialog.lpstrFileTitle = NULL;
    openSecondFileDialog.nMaxFileTitle = 0;
    openSecondFileDialog.lpstrInitialDir = NULL;
    openSecondFileDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    if (GetOpenFileName(&openSecondFileDialog)) {
        int i = 0;
        int j = 0;
        while (file_s[i] != -52) {
            if (file_s[i] == 0) {
                i++;
            }
            else if (file_s[i] == 't' && file_s[i + 2] == 'x' && file_s[i + 4] == 't') {
                second_file[j] = 'b';
                second_file[j + 1] = 'i';
                second_file[j + 2] = 'n';
                break;
            }
            else {
                second_file[j] = file_s[i];

                if (file_s[i] == 92) {
                    j++;
                    second_file[j] = 92;
                }
                j++;
                i++;
            }
        }
    }

    output = fopen(second_file, "wb"); //файл куда записывается закодированный код
    for (int i = 0; i < n; i++) {
        printf("%c %s\n", str[i], h_c[i]);
        if (str[i] == 32) {
            fprintf(output, "%c %s\n", 1, h_c[i]);
        }
        else {
            fprintf(output, "%c %s\n", str[i], h_c[i]);
        }

    }
    unsigned char byte = ToBits("0", 1);
    fwrite(&byte, 1, 1, output);
    fprintf(output, "\n");

    // запись в файл, записываем 8 бит = 1 байту в char символ, цикл пока не кратен 8 увеличиваем str_cod строкой, потом кодируем
    //когда уже конец файла, а мы не закодировали str_cod, то кодируем что осталось с добавлением 0 в последнем байте
    int flag_second = 1;
    int flag_first = 1;
    unsigned char str_cod[65535] = "";
    while (flag_first == 1) {
        if (fscanf(fp, "%s", &word) != EOF) {
            if (flag_second == 1) {
                memset(str_cod, 0, sizeof(str_cod));
            }
            for (int i = 0; i < strlen(word); i++) {
                for (int j = 0; j < n; ++j) {
                    if (word[i] == str[j]) {
                        strcat(str_cod, h_c[j]);
                        break;
                    }
                }
            }
            for (int j = 0; j < n; ++j) {//этот цикл нужно оптимизировать, тут просто поиск кода пробела.
                if (32 == str[j]) {
                    strcat(str_cod, h_c[j]);
                    break;
                }
            }
            if (strlen(str_cod) % 8 == 0) {
                const char* str_array = str_cod;
                if (strlen(str_cod) == 8) {
                    unsigned char byte = ToBits(str_array, strlen(str_cod));
                    fwrite(&byte, 1, 1, output);

                }
                else {
                    int g = strlen(str_cod);
                    while (g > 0) {
                        if (g - 8 >= 0) {
                            g -= 8;
                            unsigned char byte = ToBits(str_array, 8);
                            fwrite(&byte, 1, 1, output);
                            str_array += 8;
                        }
                        else {
                            int k = 8;
                            while (g - k != 0) {
                                k--;
                            }
                            unsigned char byte = ToBits(str_array, k);
                            fwrite(&byte, 1, 1, output);
                            g = 0;
                        }


                    }


                }
                flag_second = 1;
                str_array = "0";
                unsigned char byte = ToBits(str_array, 1);
                fwrite(&byte, 1, 1, output);
            }
            else {
                flag_second = 0;
            }
        }
        else {
            const char* str_array = str_cod;
            if (strlen(str_cod) == 8) {
                unsigned char byte = ToBits(str_array, strlen(str_cod));
                fwrite(&byte, 1, 1, output);

            }
            else {
                int g = strlen(str_cod);
                while (g > 0) { //12
                    if (g - 8 >= 0) {
                        g -= 8;
                        unsigned char byte = ToBits(str_array, 8);
                        fwrite(&byte, 1, 1, output);
                        str_array += 8;
                    }
                    else {
                        int k = 8;
                        while (g - k != 0) {
                            k--;
                        }
                        unsigned char byte = ToBits(str_array, k);
                        fwrite(&byte, 1, 1, output);
                        g = 0;
                    }
                }
            }
            flag_first = 0;
            str_array = "0";
            unsigned char byte = ToBits(str_array, 1);
            fwrite(&byte, 1, 1, output);
        }
    }
    memset(str_cod, 0, sizeof(str_cod));
    fclose(fp);
    fclose(output);





}