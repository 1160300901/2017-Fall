#include <stdio.h>
#include <stdlib.h>
#include <time.h> /*�õ���time����*/
#include <iostream>

#define MAX 400
using namespace std;
int SortNum1 = 0, SortNum2 = 0;

//ð������
void swap(int &x, int &y) {
    int t;
    t = x;
    x = y;
    y = t;
}

void BubbleSort(int n, int number[])//�����ѭ���Ż�
{

    for (int i = 1; i <= n - 1; i++) { //һ��Ҫ����n-1��
        int sp = 0; //ÿ�������־λ��Ҫ����Ϊ0���ж��ڲ�ѭ���Ƿ����˽���
        for (int j = n; j >= i + 1; j--) {//ѡ�������������Сֵǰ�ƣ��ڲ�ѭ�����Ż�
            if (number[j] < number[j - 1]) {
                swap(number[j], number[j - 1]);
                sp = 1; //ֻҪ�з���������sp����Ϊ1
                SortNum1++;
            }
        }
        if (sp == 0) { //����־λΪ0��˵������Ԫ���Ѿ����򣬾�ֱ�ӷ���
            return;
        }
    }
}

//��������
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
        prvotloc = partions(l, low, high);    //����һ������Ľ����Ϊ����
        qsort(l, low, prvotloc - 1); //�ݹ�������� ��low ��prvotloc-1
        qsort(l, prvotloc + 1, high); //�ݹ�������� �� prvotloc+1�� high
    }
}

void quicksort(int l[], int n) {
    qsort(l, 1, n); //��һ����Ϊ���� ���ӵ�һ���ŵ���n��
}

//�۰���ҷ��ķǵݹ���ʽ
int Search_Bin1(int number[], int x, int n) {
    int low = 1;
    int high = n;     // �������ֵ
    int mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x == number[mid])
            return mid;       // �ҵ�����Ԫ��
        else if (x < number[mid])
            high = mid - 1;   // ������ǰ��������в���
        else low = mid + 1;  // �����ں��������в���
    }
    return 0;
}


//�۰���ҷ��ĵݹ���ʽ
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
    printf("************ ���ҽṹ�����򷽷���ʵ����ʵ��Ƚ� ************\n");
    SortNum1 = 0;
    SortNum2 = 0;
    int n, m, i,j;
    cout << "������������ĸ�����";
    cin >> n;
    int number[n] = {0};
    int number1[n] = {0};
    cout << "��ѡ�����ݷֲ����ͣ�����1��/����2��/����3����";
    cin >> m;
    if (m == 1) {
        for (i = 0; i < n; i++)
            number[i] = number1[i] = i;
    } else if (m == 2) {
        for (i = n - 1,j=0; i >= 0,j<n; i--,j++)
            number[j] = number1[j] = i;
    } else if (m == 3) {
        srand((unsigned) time(NULL)); /*������*/
        for (i = 0; i < n; i++) {
            number[i] = rand() % 20000; /*����20000���ڵ��������*/
            number1[i] = number[i];
            while (number[i] == 0) {
                number[i] = rand() % 20000;
                number1[i] = number[i];
            }
        }
    }

    //�������򲢼���ʱ��
    clock_t begin1, end1;
    double cost1;
    begin1 = clock();
    BubbleSort(n, number);
    end1 = clock();
    cost1 = (double) (end1 - begin1) * 1000 / CLOCKS_PER_SEC;

    //ð�����򲢼���ʱ��
    clock_t begin2, end2;
    double cost2;
    begin2 = clock();
    quicksort(number1, n);
    end2 = clock();
    cost2 = (double) (end2 - begin2) * 1000 / CLOCKS_PER_SEC;

    printf("�������!\n");
    cout << "ð�����򽻻��Ĵ���:" << SortNum1 << endl;
    cout << "�������򽻻��Ĵ���:" << SortNum2 << endl;
    cout << "ð�������ʱ:" << cost1 << "ms" << endl;
    cout << "���������ʱ:" << cost2 << "ms" << endl;
    printf("������Ҫ���ҵ�ֵ��\n");
    int find;
    scanf("%d", &find);
    //�ǵݹ�ʱ��
    clock_t begin3, end3;
    double cost3;
    begin3 = clock();
    for ( j = 0; j < 1000000; j++) {
        Search_Bin1(number, find, n);
    }
    end3 = clock();
    cost3 = (double) (end3 - begin3) * 1000 / CLOCKS_PER_SEC;

    //�ݹ�ʱ��
    clock_t begin4, end4;
    double cost4;
    begin4 = clock();
    for ( j = 0; j < 1000000; j++) {
        BinSearch2(number, 0, n - 1, find);
    }
    end4 = clock();
    cost4 = (double) (end4 - begin4) * 1000 / CLOCKS_PER_SEC;
    cout << "�ǵݹ���ֲ��Һ�ʱ:" << cost3 << "ms" << endl;
    cout << "�ݹ���ֲ��Һ�ʱ:" << cost4 << "ms" << endl;
    return 0;
}
