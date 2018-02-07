#include <iostream>
#include <afxres.h>
#include <cstdio>
#include <time.h>

using namespace std;
#define MAX_LEN 20
#define OK 1
#define OVERFLOW -2
typedef int status;
typedef enum {
    Link, Thread
} PointerTag;//Link==0:ָ�룬Thread==1:����
typedef struct node {   //�������ṹ��
    char data;  //����
    struct node *lchild;  //���Ҷ���
    struct node *rchild;
    bool ltag, rtag;
} BiThrNode, *BiTree, *BiThrTree;

struct node *s[MAX_LEN]; /* ����ָ�����飬��Ŷ��������ָ�� */

BiTree Queue[MAX_LEN + 1];//����ջ�Ͷ��У��洢���ڵ��ַ

struct Stack1 {
    BiTree root;
    int flag;
};
struct Stack1 s1[MAX_LEN];//����ջ�����ں���ǵݹ����
BiThrTree pre;//���ǰ��
BiThrTree i;//ͷ���
char PostOrder[MAX_LEN];
char Tree[MAX_LEN]; //����Ԫ�ص�����
static int S; //�����±�
int A = 0, B = 0; //�ۼӵݹ����
DWORD start1, end1, start2, end2; //��ʱ
double dur1, dur2;//��ʱ

status Visit(char e) {
    printf(" %c ", e);
    return OK;
}

/*  ���������н������������������洢�ṹ,����һ ,ABDH##I##E##CF#J##G##���У�#��ʾ�� */
status CreateBT(BiTree &T) {
    char ch;
    ch = Tree[S++];
    if (ch == '#') T = NULL;
    else {
        T = new node;
        T->data = ch;
        if (CreateBT(T->lchild)) T->ltag = Link;
        if (CreateBT(T->rchild)) T->rtag = Link;
    }
    return OK;
}

/*  ���������������������������洢�ṹ�ķǵݹ��㷨. */
BiTree CreateBT2() {
    int i, j;
    char ch;
    struct node *bt, *p;  /* btΪ����p ���ڽ������ */
    cin >> i >> ch;
    while (i != 0 && ch != 0) {
        p = new node;
        p->data = ch;
        p->lchild = NULL;
        p->rchild = NULL;
        s[i] = p;
        if (i == 1) bt = p;
        else {
            j = i / 2; /* �����ı�� */
            if (i % 2 == 0) s[j]->lchild = p; /* i �� j ������� */
            else s[j]->rchild = p; /* i �� j ���Ҷ��� */
        }
        cin >> i >> ch;
    }
}

//����ݹ����
void Recur_PreOrder(BiTree BT) {
    if (BT != NULL) {
        cout << BT->data << ' ';

        Recur_PreOrder(BT->lchild);
        Recur_PreOrder(BT->rchild);
    }
}

//����ݹ����
void Recur_InOrder(BiTree BT) {
    if (BT != NULL) {
        Recur_InOrder(BT->lchild);
        cout << BT->data << ' ';
        A++;
        Recur_InOrder(BT->rchild);
    }
}

//����ݹ����
void Recur_PostOrder(BiTree BT) {
    if (BT != NULL) {
        Recur_PostOrder(BT->lchild);
        Recur_PostOrder(BT->rchild);
        cout << BT->data << ' ';
    }
}

//��������ǵݹ��㷨
void NoRecur_PreOrder(BiTree root) {
    int top = -1; //����˳��ջ�����ٶ����ᷢ������
    while (root != NULL || top != -1) {
        while (root != NULL) {
            cout << root->data << ' ';
            s[++top] = root;
            root = root->lchild;
        }
        if (top != -1) {
            root = s[top--];
            root = root->rchild;
        }
    }
}

//��������ǵݹ��㷨
void NoRecur_InOrder(BiTree root) {
    int top = -1; //����˳��ջ�����ٶ����ᷢ������
    while (root != NULL || top != -1) {
        while (root != NULL) {
            s[++top] = root;
            root = root->lchild;
        }
        if (top != -1) {
            root = s[top--];
            cout << root->data << ' ';
            root = root->rchild;
        }
    }
}

//��������ǵݹ��㷨
void NoRecur_PostOrder(BiTree root) {
    int top = -1;
    while (root != NULL || top != -1) //ѭ��ֱ��ջ�͸�Ϊ��
    {
        while (root != NULL) //root �ǿ�ʱѭ��
        {
            top++;
            s1[top].root = root;  //root ��ͬflag = 1��ջ
            s1[top].flag = 1;
            root = root->lchild; //��������������
        }
        while (top != -1 && s1[top].flag == 2) //��ջ��Ԫ�طǿ��ұ�־λΪ2ʱ�����ջ�����
            cout << s1[top--].root->data << ' ';
        if (top != -1) //ջ���ǿ�
        {
            s1[top].flag = 2; //ջ����־��2
            root = s1[top].root->rchild; //����������
        }
    }
}

//�������
void LeverOrder(BiTree root) {
    int Front = 0, Rear = 0;//���ö���
    if (root == NULL)
        return;
    Queue[++Rear] = root;//���ڵ����
    while (Front != Rear)//�������л���Ԫ��ʱ
    {
        root = Queue[++Front];//���ڵ����
        cout << root->data << ' ';
        if (root->lchild != NULL)//���ڵ��������ǿ�
            Queue[++Rear] = root->lchild;//�������ջ
        if (root->rchild != NULL) //���ڵ��������ǿ�
            Queue[++Rear] = root->rchild;//�Ҷ�����ջ
    }
}

//����������
void InThreading(BiThrTree p) {
    if (p) {
        InThreading(p->lchild);
        if (!p->lchild) {
            p->ltag = Thread;
            p->lchild = pre;
        }
        if (!pre->rchild) {
            pre->rtag = Thread;
            pre->rchild = p;
        }
        pre = p;
        InThreading(p->rchild);
    }
}

//�������������T��������������������Thrtָ��ͷ���
void InOrderThreading(BiThrTree &Thrt, BiThrTree T) {
    Thrt = new node;
    Thrt->ltag = Link;//��ͷ���
    Thrt->rtag = Thread;
    Thrt->rchild = Thrt;//��ָ���ָ
    if (!T)
        Thrt->lchild = Thrt;    //��������Ϊ�գ�����ָ���ָ
    else {
        Thrt->lchild = T;
        pre = Thrt;
        InThreading(T);//���������������������
        pre->rchild = Thrt;
        pre->rtag = Thread;//���һ�����������
        Thrt->rchild = pre;
    }
    i = Thrt;
}


//����������
void PreThreading(BiThrTree p) {
    if (p) {
        if (!p->lchild) {
            p->ltag = Thread;
            p->lchild = pre;
        }
        if (!pre->rchild) {
            pre->rtag = Thread;
            pre->rchild = p;
        }
        pre = p;
        Visit(p->data);

        if (p->ltag == Link)
            PreThreading(p->lchild);
        if (p->rtag == Link)
            PreThreading(p->rchild);
    }
}//PreThreading

//�������������T��������������������Thrtָ��ͷ���
void PreOrderThreading(BiThrTree &Thrt, BiThrTree T) {
    if (!(Thrt = (BiThrTree) malloc(sizeof(BiThrNode)))) exit(OVERFLOW);
    Thrt->ltag = Link;
    Thrt->rtag = Thread;//��ͷ���
    Thrt->rchild = Thrt;//��ָ���ָ
    if (!T)
        Thrt->lchild = Thrt;
    else {
        Thrt->lchild = T;
        pre = Thrt;
        PreThreading(T);//���������������������
        pre->rchild = Thrt;
        pre->rtag = Thread;//���һ�����������
        Thrt->rchild = pre;
    }
    i = Thrt;
}

//�������ģ������������㷨-------�ݹ��㷨
void InOrderTh(BiThrTree p) {   //�������� p����������
    if (p) { //p �ǿ�ʱ����ǰ���ʵĽ���� p
        InOrderTh(p->lchild); //�ݹ��������������
        p->ltag = (p->lchild) ? TRUE : FALSE; //��(��)���ӷǿ�
        p->rtag = (p->rchild) ? TRUE : FALSE; //ʱ,��־1,��: 0
        if (pre) { //��*p ��ǰ��*pre ����
            if (pre->rtag == FALSE) // *p��ǰ���ұ�־Ϊ����
                pre->rchild = p; // �� *pre ��������ָ��������
            if (p->ltag == FALSE) // *p�����־Ϊ����
                p->lchild = pre; //�� *p��������ָ������ǰ��
        }
        pre = p; // ��pre ����һ�����ʵ�����ǰ��
        InOrderTh(p->rchild); //�ݹ��������������
    }
}

//�����������������н��p ������˳��ĺ�̽��p*
BiThrTree PreNext(BiThrTree p) {
    BiThrTree Q;
    if (p->ltag == FALSE)
        Q = p->lchild; //(1)
    else {
        Q = p; //(3)
        while (Q->rtag == TRUE)
            Q = Q->rchild;
        Q = Q->rchild; //(2)
    }
    return (Q);
}

//�����������������н��p ������˳��ĺ�̽��p*
BiThrTree InNext(BiThrTree p) {
    BiThrTree Q;
    Q = p->rchild;
    if (p->rtag == TRUE)
        while (Q->ltag == TRUE)
            Q = Q->lchild;
    return (Q);
}

//�����������������н��p �ĺ���˳��ĺ�̽��p*
char PostNext(BiThrTree Thre, char data)//�����������������
{
    BiThrTree p;
    p = Thre->lchild;
    int num = MAX_LEN - 1;
    while (p != Thre) {
        if (p->data == data)
            return PostOrder[num + 1];
        PostOrder[num] = p->data;
        num--;
        while (p->rtag == Link) {
            p = p->rchild;
            if (p->data == data)
                return PostOrder[num + 1];
            PostOrder[num] = p->data;
            num--;
        }
        if (p->ltag == Thread && p->lchild != Thre) {
            p = p->lchild;
        }
        if (p->ltag == Link && p->lchild != Thre) {
            p = p->lchild;
        } else if (p->lchild != Thre) {
            while (p->ltag == Thread) {
                p = p->lchild;
                if (p == Thre)
                    break;
            }
            p = p->lchild;
        } else {
            p = Thre;
        }
    }
    return num;
}


void OutputBiTree(BiTree T) {
    if (T == NULL) return;
    else {
        printf("[ ");
        OutputBiTree(T->lchild);
        printf(" %c ", T->data);
        OutputBiTree(T->rchild);
        printf("] ");
    }
}

//���ҽ�㣬�����ؽ��ָ��
BiThrTree LeverOrderFind(BiTree root, char data) {
    int Front = 0, Rear = 0;//���ö���
    if (root == NULL)
        return NULL;
    Queue[++Rear] = root;//���ڵ����
    while (Front != Rear)//�������л���Ԫ��ʱ
    {
        root = Queue[++Front];//���ڵ����
        if (root->data == data) {
            //cout << root->data << ' ';
            return root;
        }

        if (root->lchild != NULL)//���ڵ��������ǿ�
            Queue[++Rear] = root->lchild;//�������ջ
        if (root->rchild != NULL) //���ڵ��������ǿ�
            Queue[++Rear] = root->rchild;//�Ҷ�����ջ
    }
}

//����ǵݹ��������������������
void PreThrTravel(BiThrTree Thre) {
    BiThrTree p;
    p = Thre->lchild;
    while (p != Thre) {
        printf("%c ", p->data);
        while (p->ltag == Link) {
            p = p->lchild;
            printf("%c ", p->data);
        }
        if (p->rtag == Thread) {
            p = p->rchild;
        }
        if (p->rtag == Link) {
            p = p->rchild;
        }
    }
    printf("\n");
}//

//����ǵݹ��������������������
void InThrTravel(BiThrTree Thre)//�����������������
{
    BiThrTree p;
    p = Thre->lchild;
    while (p->ltag == FALSE) {
        p = p->lchild;
    }

    while (p != Thre) {
        B++;
        printf("%c ", p->data);
        if (p->rtag == FALSE) {
            p = p->rchild;
            while (p->ltag == FALSE) {
                B++;
                p = p->lchild;
            }
        } else {
            p = p->rchild;
        }
    }

    printf("\n");
}

//����ǵݹ��������������������
int PostThrTravel(BiThrTree Thre)//�����������������
{
    BiThrTree p;
    p = Thre->lchild;
    int num = MAX_LEN - 1;
    while (p != Thre) {
        //printf("%c ", p->data);
        PostOrder[num] = p->data;
        num--;
        while (p->rtag == Link) {
            p = p->rchild;
            //printf("%c ", p->data);
            PostOrder[num] = p->data;
            num--;
        }
        if (p->ltag == Thread && p->lchild != Thre) {
            p = p->lchild;
        }
        if (p->ltag == Link && p->lchild != Thre) {
            p = p->lchild;
        } else if (p->lchild != Thre) {
            while (p->ltag == Thread) {
                p = p->lchild;
                if (p == Thre)
                    break;
            }
            p = p->lchild;
        } else {
            p = Thre;
        }
    }
    return num;
}

int main() {
    BiTree T, test;
    BiThrTree Thrt1, Thrt2, Node;
    char data;
    int N, m = 0, I = 0;
    FILE *fp = fopen("test.txt", "r");
    if (fp == NULL) {
        printf("Can' t open the %s\n", "test.txt");
        exit(0);
    }
    while (fscanf(fp, "%c", &Tree[I]) != EOF)
        I++;
    fclose(fp);
    S = 0;
    CreateBT(T);
    printf("\n���ļ���ȡ�ַ���������������\n");
    OutputBiTree(T);
    printf("\n");
    cout << "\n����ݹ���������" << endl;
    Recur_PreOrder(T);
    cout << endl;


    cout << "\n����ǵݹ���������" << endl;
    NoRecur_PreOrder(T);
    cout << endl;

    cout << "\n����ݹ���������" << endl;
    Recur_InOrder(T);
    cout << endl;
    //printf("�ݹ������%d\n", A);
    cout << "\n����ǵݹ���������" << endl;

    start1 = GetTickCount();
    NoRecur_InOrder(T);
    end1 = GetTickCount();
    dur1 = 10000 * (double) (end1 - start1) / CLK_TCK;
    cout << endl;
    //printf("����ǵݹ�ʱ�䣺%f",dur1);
    cout << endl;

    cout << "\n����ݹ���������" << endl;
    Recur_PostOrder(T);
    cout << endl;

    cout << "\n����ǵݹ���������" << endl;
    NoRecur_PostOrder(T);
    cout << endl;

    cout << "\n������������" << endl;
    LeverOrder(T);
    cout << endl;
    printf("\n");
    InOrderThreading(Thrt1, T);//������

    printf("\n������������������Ķ������ķǵݹ�����\n");
    PreThrTravel(i);

    printf("\n������������������Ķ������ķǵݹ�����\n");
    start2 = GetTickCount();
    InThrTravel(i);
    end2 = GetTickCount();
    dur2 = 100000 * ((double) (end2 - start2)) / CLK_TCK;
    //printf("����ǵݹ��������������ʱ�䣺%f\n",dur2);
    //printf("��������������%d\n", B);
    printf("\n������������������Ķ������ķǵݹ�����\n");
    N = PostThrTravel(Thrt1);
    for (int i = N + 1; i < MAX_LEN; i++)
        printf("%c ", PostOrder[i]);
    printf("\n");

    printf("\n������,�����������������������ϵ��������򡢺���ĺ�̽�㣺\n");
    cin >> data;
    Node = LeverOrderFind(T, data);
    printf("\n�����������������ϣ�%c �������̽��Ϊ%c\n", data, PreNext(Node)->data);

    printf("\n�����������������ϣ�%c �������̽��Ϊ%c\n", data, InNext(Node)->data);

    printf("\n�����������������ϣ�%c �ĺ������̽��Ϊ%c\n", data, PostNext(Thrt1, data));

    printf("\n�����������������������:");
    PreOrderThreading(Thrt2, T);
    printf("\n");
    return 0;
}
