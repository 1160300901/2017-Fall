#include <iostream>
#include <afxres.h>
#include <cstdio>

/* --------------------平衡二叉树的结构体------------------*/
typedef int elementType;
typedef struct AVLnode {
    elementType data;
    struct AVLnode *left;
    struct AVLnode *right;
    int height;  //以此节点为根，树的高度；
    unsigned int freq;//此节点保存的数据出现的频率
}  *PtrToNode;

/* ------------------------树的高度------------------------*/
//只有一个根节点的高度为0，空树的高度-1
int NodeHeight(PtrToNode ptrTree) {
    return ptrTree == NULL ? -1 : ptrTree->height;
}

int max(int a, int b) {
    return a < b ? b : a;
}

/* ------------------------LL旋转------------------------*/
void LLrotate(PtrToNode &k1) {
    PtrToNode k = k1->left;  //保存节点的左子树
    k1->left = k->right;   //k节点的右子树作为k1的左子树
    k->right = k1;          //把k1的k的右子树
    //更新树的深度
    k1->height = max(NodeHeight(k1->left), NodeHeight(k1->right)) + 1;
    k->height = max(NodeHeight(k->left), NodeHeight(k->right)) + 1;
    k1 = k;
}

/* ------------------------RR旋转------------------------*/
void RRrotate(PtrToNode &k2) {
    PtrToNode k = k2->right;  //保存节点的右子树
    k2->right = k->left;
    k->left = k2;
    //更新树的深度
    k2->height = max(NodeHeight(k2->left), NodeHeight(k2->right)) + 1;
    k->height = max(NodeHeight(k->left), NodeHeight(k->right)) + 1;
    k2 = k;
}

/* ------------------------LR旋转------------------------*/
void LRrotate(PtrToNode &k3) {
    RRrotate(k3->left);
    LLrotate(k3);
    printf("进行的旋转为：LR\n");
}

/* ------------------------RL旋转------------------------*/
void RLrotate(PtrToNode &k3) {
    LLrotate(k3->right);
    RRrotate(k3);
    printf("进行的旋转为：RL\n");
}

/* -----------------------左平衡处理---------------------*/
void LeftBalance(PtrToNode &node) {
    PtrToNode ptrTmp = node->left;
    if (NodeHeight(ptrTmp->left) - NodeHeight(ptrTmp->right) == -1) {
        //右子树高于左子树，在右子树插入的
        LRrotate(node); //LR
    } else {
        LLrotate(node); //LL
        printf("进行的旋转为：LL\n");
    }
}

/* -----------------------右平衡处理---------------------*/
void RightBalance(PtrToNode &node) {
    PtrToNode ptrTmp = node->right;
    if (NodeHeight(ptrTmp->right) - NodeHeight(ptrTmp->left) == -1) {
        //左子树比右子树高，说明在左子树插入的
        RLrotate(node); //RL
    } else {
        RRrotate(node);  //RR
        printf("进行的旋转为：RR\n");
    }
}

/* -----------------------插入操作-----------------------*/
void AVL_Insert(PtrToNode &node, elementType x) {
    if (NULL == node) //找到插入的节点的位置
    {
        node = (struct AVLnode *) malloc(sizeof(struct AVLnode));
        node->data = x;
        node->height = 0;
        node->freq = 1;
        node->left = NULL;
        node->right = NULL;
    } else if (x < node->data)  //在左子树插入
    {
        AVL_Insert(node->left, x);
        //判断是否破坏AVL树的平衡性
        if (NodeHeight(node->left) - NodeHeight(node->right) == 2)
            LeftBalance(node);  //左平衡处理
    } else if (node->data < x)   //在右子树插入
    {
        AVL_Insert(node->right, x);
        //判断是否破坏AVL树的平衡性
        if (NodeHeight(node->right) - NodeHeight(node->left) == 2)
            RightBalance(node); //右平衡处理
    } else {
        node->freq++;
    }
    node->height = max(NodeHeight(node->left), NodeHeight(node->right)) + 1;  //跟新树的高度
}

/* -----------------------查找操作-----------------------*/
PtrToNode AVL_Find(PtrToNode &node, elementType x) {
    if (node == NULL)  //没找到元素
    {
        return NULL;
    } else if (x < node->data) {
        return AVL_Find(node->left, x); //在左子树里面查找
    } else if (node->data < x) {
        return AVL_Find(node->right, x); //在右子树里面查找
    } else //相等
        return node;
}

/* -----------------------删除操作-----------------------*/
void AVL_Delete(PtrToNode &node, elementType x) {
    if (NULL == node)  //空树直接返回
        return;
    if (x < node->data)  //在左子树中查找
    {
        AVL_Delete(node->left, x);
        if (NodeHeight(node->right) - NodeHeight(node->left) == 2)  //树左平衡处理
            RightBalance(node);
    } else if (node->data < x)  //在右子树中查找
    {
        AVL_Delete(node->right, x);
        if (NodeHeight(node->left) - NodeHeight(node->right) == 2)  //树右平衡处理
            LeftBalance(node);
    } else //找到要删除的元素节点
    {
        if (node->left == NULL) //左子树为空
        {
            PtrToNode ptrTmp = node;
            node = node->right;         //用右孩子代替此节点
            free(ptrTmp);            //释放内存
        } else if (node->right == NULL)  //右子树为空
        {
            PtrToNode ptrTmp = node;
            node = node->left;       //用左孩子代替此节点
            free(ptrTmp);
        } else   //左右子树都不为空
        {
            //左子树的最小数据代替该节点
            PtrToNode ptrTmp = node->left;  //从左子树中查找
            while (ptrTmp->right != NULL) ptrTmp = ptrTmp->right;
            //此时的ptrTmp指向左子树中的最大元素
            node->data = ptrTmp->data;
            AVL_Delete(node->left, ptrTmp->data);  //递归的删除该节点
        }
    }
    //更新节点的高度
    if (node)
        node->height = max(NodeHeight(node->left), NodeHeight(node->right));
}

/* -----------------------中序遍历-----------------------*/
void In_traverse(PtrToNode &root) {
    if (NULL == root) {
        return;
    }
    In_traverse(root->left);
    printf("%d ", root->data);
    In_traverse(root->right);
}

/* -----------------------打印树-----------------------*/
void OutputAVLTree(PtrToNode T) {
    if (T == NULL) return;
    else {
        printf("[ ");
        OutputAVLTree(T->left);
        printf(" %d ", T->data);
        OutputAVLTree(T->right);
        printf("] ");
    }
}

int main() {
/* -----------------------从文件建AVL树-----------------------*/
    FILE *fp;
    int n, k;
    PtrToNode root = NULL;
    int choice = 1;
    while (choice) {
        printf("************ AVL 树 算 法 的 实 现 ************\n");
        printf("\t1.从文件建立AVL树并排序\n");
        printf("\t2.排序（中序遍历）\n");
        printf("\t3.查找操作\n");
        printf("\t4.插入操作\n");
        printf("\t5.删除操作\n");
        printf("\t0.退出\n");
        printf("\n请输入0-5,选择要执行的操作:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                fp = fopen("AVLtree.txt", "r");
                if (fp == NULL) {
                    printf("文件打开失败！\n");
                    exit(0);
                }
                fscanf(fp, "%6d", &n);
                for (int i = 0; i < n; i++) {
                    fscanf(fp, "%6d", &k);
                    AVL_Insert(root, k);
                    printf("插入的数据为：%d \n", k);
                }
                printf("当前的AVL树：");
                OutputAVLTree(root);
                printf("\n");

            }
                system("pause");
                break;
            case 2: {
                printf("中序遍历：");
                In_traverse(root);
                printf("\n");
            }
                system("pause");
                break;
            case 3: {
                printf("请输入要查找的值：\n");
                int F;
                scanf("%d", &F);
                PtrToNode y = AVL_Find(root, F);
                if (y == NULL) {
                    printf("没有查找到%d\n", F);
                } else {
                    printf("所在节点的高度：%d\n", y->height);
                    if (NULL != y->left) {
                        printf("所在节点的左孩子：%d\n", y->left->data);
                    }
                    if (NULL != y->right) {
                        printf("所在节点的右孩子：%d\n", y->right->data);
                    }
                }

                printf("\n");
            }
                system("pause");
                break;
            case 4: {
                printf("当前的AVL树：");
                OutputAVLTree(root);
                printf("\n");
                printf("请输入要插入的值：\n");
                int m;
                scanf("%d", &m);
                AVL_Insert(root, m);
                printf("当前的AVL树：");
                OutputAVLTree(root);
                printf("\n");
            }
                system("pause");
                break;
            case 5: {
                printf("当前的AVL树：");
                OutputAVLTree(root);
                printf("\n");
                printf("请输入要删除的值：\n");
                int D;
                scanf("%d", &D);
                AVL_Delete(root, D);
                printf("当前的AVL树：");
                OutputAVLTree(root);
                printf("\n");
            }
                system("pause");
                break;
            case 0:
                break;
        }
        system("CLS");
    }
}
