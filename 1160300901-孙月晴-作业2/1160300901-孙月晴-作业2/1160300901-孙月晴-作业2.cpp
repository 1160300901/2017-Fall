#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <fstream>

#define  N 256
typedef struct {
    char name;
    int weight; //Ȩ��
    int lchild;   //������
    int rchild;  //������
    int parent;  //���ڵ�
} HTNODE;
typedef struct {
    char ch;  //��������ַ�
    char bits[N + 1]; //�ַ�����λ��
} CodeNode;
CodeNode HuffmanCode[N];
HTNODE HuffmanT[2 * N - 1];

void CreatHT(HTNODE *T);   //������������
void InitHT(HTNODE *T);    //��ʼ����������
void InputW(HTNODE *T);   //ͳ������Ȩ��
void SelectMin(HTNODE *T, int n, int *p1, int *p2);  //ѡȡ����Ȩ����С�ĸ��ڵ�
void CharSetHuffmanEncoding(HTNODE *T, CodeNode *H); //��������������
void WriteToFile(HTNODE *T, CodeNode *H);  //������������д���ļ�
void TransferToBit();  //ת��Ϊ�������ļ�
void TransferToArticle(HTNODE *T); //����
void DecodeArticle(HTNODE *T);
double PressRate();
int flag = 0, len = 0;
int article_num = 0;
int word_num = 0;
using namespace std;

int main() {
    char flag;     //�����־��
    CreatHT(HuffmanT); //������������
    CharSetHuffmanEncoding(HuffmanT, HuffmanCode); //��������������
    WriteToFile(HuffmanT, HuffmanCode); //������������д���ļ�
    int choice=1;
    while (choice) {
        printf("************ �� �� �� �� �� �� �� �� �� ʵ �� ************\n");
        printf("\t1.���ַ���ʹ��Ƶ��\n");
        printf("\t2.�ַ����Ĺ����������\n");
        printf("\t3.�����ļ�ѹ��������ѹ����\n");
        printf("\t4.�ļ���ѹ\n");
        printf("\t0.�˳�����������\n");
        printf("\n������0-4,ѡ��Ҫִ�еĲ���:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                printf("--------------------------------------\n");
                int i = 0;
                printf("\t �ַ� \t ����Ƶ�� \n");
                while (i < N) {
                    if (HuffmanT[i].weight != 0) {
                        if (i == 10) {
                            printf("\t %c   (���з�)\t %d\n", i, HuffmanT[i].weight);
                        } else
                            printf("\t %c\t %d\n", i, HuffmanT[i].weight);
                    }
                    i++;
                }
                printf("--------------------------------------\n");
            }
                system("pause");
                break;
            case 2: {
                printf("--------------------------------------\n");
                int j = 0;
                printf("\t �ַ� \t ���������� \n");
                while (j < N) {
                    if (HuffmanT[j].weight != 0) {
                        if (j == 10) {
                            printf("\t %c   (���з�)\t %s\n", HuffmanCode[j].ch, HuffmanCode[j].bits);
                        } else
                            printf("\t %c\t %s\n", HuffmanCode[j].ch, HuffmanCode[j].bits);
                    }
                    j++;
                }
                printf("--------------------------------------\n");

            }
                system("pause");
                break;
            case 3: {
                printf("--------------------------------------\n");
                printf("ԭ�ļ�Ϊ��article.txt \n\n�����������ļ�Ϊ��str_article.txt \n\nѹ������ļ�Ϊ��bit_article.bin \n\n");
                TransferToBit();      //���ַ���01ÿ8λ1�飬д��������ļ��У������ļ�ѹ�����
                printf("�ļ�ѹ���ʣ�%.2f",PressRate()*100);
                printf("%%\n\n");
                printf("--------------------------------------\n");

            }
                system("pause");
                break;
            case 4: {
                printf("--------------------------------------\n");
                printf("��ѹ���ļ�Ϊ��bit_article.bin \n\n��ѹ����ļ�Ϊ��decode_article.txt \n\n");
                TransferToArticle(HuffmanT);  //���ļ����н�ѹ����ԭ�ļ�
                printf("--------------------------------------\n");

            }
                system("pause");
                break;
            case 0:
                break;
        }
        system("CLS");
    }
    return 0;
}

//����ѹ����
double PressRate() {
    ifstream in("article.txt");
    ifstream out("bit_article.bin");
    FILE * pFile1,* pFile2;
    long size1,size2;
    pFile2 = fopen ("bit_article.bin","rb");
    if (pFile2==NULL)
        perror ("Error opening file");
    else
    {
        fseek (pFile2, 0, SEEK_END);   ///���ļ�ָ���ƶ��ļ���β
        size2=ftell (pFile2); ///�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
        fclose (pFile2);
    }
    pFile1 = fopen ("article.txt","r");
    if (pFile1==NULL)
        perror ("Error opening file");
    else
    {
        fseek (pFile1, 0, SEEK_END);   ///���ļ�ָ���ƶ��ļ���β
        size1=ftell (pFile1); ///�����ǰ�ļ�ָ������ļ���ʼ���ֽ���
        fclose (pFile1);
    }
    return ((double)(size1-size2)/size1);
}
//������������
void CreatHT(HTNODE *T) {
    int i, p1, p2;
    InitHT(T);   //��ʼ����������
    InputW(T);   //����Ȩ��
    for (i = N; i < 2 * N - 1; i++)   //N-1�κϲ�
    {
        SelectMin(T, i - 1, &p1, &p2); //ѡȡ����Ȩ����С��
        T[p1].parent = i;  //���������������ڵ�ΪT[i]
        T[p2].parent = i;
        T[i].lchild = p1;
        T[i].rchild = p2;
        T[i].weight = T[p1].weight + T[p2].weight; //���ڵ��Ȩ��Ϊ�����ӽڵ�Ȩ�صĺ�
    }
}

//��ʼ����������
void InitHT(HTNODE *T) {
    int i;
    for (i = 0; i < 2 * N - 1; i++) {
        T[i].weight = 0; //Ȩ�س�ʼ��Ϊ0
        T[i].parent = -1; //���ڵ�Ϊ-1�ÿ�
        T[i].lchild = -1; //����
        T[i].rchild = -1; //�Һ���
    }
}

//��ȡ�ļ���ͳ������Ȩ��
void InputW(HTNODE *T) {
    FILE *fp;
    char ch;
    fp = fopen("article.txt", "r"); //���ļ�
    if (fp == NULL) {
        printf("article�ļ���ȡʧ�ܣ�\n");
        exit(0);
    }
    while (!feof(fp)) {
        fscanf(fp, "%c", &ch);
        article_num++; //ͳ����������
        T[ch].weight++; //��ӦԪ��Ȩ�ؼ�1
    }
    article_num--;   //��!feof(fp)�ڶ����ļ�β��Ҫ�ٶ�һ���ļ������һ��Ԫ�زŽ�������������article_num��һ
    T[ch].weight--;  //���һ��Ԫ�ص�Ȩ�ؼ�һ
    rewind(fp);//ʹ�ļ�ָ��ص��ļ�ͷ
    fclose(fp);  //�ر��ļ�
}

//ѡȡɭ�������ø��ڵ�Ȩ����С����
void SelectMin(HTNODE *T, int n, int *p1, int *p2) {
    int i, j;
    for (i = 0; i < n; i++) {
        if (T[i].parent == -1) //�Ҹ��ڵ�
        {
            *p1 = i;
            break;
        }
    }
    for (j = i + 1; j <= n; j++) {
        if (T[j].parent == -1) //���ڵ�
        {
            *p2 = j;
            break;
        }
    }
    for (i = 0; i <= n; i++)                   //��n+1���ڵ���Ѱ����Сֵ�ʹ�Сֵ
    {
        if ((T[*p1].weight > T[i].weight) && (T[i].parent == -1) && (*p2 != i))
            *p1 = i;
    }
    for (j = 0; j <= n; j++) {
        if ((T[*p2].weight > T[j].weight) && (T[j].parent == -1) && (*p1 != j))
            *p2 = j;
    }
}

//��������������
void CharSetHuffmanEncoding(HTNODE *T, CodeNode *H) {
    int c, p, i;
    char cd[N + 1];
    int start;
    cd[N] = '\0';
    for (i = 0; i < N; i++)  //ΪN��Ҷ�ڵ㹹����������룬��Ҷ�ڵ����������ڵ㣬��������Ϊ��0����������Ϊ��1��
    {
        H[i].ch = i;
        start = N;
        c = i;
        while ((p = T[c].parent) > 0) {
            cd[--start] = (T[p].lchild == c) ? '0' : '1';
            c = p;
        }
        strcpy(H[i].bits, &cd[start]);
    }
}

//������������д���ļ��У�ʵ���ϴ�ʱ�ǰ�01�ַ���д���ļ�
void WriteToFile(HTNODE *T, CodeNode *H) {
    FILE *fp1, *fp2;
    int i;
    fp1 = fopen("article.txt", "r");
    fp2 = fopen("str_article.txt", "w");
    if (fp1 == NULL) {
        printf("�ļ�1��ʧ�ܣ�\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("�ļ�2��ʧ�ܣ�\n");
        exit(0);
    }
    for (i = 0; i < article_num; i++) {
        fprintf(fp2, "%s", H[fgetc(fp1)].bits); //��ÿ��Ԫ�صĹ����������������Ԫ��
    }
    rewind(fp1);
    rewind(fp2);
    fclose(fp1);
    fclose(fp2);
}

//��01�ַ����ļ�ת��Ϊ�������ļ���ʵ��������ѹ��
void TransferToBit() {
    FILE *fp1, *fp2;
    int i = 0, j;
    long num = 0;
    unsigned char a;
    fp1 = fopen("str_article.txt", "r");
    fp2 = fopen("bit_article.bin", "wb"); //д�������ļ�
    if (fp1 == NULL) {
        printf("�ļ�str_article��ʧ�ܣ�\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("�ļ�bit_article��ʧ�ܣ�\n");
        exit(0);
    }
    while ((getc(fp1)) != EOF) word_num++; //ͳ��01�ַ��ĸ���
    rewind(fp1); //ʹ�ļ�ָ��ص��ļ�ͷ���Թ�����ɨ���ļ�
    if (word_num % 8 != 0) //���01���������һ��δ��8��
    {
        for (i = 0; i < 8 - word_num % 8; i++)
            fprintf(fp1, "0");    //��0����8��
        fclose(fp1);     //���ļ��޸ĺ�Ҫ�ȹر��ļ����ܱ������ݣ������޸���Ч
        fp1 = fopen("str_article.txt", "r"); //���´��ļ�
        if (fp1 == NULL) {
            printf("�ļ�str_article��ʧ�ܣ�\n");
            exit(0);
        }
    }
    for (i = 0; i <= word_num / 8; i++) {
        num = 0;
        for (j = 0; j < 8; j++)    //ÿ8��01һ��д��������ļ�
        {
            num += (a = getc(fp1) - '0') * (int) pow(2, j);
        }
        fwrite(&num, sizeof(char), 1, fp2);
    }
    printf("�ļ�ѹ���ɹ���\n\n");
    rewind(fp1);
    rewind(fp2);
    fclose(fp1);
    fclose(fp2);
}

//��ѹ�ļ�
void TransferToArticle(HTNODE *T) {
    FILE *fp1, *fp2;
    int i = 0;
    unsigned char n, d;
    fp1 = fopen("bit_article.bin", "rb"); //��ȡ�������ļ��á�rb��
    fp2 = fopen("str_article.txt", "w");
    if (fp1 == NULL) {
        printf("�ļ�bit_article��ʧ�ܣ�\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("�ļ�str_article��ʧ�ܣ�\n");
        exit(0);
    }
    while (fread(&n, sizeof(char), 1, fp1)) {
        for (i = 0; i < 8; i++) {
            fprintf(fp2, "%c", d = n % 2 + 48); //��ԭ01�ַ��ļ�
            n = n / 2;
        }
    }
    rewind(fp1);
    rewind(fp2);
    fclose(fp1);
    fclose(fp2);
    DecodeArticle(T);//��ѹ���ļ�
}

//��ԭ�ļ�
void DecodeArticle(HTNODE *T) {
    FILE *fp1, *fp2;
    int i;
    unsigned char flag;
    fp1 = fopen("str_article.txt", "r");
    fp2 = fopen("decode_article.txt", "w");
    if (fp1 == NULL) {
        printf("�ļ�str_article��ʧ�ܣ�\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("�ļ�decode_article��ʧ�ܣ�\n");
        exit(0);
    }
    i = 2 * N - 2;
    while (word_num > 0)  //�Ը����£���ȡ��0��Ϊ����������1��Ϊ��������ֱ����Ҷ�ڵ㣬�����Ҷ�ڵ�����Ԫ��
    {
        fscanf(fp1, "%1c", &flag);
        word_num--;
        if (flag == '0' && T[i].lchild != -1) {
            i = T[i].lchild;
            if (T[i].lchild == -1 && T[i].rchild == -1) {
                fprintf(fp2, "%c", i);
                i = 2 * N - 2;       //���´Ӹ������±���
            }
        } else if (flag == '1' && T[i].rchild != -1) {
            i = T[i].rchild;
            if (T[i].lchild == -1 && T[i].rchild == -1) {
                fprintf(fp2, "%c", i);
                i = 2 * N - 2;  //���´Ӹ������±���
            }
        }
    }
    printf("�ļ���ѹ�ɹ���\n\n");
    fclose(fp1);
    fclose(fp2);
}
