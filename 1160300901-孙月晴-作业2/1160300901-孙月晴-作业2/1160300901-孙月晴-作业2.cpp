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
    int weight; //权重
    int lchild;   //左子树
    int rchild;  //右子树
    int parent;  //父节点
} HTNODE;
typedef struct {
    char ch;  //被编码的字符
    char bits[N + 1]; //字符编码位串
} CodeNode;
CodeNode HuffmanCode[N];
HTNODE HuffmanT[2 * N - 1];

void CreatHT(HTNODE *T);   //建立哈弗曼树
void InitHT(HTNODE *T);    //初始化哈弗曼树
void InputW(HTNODE *T);   //统计输入权重
void SelectMin(HTNODE *T, int n, int *p1, int *p2);  //选取两个权重最小的根节点
void CharSetHuffmanEncoding(HTNODE *T, CodeNode *H); //构建哈夫曼编码
void WriteToFile(HTNODE *T, CodeNode *H);  //将哈夫曼编码写入文件
void TransferToBit();  //转化为二进制文件
void TransferToArticle(HTNODE *T); //译码
void DecodeArticle(HTNODE *T);
double PressRate();
int flag = 0, len = 0;
int article_num = 0;
int word_num = 0;
using namespace std;

int main() {
    char flag;     //定义标志量
    CreatHT(HuffmanT); //建立哈弗曼树
    CharSetHuffmanEncoding(HuffmanT, HuffmanCode); //产生哈夫曼编码
    WriteToFile(HuffmanT, HuffmanCode); //将哈夫曼编码写入文件
    int choice=1;
    while (choice) {
        printf("************ 哈 夫 曼 编 码 与 译 码 的 实 现 ************\n");
        printf("\t1.各字符的使用频率\n");
        printf("\t2.字符集的哈夫曼编码表\n");
        printf("\t3.进行文件压缩，计算压缩率\n");
        printf("\t4.文件解压\n");
        printf("\t0.退出编码译码器\n");
        printf("\n请输入0-4,选择要执行的操作:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                printf("--------------------------------------\n");
                int i = 0;
                printf("\t 字符 \t 出现频率 \n");
                while (i < N) {
                    if (HuffmanT[i].weight != 0) {
                        if (i == 10) {
                            printf("\t %c   (换行符)\t %d\n", i, HuffmanT[i].weight);
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
                printf("\t 字符 \t 哈夫曼编码 \n");
                while (j < N) {
                    if (HuffmanT[j].weight != 0) {
                        if (j == 10) {
                            printf("\t %c   (换行符)\t %s\n", HuffmanCode[j].ch, HuffmanCode[j].bits);
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
                printf("原文件为：article.txt \n\n哈夫曼编码文件为：str_article.txt \n\n压缩后的文件为：bit_article.bin \n\n");
                TransferToBit();      //将字符串01每8位1组，写入二进制文件中，至此文件压缩完成
                printf("文件压缩率：%.2f",PressRate()*100);
                printf("%%\n\n");
                printf("--------------------------------------\n");

            }
                system("pause");
                break;
            case 4: {
                printf("--------------------------------------\n");
                printf("解压的文件为：bit_article.bin \n\n解压后的文件为：decode_article.txt \n\n");
                TransferToArticle(HuffmanT);  //对文件进行解压，还原文件
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

//计算压缩率
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
        fseek (pFile2, 0, SEEK_END);   ///将文件指针移动文件结尾
        size2=ftell (pFile2); ///求出当前文件指针距离文件开始的字节数
        fclose (pFile2);
    }
    pFile1 = fopen ("article.txt","r");
    if (pFile1==NULL)
        perror ("Error opening file");
    else
    {
        fseek (pFile1, 0, SEEK_END);   ///将文件指针移动文件结尾
        size1=ftell (pFile1); ///求出当前文件指针距离文件开始的字节数
        fclose (pFile1);
    }
    return ((double)(size1-size2)/size1);
}
//构建哈弗曼树
void CreatHT(HTNODE *T) {
    int i, p1, p2;
    InitHT(T);   //初始化哈弗曼树
    InputW(T);   //输入权重
    for (i = N; i < 2 * N - 1; i++)   //N-1次合并
    {
        SelectMin(T, i - 1, &p1, &p2); //选取两个权重最小的
        T[p1].parent = i;  //构建二叉树，根节点为T[i]
        T[p2].parent = i;
        T[i].lchild = p1;
        T[i].rchild = p2;
        T[i].weight = T[p1].weight + T[p2].weight; //父节点的权重为两个子节点权重的和
    }
}

//初始化哈弗曼树
void InitHT(HTNODE *T) {
    int i;
    for (i = 0; i < 2 * N - 1; i++) {
        T[i].weight = 0; //权重初始化为0
        T[i].parent = -1; //父节点为-1置空
        T[i].lchild = -1; //左孩子
        T[i].rchild = -1; //右孩子
    }
}

//读取文件，统计输入权重
void InputW(HTNODE *T) {
    FILE *fp;
    char ch;
    fp = fopen("article.txt", "r"); //打开文件
    if (fp == NULL) {
        printf("article文件读取失败！\n");
        exit(0);
    }
    while (!feof(fp)) {
        fscanf(fp, "%c", &ch);
        article_num++; //统计文章字数
        T[ch].weight++; //对应元素权重加1
    }
    article_num--;   //因!feof(fp)在读到文件尾还要再读一遍文件的最后一个元素才结束，故总字数article_num减一
    T[ch].weight--;  //最后一个元素的权重减一
    rewind(fp);//使文件指针回到文件头
    fclose(fp);  //关闭文件
}

//选取森林中两棵根节点权重最小的树
void SelectMin(HTNODE *T, int n, int *p1, int *p2) {
    int i, j;
    for (i = 0; i < n; i++) {
        if (T[i].parent == -1) //找根节点
        {
            *p1 = i;
            break;
        }
    }
    for (j = i + 1; j <= n; j++) {
        if (T[j].parent == -1) //根节点
        {
            *p2 = j;
            break;
        }
    }
    for (i = 0; i <= n; i++)                   //在n+1个节点中寻找最小值和次小值
    {
        if ((T[*p1].weight > T[i].weight) && (T[i].parent == -1) && (*p2 != i))
            *p1 = i;
    }
    for (j = 0; j <= n; j++) {
        if ((T[*p2].weight > T[j].weight) && (T[j].parent == -1) && (*p1 != j))
            *p2 = j;
    }
}

//构建哈夫曼编码
void CharSetHuffmanEncoding(HTNODE *T, CodeNode *H) {
    int c, p, i;
    char cd[N + 1];
    int start;
    cd[N] = '\0';
    for (i = 0; i < N; i++)  //为N个叶节点构造哈夫曼编码，自叶节点上溯至根节点，左子树编为’0‘，右子树为’1‘
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

//将哈夫曼编码写入文件中，实际上此时是把01字符串写入文件
void WriteToFile(HTNODE *T, CodeNode *H) {
    FILE *fp1, *fp2;
    int i;
    fp1 = fopen("article.txt", "r");
    fp2 = fopen("str_article.txt", "w");
    if (fp1 == NULL) {
        printf("文件1打开失败！\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("文件2打开失败！\n");
        exit(0);
    }
    for (i = 0; i < article_num; i++) {
        fprintf(fp2, "%s", H[fgetc(fp1)].bits); //用每个元素的哈夫曼编码来代替此元素
    }
    rewind(fp1);
    rewind(fp2);
    fclose(fp1);
    fclose(fp2);
}

//将01字符串文件转化为二进制文件，实现真正的压缩
void TransferToBit() {
    FILE *fp1, *fp2;
    int i = 0, j;
    long num = 0;
    unsigned char a;
    fp1 = fopen("str_article.txt", "r");
    fp2 = fopen("bit_article.bin", "wb"); //写二进制文件
    if (fp1 == NULL) {
        printf("文件str_article打开失败！\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("文件bit_article打开失败！\n");
        exit(0);
    }
    while ((getc(fp1)) != EOF) word_num++; //统计01字符的个数
    rewind(fp1); //使文件指针回到文件头，以供下面扫描文件
    if (word_num % 8 != 0) //如果01个数最后有一组未满8个
    {
        for (i = 0; i < 8 - word_num % 8; i++)
            fprintf(fp1, "0");    //用0补齐8个
        fclose(fp1);     //对文件修改后要先关闭文件才能保存内容，否则修改无效
        fp1 = fopen("str_article.txt", "r"); //重新打开文件
        if (fp1 == NULL) {
            printf("文件str_article打开失败！\n");
            exit(0);
        }
    }
    for (i = 0; i <= word_num / 8; i++) {
        num = 0;
        for (j = 0; j < 8; j++)    //每8个01一组写入二进制文件
        {
            num += (a = getc(fp1) - '0') * (int) pow(2, j);
        }
        fwrite(&num, sizeof(char), 1, fp2);
    }
    printf("文件压缩成功！\n\n");
    rewind(fp1);
    rewind(fp2);
    fclose(fp1);
    fclose(fp2);
}

//解压文件
void TransferToArticle(HTNODE *T) {
    FILE *fp1, *fp2;
    int i = 0;
    unsigned char n, d;
    fp1 = fopen("bit_article.bin", "rb"); //读取二进制文件用’rb‘
    fp2 = fopen("str_article.txt", "w");
    if (fp1 == NULL) {
        printf("文件bit_article打开失败！\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("文件str_article打开失败！\n");
        exit(0);
    }
    while (fread(&n, sizeof(char), 1, fp1)) {
        for (i = 0; i < 8; i++) {
            fprintf(fp2, "%c", d = n % 2 + 48); //还原01字符文件
            n = n / 2;
        }
    }
    rewind(fp1);
    rewind(fp2);
    fclose(fp1);
    fclose(fp2);
    DecodeArticle(T);//解压缩文件
}

//还原文件
void DecodeArticle(HTNODE *T) {
    FILE *fp1, *fp2;
    int i;
    unsigned char flag;
    fp1 = fopen("str_article.txt", "r");
    fp2 = fopen("decode_article.txt", "w");
    if (fp1 == NULL) {
        printf("文件str_article打开失败！\n");
        exit(0);
    }
    if (fp2 == NULL) {
        printf("文件decode_article打开失败！\n");
        exit(0);
    }
    i = 2 * N - 2;
    while (word_num > 0)  //自根向下，读取’0‘为左子树，’1‘为右子树，直至到叶节点，输出该叶节点代表的元素
    {
        fscanf(fp1, "%1c", &flag);
        word_num--;
        if (flag == '0' && T[i].lchild != -1) {
            i = T[i].lchild;
            if (T[i].lchild == -1 && T[i].rchild == -1) {
                fprintf(fp2, "%c", i);
                i = 2 * N - 2;       //重新从根节向下遍历
            }
        } else if (flag == '1' && T[i].rchild != -1) {
            i = T[i].rchild;
            if (T[i].lchild == -1 && T[i].rchild == -1) {
                fprintf(fp2, "%c", i);
                i = 2 * N - 2;  //重新从根节向下遍历
            }
        }
    }
    printf("文件解压成功！\n\n");
    fclose(fp1);
    fclose(fp2);
}
