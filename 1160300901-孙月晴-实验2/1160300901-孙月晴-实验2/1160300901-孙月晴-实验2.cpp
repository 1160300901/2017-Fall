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
} PointerTag;//Link==0:指针，Thread==1:线索
typedef struct node {   //建立结点结构体
    char data;  //数据
    struct node *lchild;  //左右儿子
    struct node *rchild;
    bool ltag, rtag;
} BiThrNode, *BiTree, *BiThrTree;

struct node *s[MAX_LEN]; /* 辅助指针数组，存放二叉树结点指针 */

BiTree Queue[MAX_LEN + 1];//定义栈和队列，存储根节点地址

struct Stack1 {
    BiTree root;
    int flag;
};
struct Stack1 s1[MAX_LEN];//定义栈，用于后序非递归遍历
BiThrTree pre;//结点前驱
BiThrTree i;//头结点
char PostOrder[MAX_LEN];
char Tree[MAX_LEN]; //树的元素的数组
static int S; //数组下标
int A = 0, B = 0; //累加递归次数
DWORD start1, end1, start2, end2; //计时
double dur1, dur2;//计时

status Visit(char e) {
    printf(" %c ", e);
    return OK;
}

/*  按先序序列建立二叉树的左右链存储结构,方法一 ,ABDH##I##E##CF#J##G##其中：#表示空 */
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

/*  方法二：建立二叉树的左右链存储结构的非递归算法. */
BiTree CreateBT2() {
    int i, j;
    char ch;
    struct node *bt, *p;  /* bt为根，p 用于建立结点 */
    cin >> i >> ch;
    while (i != 0 && ch != 0) {
        p = new node;
        p->data = ch;
        p->lchild = NULL;
        p->rchild = NULL;
        s[i] = p;
        if (i == 1) bt = p;
        else {
            j = i / 2; /* 父结点的编号 */
            if (i % 2 == 0) s[j]->lchild = p; /* i 是 j 的左儿子 */
            else s[j]->rchild = p; /* i 是 j 的右儿子 */
        }
        cin >> i >> ch;
    }
}

//先序递归遍历
void Recur_PreOrder(BiTree BT) {
    if (BT != NULL) {
        cout << BT->data << ' ';

        Recur_PreOrder(BT->lchild);
        Recur_PreOrder(BT->rchild);
    }
}

//中序递归遍历
void Recur_InOrder(BiTree BT) {
    if (BT != NULL) {
        Recur_InOrder(BT->lchild);
        cout << BT->data << ' ';
        A++;
        Recur_InOrder(BT->rchild);
    }
}

//后序递归遍历
void Recur_PostOrder(BiTree BT) {
    if (BT != NULL) {
        Recur_PostOrder(BT->lchild);
        Recur_PostOrder(BT->rchild);
        cout << BT->data << ' ';
    }
}

//先序遍历非递归算法
void NoRecur_PreOrder(BiTree root) {
    int top = -1; //采用顺序栈，并假定不会发生上溢
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

//中序遍历非递归算法
void NoRecur_InOrder(BiTree root) {
    int top = -1; //采用顺序栈，并假定不会发生上溢
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

//后序遍历非递归算法
void NoRecur_PostOrder(BiTree root) {
    int top = -1;
    while (root != NULL || top != -1) //循环直到栈和根为空
    {
        while (root != NULL) //root 非空时循环
        {
            top++;
            s1[top].root = root;  //root 连同flag = 1入栈
            s1[top].flag = 1;
            root = root->lchild; //继续遍历左子树
        }
        while (top != -1 && s1[top].flag == 2) //当栈顶元素非空且标志位为2时，输出栈顶结点
            cout << s1[top--].root->data << ' ';
        if (top != -1) //栈顶非空
        {
            s1[top].flag = 2; //栈顶标志置2
            root = s1[top].root->rchild; //遍历右子树
        }
    }
}

//层序遍历
void LeverOrder(BiTree root) {
    int Front = 0, Rear = 0;//采用队列
    if (root == NULL)
        return;
    Queue[++Rear] = root;//根节点入队
    while (Front != Rear)//当队列中还有元素时
    {
        root = Queue[++Front];//根节点出队
        cout << root->data << ' ';
        if (root->lchild != NULL)//根节点左子树非空
            Queue[++Rear] = root->lchild;//左儿子入栈
        if (root->rchild != NULL) //根节点右子树非空
            Queue[++Rear] = root->rchild;//右儿子入栈
    }
}

//中序线索化
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

//中序遍历二叉树T，并将其中序线索化，Thrt指向头结点
void InOrderThreading(BiThrTree &Thrt, BiThrTree T) {
    Thrt = new node;
    Thrt->ltag = Link;//建头结点
    Thrt->rtag = Thread;
    Thrt->rchild = Thrt;//右指针回指
    if (!T)
        Thrt->lchild = Thrt;    //若二叉树为空，则左指针回指
    else {
        Thrt->lchild = T;
        pre = Thrt;
        InThreading(T);//中序遍历进行中序线索化
        pre->rchild = Thrt;
        pre->rtag = Thread;//最后一个结点线索化
        Thrt->rchild = pre;
    }
    i = Thrt;
}


//先序线索化
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

//先序遍厉二叉树T，并将其先序线索化，Thrt指向头结点
void PreOrderThreading(BiThrTree &Thrt, BiThrTree T) {
    if (!(Thrt = (BiThrTree) malloc(sizeof(BiThrNode)))) exit(OVERFLOW);
    Thrt->ltag = Link;
    Thrt->rtag = Thread;//建头结点
    Thrt->rchild = Thrt;//右指针回指
    if (!T)
        Thrt->lchild = Thrt;
    else {
        Thrt->lchild = T;
        pre = Thrt;
        PreThreading(T);//先序遍历进行先序线索化
        pre->rchild = Thrt;
        pre->rtag = Thread;//最后一个结点线索化
        Thrt->rchild = pre;
    }
    i = Thrt;
}

//二叉树的（中序）线索化算法-------递归算法
void InOrderTh(BiThrTree p) {   //将二叉树 p中序线索化
    if (p) { //p 非空时，当前访问的结点是 p
        InOrderTh(p->lchild); //递归地线索化左子树
        p->ltag = (p->lchild) ? TRUE : FALSE; //左(右)孩子非空
        p->rtag = (p->rchild) ? TRUE : FALSE; //时,标志1,否: 0
        if (pre) { //若*p 的前驱*pre 存在
            if (pre->rtag == FALSE) // *p的前驱右标志为线索
                pre->rchild = p; // 令 *pre 的右线索指向中序后继
            if (p->ltag == FALSE) // *p的左标志为线索
                p->lchild = pre; //令 *p的左线索指向中序前驱
        }
        pre = p; // 令pre 是下一个访问的中序前驱
        InOrderTh(p->rchild); //递归地线索化右子树
    }
}

//求中序线索二叉树中结点p 的先序顺序的后继结点p*
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

//求中序线索二叉树中结点p 的中序顺序的后继结点p*
BiThrTree InNext(BiThrTree p) {
    BiThrTree Q;
    Q = p->rchild;
    if (p->rtag == TRUE)
        while (Q->ltag == TRUE)
            Q = Q->lchild;
    return (Q);
}

//求中序线索二叉树中结点p 的后序顺序的后继结点p*
char PostNext(BiThrTree Thre, char data)//后序遍历线索二叉树
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

//查找结点，并返回结点指针
BiThrTree LeverOrderFind(BiTree root, char data) {
    int Front = 0, Rear = 0;//采用队列
    if (root == NULL)
        return NULL;
    Queue[++Rear] = root;//根节点入队
    while (Front != Rear)//当队列中还有元素时
    {
        root = Queue[++Front];//根节点出队
        if (root->data == data) {
            //cout << root->data << ' ';
            return root;
        }

        if (root->lchild != NULL)//根节点左子树非空
            Queue[++Rear] = root->lchild;//左儿子入栈
        if (root->rchild != NULL) //根节点右子树非空
            Queue[++Rear] = root->rchild;//右儿子入栈
    }
}

//先序非递归遍历中序线索化二叉树
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

//中序非递归遍历中序线索化二叉树
void InThrTravel(BiThrTree Thre)//中序遍历线索二叉树
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

//后序非递归遍历中序线索化二叉树
int PostThrTravel(BiThrTree Thre)//后序遍历线索二叉树
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
    printf("\n从文件读取字符，建立二叉树：\n");
    OutputBiTree(T);
    printf("\n");
    cout << "\n先序递归遍历结果：" << endl;
    Recur_PreOrder(T);
    cout << endl;


    cout << "\n先序非递归遍历结果：" << endl;
    NoRecur_PreOrder(T);
    cout << endl;

    cout << "\n中序递归遍历结果：" << endl;
    Recur_InOrder(T);
    cout << endl;
    //printf("递归次数：%d\n", A);
    cout << "\n中序非递归遍历结果：" << endl;

    start1 = GetTickCount();
    NoRecur_InOrder(T);
    end1 = GetTickCount();
    dur1 = 10000 * (double) (end1 - start1) / CLK_TCK;
    cout << endl;
    //printf("中序非递归时间：%f",dur1);
    cout << endl;

    cout << "\n后序递归遍历结果：" << endl;
    Recur_PostOrder(T);
    cout << endl;

    cout << "\n后序非递归遍历结果：" << endl;
    NoRecur_PostOrder(T);
    cout << endl;

    cout << "\n层序遍历结果：" << endl;
    LeverOrder(T);
    cout << endl;
    printf("\n");
    InOrderThreading(Thrt1, T);//线索化

    printf("\n先序遍历中序线索化的二叉树的非递归结果：\n");
    PreThrTravel(i);

    printf("\n中序遍历中序线索化的二叉树的非递归结果：\n");
    start2 = GetTickCount();
    InThrTravel(i);
    end2 = GetTickCount();
    dur2 = 100000 * ((double) (end2 - start2)) / CLK_TCK;
    //printf("中序非递归遍历中序线索化时间：%f\n",dur2);
    //printf("遍历次数次数：%d\n", B);
    printf("\n后序遍历中序线索化的二叉树的非递归结果：\n");
    N = PostThrTravel(Thrt1);
    for (int i = N + 1; i < MAX_LEN; i++)
        printf("%c ", PostOrder[i]);
    printf("\n");

    printf("\n输入结点,查找其在中序线索二叉树上的先序、中序、后序的后继结点：\n");
    cin >> data;
    Node = LeverOrderFind(T, data);
    printf("\n在中序线索二叉树上，%c 的先序后继结点为%c\n", data, PreNext(Node)->data);

    printf("\n在中序线索二叉树上，%c 的中序后继结点为%c\n", data, InNext(Node)->data);

    printf("\n在中序线索二叉树上，%c 的后序序后继结点为%c\n", data, PostNext(Thrt1, data));

    printf("\n先序遍历先序线索化二叉树:");
    PreOrderThreading(Thrt2, T);
    printf("\n");
    return 0;
}
