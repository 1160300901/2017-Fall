#include <iostream>
#include <afxres.h>
#include <cstdio>

/* --------------------ƽ��������Ľṹ��------------------*/
typedef int elementType;
typedef struct AVLnode {
    elementType data;
    struct AVLnode *left;
    struct AVLnode *right;
    int height;  //�Դ˽ڵ�Ϊ�������ĸ߶ȣ�
    unsigned int freq;//�˽ڵ㱣������ݳ��ֵ�Ƶ��
}  *PtrToNode;

/* ------------------------���ĸ߶�------------------------*/
//ֻ��һ�����ڵ�ĸ߶�Ϊ0�������ĸ߶�-1
int NodeHeight(PtrToNode ptrTree) {
    return ptrTree == NULL ? -1 : ptrTree->height;
}

int max(int a, int b) {
    return a < b ? b : a;
}

/* ------------------------LL��ת------------------------*/
void LLrotate(PtrToNode &k1) {
    PtrToNode k = k1->left;  //����ڵ��������
    k1->left = k->right;   //k�ڵ����������Ϊk1��������
    k->right = k1;          //��k1��k��������
    //�����������
    k1->height = max(NodeHeight(k1->left), NodeHeight(k1->right)) + 1;
    k->height = max(NodeHeight(k->left), NodeHeight(k->right)) + 1;
    k1 = k;
}

/* ------------------------RR��ת------------------------*/
void RRrotate(PtrToNode &k2) {
    PtrToNode k = k2->right;  //����ڵ��������
    k2->right = k->left;
    k->left = k2;
    //�����������
    k2->height = max(NodeHeight(k2->left), NodeHeight(k2->right)) + 1;
    k->height = max(NodeHeight(k->left), NodeHeight(k->right)) + 1;
    k2 = k;
}

/* ------------------------LR��ת------------------------*/
void LRrotate(PtrToNode &k3) {
    RRrotate(k3->left);
    LLrotate(k3);
    printf("���е���תΪ��LR\n");
}

/* ------------------------RL��ת------------------------*/
void RLrotate(PtrToNode &k3) {
    LLrotate(k3->right);
    RRrotate(k3);
    printf("���е���תΪ��RL\n");
}

/* -----------------------��ƽ�⴦��---------------------*/
void LeftBalance(PtrToNode &node) {
    PtrToNode ptrTmp = node->left;
    if (NodeHeight(ptrTmp->left) - NodeHeight(ptrTmp->right) == -1) {
        //�������������������������������
        LRrotate(node); //LR
    } else {
        LLrotate(node); //LL
        printf("���е���תΪ��LL\n");
    }
}

/* -----------------------��ƽ�⴦��---------------------*/
void RightBalance(PtrToNode &node) {
    PtrToNode ptrTmp = node->right;
    if (NodeHeight(ptrTmp->right) - NodeHeight(ptrTmp->left) == -1) {
        //���������������ߣ�˵���������������
        RLrotate(node); //RL
    } else {
        RRrotate(node);  //RR
        printf("���е���תΪ��RR\n");
    }
}

/* -----------------------�������-----------------------*/
void AVL_Insert(PtrToNode &node, elementType x) {
    if (NULL == node) //�ҵ�����Ľڵ��λ��
    {
        node = (struct AVLnode *) malloc(sizeof(struct AVLnode));
        node->data = x;
        node->height = 0;
        node->freq = 1;
        node->left = NULL;
        node->right = NULL;
    } else if (x < node->data)  //������������
    {
        AVL_Insert(node->left, x);
        //�ж��Ƿ��ƻ�AVL����ƽ����
        if (NodeHeight(node->left) - NodeHeight(node->right) == 2)
            LeftBalance(node);  //��ƽ�⴦��
    } else if (node->data < x)   //������������
    {
        AVL_Insert(node->right, x);
        //�ж��Ƿ��ƻ�AVL����ƽ����
        if (NodeHeight(node->right) - NodeHeight(node->left) == 2)
            RightBalance(node); //��ƽ�⴦��
    } else {
        node->freq++;
    }
    node->height = max(NodeHeight(node->left), NodeHeight(node->right)) + 1;  //�������ĸ߶�
}

/* -----------------------���Ҳ���-----------------------*/
PtrToNode AVL_Find(PtrToNode &node, elementType x) {
    if (node == NULL)  //û�ҵ�Ԫ��
    {
        return NULL;
    } else if (x < node->data) {
        return AVL_Find(node->left, x); //���������������
    } else if (node->data < x) {
        return AVL_Find(node->right, x); //���������������
    } else //���
        return node;
}

/* -----------------------ɾ������-----------------------*/
void AVL_Delete(PtrToNode &node, elementType x) {
    if (NULL == node)  //����ֱ�ӷ���
        return;
    if (x < node->data)  //���������в���
    {
        AVL_Delete(node->left, x);
        if (NodeHeight(node->right) - NodeHeight(node->left) == 2)  //����ƽ�⴦��
            RightBalance(node);
    } else if (node->data < x)  //���������в���
    {
        AVL_Delete(node->right, x);
        if (NodeHeight(node->left) - NodeHeight(node->right) == 2)  //����ƽ�⴦��
            LeftBalance(node);
    } else //�ҵ�Ҫɾ����Ԫ�ؽڵ�
    {
        if (node->left == NULL) //������Ϊ��
        {
            PtrToNode ptrTmp = node;
            node = node->right;         //���Һ��Ӵ���˽ڵ�
            free(ptrTmp);            //�ͷ��ڴ�
        } else if (node->right == NULL)  //������Ϊ��
        {
            PtrToNode ptrTmp = node;
            node = node->left;       //�����Ӵ���˽ڵ�
            free(ptrTmp);
        } else   //������������Ϊ��
        {
            //����������С���ݴ���ýڵ�
            PtrToNode ptrTmp = node->left;  //���������в���
            while (ptrTmp->right != NULL) ptrTmp = ptrTmp->right;
            //��ʱ��ptrTmpָ���������е����Ԫ��
            node->data = ptrTmp->data;
            AVL_Delete(node->left, ptrTmp->data);  //�ݹ��ɾ���ýڵ�
        }
    }
    //���½ڵ�ĸ߶�
    if (node)
        node->height = max(NodeHeight(node->left), NodeHeight(node->right));
}

/* -----------------------�������-----------------------*/
void In_traverse(PtrToNode &root) {
    if (NULL == root) {
        return;
    }
    In_traverse(root->left);
    printf("%d ", root->data);
    In_traverse(root->right);
}

/* -----------------------��ӡ��-----------------------*/
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
/* -----------------------���ļ���AVL��-----------------------*/
    FILE *fp;
    int n, k;
    PtrToNode root = NULL;
    int choice = 1;
    while (choice) {
        printf("************ AVL �� �� �� �� ʵ �� ************\n");
        printf("\t1.���ļ�����AVL��������\n");
        printf("\t2.�������������\n");
        printf("\t3.���Ҳ���\n");
        printf("\t4.�������\n");
        printf("\t5.ɾ������\n");
        printf("\t0.�˳�\n");
        printf("\n������0-5,ѡ��Ҫִ�еĲ���:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                fp = fopen("AVLtree.txt", "r");
                if (fp == NULL) {
                    printf("�ļ���ʧ�ܣ�\n");
                    exit(0);
                }
                fscanf(fp, "%6d", &n);
                for (int i = 0; i < n; i++) {
                    fscanf(fp, "%6d", &k);
                    AVL_Insert(root, k);
                    printf("���������Ϊ��%d \n", k);
                }
                printf("��ǰ��AVL����");
                OutputAVLTree(root);
                printf("\n");

            }
                system("pause");
                break;
            case 2: {
                printf("���������");
                In_traverse(root);
                printf("\n");
            }
                system("pause");
                break;
            case 3: {
                printf("������Ҫ���ҵ�ֵ��\n");
                int F;
                scanf("%d", &F);
                PtrToNode y = AVL_Find(root, F);
                if (y == NULL) {
                    printf("û�в��ҵ�%d\n", F);
                } else {
                    printf("���ڽڵ�ĸ߶ȣ�%d\n", y->height);
                    if (NULL != y->left) {
                        printf("���ڽڵ�����ӣ�%d\n", y->left->data);
                    }
                    if (NULL != y->right) {
                        printf("���ڽڵ���Һ��ӣ�%d\n", y->right->data);
                    }
                }

                printf("\n");
            }
                system("pause");
                break;
            case 4: {
                printf("��ǰ��AVL����");
                OutputAVLTree(root);
                printf("\n");
                printf("������Ҫ�����ֵ��\n");
                int m;
                scanf("%d", &m);
                AVL_Insert(root, m);
                printf("��ǰ��AVL����");
                OutputAVLTree(root);
                printf("\n");
            }
                system("pause");
                break;
            case 5: {
                printf("��ǰ��AVL����");
                OutputAVLTree(root);
                printf("\n");
                printf("������Ҫɾ����ֵ��\n");
                int D;
                scanf("%d", &D);
                AVL_Delete(root, D);
                printf("��ǰ��AVL����");
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
