#include <stdio.h>
#include <stdlib.h>
#include <time.h> /*用到了time函数*/
#include <iostream>

#define MAX 400
using namespace std;
int SortNum1 = 0, SortNum2 = 0;

//冒泡排序
void swap(int &x, int &y) {
    int t;
    t = x;
    x = y;
    y = t;
}

void BubbleSort(int n, int number[])//内外层循环优化
{

    for (int i = 1; i <= n - 1; i++) { //一共要排序n-1趟
        int sp = 0; //每趟排序标志位都要先置为0，判断内层循环是否发生了交换
        for (int j = n; j >= i + 1; j--) {//选出该趟排序的最小值前移；内层循环已优化
            if (number[j] < number[j - 1]) {
                swap(number[j], number[j - 1]);
                sp = 1; //只要有发生交换，sp就置为1
                SortNum1++;
            }
        }
        if (sp == 0) { //若标志位为0，说明所有元素已经有序，就直接返回
            return;
        }
    }
}

//快速排序
int partions(int l[], int low, int high) {
    int prvotkey = l[low];
    l[0] = l[low];
    while (low < high) {
        while (low < high && l[high] >= prvotkey)
            --high;
        l[low] = l[high];
        SortNum2++;
        while (low < high && l[low] <= prvotkey)
            ++low;
        l[high] = l[low];
        SortNum2++;
    }
    l[low] = l[0];
    return low;
}

void qsort(int l[], int low, int high) {
    int prvotloc;
    if (low < high) {
        prvotloc = partions(l, low, high);    //将第一次排序的结果作为枢轴
        qsort(l, low, prvotloc - 1); //递归调用排序 由low 到prvotloc-1
        qsort(l, prvotloc + 1, high); //递归调用排序 由 prvotloc+1到 high
    }
}

void quicksort(int l[], int n) {
    qsort(l, 1, n); //第一个作为枢轴 ，从第一个排到第n个
}

//折半查找法的非递归形式
int Search_Bin1(int number[], int x, int n) {
    int low = 1;
    int high = n;     // 置区间初值
    int mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x == number[mid])
            return mid;       // 找到待查元素
        else if (x < number[mid])
            high = mid - 1;   // 继续在前半区间进行查找
        else low = mid + 1;  // 继续在后半区间进行查找
    }
    return 0;
}


//折半查找法的递归形式
int BinSearch2(int number[], int low, int up, int k) {
    int mid;
    if (low > up) return 0;
    else {
        mid = (low + up) / 2;
        if (k < number[mid])
            return BinSearch2(number, low, mid - 1, k);
        else if (k > number[mid])
            return BinSearch2(number, mid + 1, up, k);
        else return mid;
    }
}

int main() {
    printf("************ 查找结构与排序方法的实现与实验比较 ************\n");
    SortNum1 = 0;
    SortNum2 = 0;
    int n, m, i,j;
    cout << "请输入随机数的个数：";
    cin >> n;
    int number[n] = {0};
    int number1[n] = {0};
    cout << "请选择数据分布类型：正序（1）/逆序（2）/乱序（3）：";
    cin >> m;
    if (m == 1) {
        for (i = 0; i < n; i++)
            number[i] = number1[i] = i;
    } else if (m == 2) {
        for (i = n - 1,j=0; i >= 0,j<n; i--,j++)
            number[j] = number1[j] = i;
    } else if (m == 3) {
        srand((unsigned) time(NULL)); /*播种子*/
        for (i = 0; i < n; i++) {
            number[i] = rand() % 20000; /*产生20000以内的随机整数*/
            number1[i] = number[i];
            while (number[i] == 0) {
                number[i] = rand() % 20000;
                number1[i] = number[i];
            }
        }
    }

    //快速排序并计算时间
    clock_t begin1, end1;
    double cost1;
    begin1 = clock();
    BubbleSort(n, number);
    end1 = clock();
    cost1 = (double) (end1 - begin1) * 1000 / CLOCKS_PER_SEC;

    //冒泡排序并计算时间
    clock_t begin2, end2;
    double cost2;
    begin2 = clock();
    quicksort(number1, n);
    end2 = clock();
    cost2 = (double) (end2 - begin2) * 1000 / CLOCKS_PER_SEC;

    printf("排序完成!\n");
    cout << "冒泡排序交换的次数:" << SortNum1 << endl;
    cout << "快速排序交换的次数:" << SortNum2 << endl;
    cout << "冒泡排序耗时:" << cost1 << "ms" << endl;
    cout << "快速排序耗时:" << cost2 << "ms" << endl;
    printf("请输入要查找的值：\n");
    int find;
    scanf("%d", &find);
    //非递归时间
    clock_t begin3, end3;
    double cost3;
    begin3 = clock();
    for ( j = 0; j < 1000000; j++) {
        Search_Bin1(number, find, n);
    }
    end3 = clock();
    cost3 = (double) (end3 - begin3) * 1000 / CLOCKS_PER_SEC;

    //递归时间
    clock_t begin4, end4;
    double cost4;
    begin4 = clock();
    for ( j = 0; j < 1000000; j++) {
        BinSearch2(number, 0, n - 1, find);
    }
    end4 = clock();
    cost4 = (double) (end4 - begin4) * 1000 / CLOCKS_PER_SEC;
    cout << "非递归二分查找耗时:" << cost3 << "ms" << endl;
    cout << "递归二分查找耗时:" << cost4 << "ms" << endl;
    return 0;
}
