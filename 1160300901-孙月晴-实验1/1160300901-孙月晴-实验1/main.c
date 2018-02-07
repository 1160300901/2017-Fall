#include <stdio.h>
#include <malloc.h>
#include <math.h>

struct polynode {
    float coef;
    int exp;
    struct polynode *next;
};//结点型
typedef struct polynode *polyn;

//将新的节点p 插入到现有链表的后面，并确保多项式的指数exp 是升序
void Insert(polyn p, polyn h) {
    if (p->coef == 0) free(p);   //系数为0释放结点
    else {
        polyn q1, q2;
        q1 = h;
        q2 = h->next;
        while (q2 && p->exp < q2->exp) { //查找插入位置
            q1 = q2;
            q2 = q2->next;
        }
        if (q2 && p->exp == q2->exp) { //将指数相同项合并
            q2->coef += p->coef;
            free(p);
            if (!q2->coef) {           //如果系数为零，释放节点
                q1->next = q2->next;
                free(q2);
            }
        } else {       //指数为新时将结点插入
            p->next = q2;
            q1->next = p;
        }
    }
}

/*******************以下函数实现显示输出多项式**************** ***/
void printPolyn(polyn p) {
    polyn q = p->next;
    int flag = 1;
    if (!q) {
        putchar('0');
        printf("\n");
        return;
    }
    while (q) {
        if (q->coef > 0 && flag != 1) putchar('+');
        if (q->coef != 1 && q->coef != -1) {
            printf("%.2f", q->coef);
            if (q->exp == 1) putchar('X');
            else if (q->exp == 0) putchar("");
            else printf("X^%d", q->exp);
        } else {
            if (q->coef == 1) {
                if (!q->exp) putchar('1');
                else if (q->exp == 1) putchar('X');
                else printf("X^%d", q->exp);
            }
            if (q->coef == -1) {
                if (!q->exp) printf("-1");
                else if (q->exp == 1) printf("-X");
                else printf("-X^%d", q->exp);
            }
        }
        q = q->next;
        flag++;
    }
    printf("\n");
}

//判断两个多项式在同一指数下是否有其中一个为系数为0。用来辅助加法和乘法运算
int compare(polyn a, polyn b) {
    if (a && b) {
        if (!b || a->exp > b->exp) return 1;
        else if (!a || a->exp < b->exp) return -1;
        else return 0;
    } else if (!a && b)
        return -1;
    else return 1;//b 多项式已空，但a 多项式非空
}//compare

/********************  test *****************************/
void WriteFile(polyn p) {
    FILE *fp;
    fp = fopen("polyn.txt", "a");
    if (fp == NULL) {
        printf("Can' t open the %s\n", "polyn.txt");
        exit(0);
    }
    polyn q = p->next;
    int flag = 1;
    if (!q) {
        fprintf(fp, "0");
        return;
    }
    while (q) {
        if (q->coef > 0 && flag != 1) {
            fprintf(fp, "+");
        }
        if (q->coef != 1 && q->coef != -1) {
            fprintf(fp, "%.2f", q->coef);
            if (q->exp == 1)
                fprintf(fp, "X");
            else {
                fprintf(fp, "X^");
                fprintf(fp, "%d", q->exp);
            }
        } else {
            if (q->coef == 1) {
                if (!q->exp) fprintf(fp, "1");
                else if (q->exp == 1) fprintf(fp, "X");
                else {
                    fprintf(fp, "X^");
                    fprintf(fp, "%d", q->exp);
                }
            }
            if (q->coef == -1) {
                if (!q->exp) fprintf(fp, "-1");
                else if (q->exp == 1) fprintf(fp, "-X");
                else {
                    fprintf(fp, "-X^");
                    fprintf(fp, "%d", q->exp);
                }
            }
        }
        q = q->next;
        flag++;
    }
    printf("\n");
    fprintf(fp, "\n");
    fclose(fp);
}

/*********************以下函数实现加法*********************/
//计算的结果存放在新建的qc 所指向的链表中。
polyn AddPolyn(polyn pa, polyn pb) {//求解并建立多项式a+b，返回其头指针
    polyn qa = pa->next;
    polyn qb = pb->next;
    polyn headc, hc, qc;
    hc = (polyn) malloc(sizeof(struct polynode));//建立头结点
    hc->next = NULL;
    headc = hc;
    while (qa || qb) {
        qc = (polyn) malloc(sizeof(struct polynode));
        switch (compare(qa, qb)) {
            case 1: {     //qa->exp>qb->exp 或qb空
                qc->coef = qa->coef;
                qc->exp = qa->exp;
                qa = qa->next;
                break;
            }
            case 0: { // =
                qc->coef = qa->coef + qb->coef;
                qc->exp = qa->exp;
                qa = qa->next;
                qb = qb->next;
                break;
            }
            case -1: { // < 或pa空
                qc->coef = qb->coef;
                qc->exp = qb->exp;
                qb = qb->next;
                break;
            }
        }//switch
        if (qc->coef != 0) {
            qc->next = hc->next;
            hc->next = qc;
            hc = qc;
        } else free(qc); //当相加系数为0时，释放该结点
    }//while
    return headc;

}//AddPolyn

/********************以下函数实现减法***********************/
polyn SubtractPolyn(polyn pa, polyn pb) {//求解并建立多项式a-b，返回其头指针
    polyn h = pb;
    polyn p = pb->next;
    polyn pd;
    while (p) { //将pb 的系数取反
        p->coef *= -1;
        p = p->next;
    }
    pd = AddPolyn(pa, h);
    for (p = h->next; p; p = p->next) //恢复pb 的系数
        p->coef *= -1;
    return pd;
}//SubtractPolyn

/*******************以下函数实现乘法*********************/
polyn MultiplyPolyn(polyn pa, polyn pb) {//求解并建立多项式a*b，返回其头指针,该函数实现乘法
    polyn hf, pf;
    polyn qa = pa->next;
    polyn qb;//= pb->next
    hf = (polyn) malloc(sizeof(struct polynode));//建立头结点
    hf->next = NULL;
    for (; qa; qa = qa->next) {
        for (qb = pb->next; qb; qb = qb->next) {
            pf = (polyn) malloc(sizeof(struct polynode));
            pf->coef = qa->coef * qb->coef;
            pf->exp = qa->exp + qb->exp;
            Insert(pf, hf);//调用Insert 函数以合并指数相同的项
        }
    }
    return hf;
}//MultiplyPolyn

/**********************以下函数实现多项式的销毁**********************/
void DestroyPolyn(polyn p) {//销毁多项式p
    polyn q1, q2;
    q1 = p->next;
    q2 = q1->next;
    while (q1->next) {
        free(q1);
        q1 = q2;//指针后移
        q2 = q2->next;
    }
}

/**********************以下函数实现多项式的求值**********************/
float Cal(float x, polyn a) {
    float sum = 0;
    while (a->next != NULL) {
        a = a->next;
        sum += pow(x, a->exp) * (a->coef);
    }
    return sum;
}

/**********************以下函数实现多项式的除法**********************/
void Div(polyn head1, polyn head2)            //除法
{
    polyn temp1, head3, head4, templ;
    temp1 = (polyn) malloc(sizeof(struct polynode));
    temp1->next = NULL;
    head3 = temp1;           //商
    while (head1->next != NULL && head1->next->exp >= head2->next->exp)   //判断指数大小
    {
        polyn z = (polyn) malloc(sizeof(struct polynode)); //商的一项，中间节点
        polyn a = (polyn) malloc(sizeof(struct polynode)); //每次与除数相乘
        a->next = NULL;
        z->coef = head1->next->coef / head2->next->coef;
        z->exp = head1->next->exp - head2->next->exp;
        Insert(z,head3);
        a->next = z;
        a->next->next = NULL;
        head4 = MultiplyPolyn(a, head2);
        head1 = SubtractPolyn(head1, head4);
        temp1 = temp1->next;
        free(a);
    }
    printf("\n商：");
    printPolyn(head3);
    printf("余数：");
    printPolyn(head1);
}
/********************主函数实现显示与功能选择**********************/
int main() {
    int flag = 0;
    float x;
    FILE *fp;
    char c;
    polyn pa = 0, pb = 0, pc, pd, pf;//定义各式的头指针，pa 与pb 在使用前赋初值NULL
//pc 头指针所在的多项式用在加法中作为结果，pd 用在加法中，pf 乘法中
    pa = (polyn) malloc(sizeof(struct polynode));
    pa->next = NULL;
    pb = (polyn) malloc(sizeof(struct polynode));
    pb->next = NULL;
    printf("**************** 欢迎使用一元多项式运算程序 *****************\n");
    printf("请选择您要进行的操作：\n\t1.从文件输入多项式\n\t2.从文件输出多项式a 和b\n\t3.多项式a+b\n\t4.多项式a-b\n\t"
                   "5.计算多项式a*b 的值\n\t6.计算多项式a/b\n\t7.计算第一个多项式在x处的值\n\t8.退出\n");
    for (;; flag = 0) {
        printf("\n");
        scanf("%d", &flag);
        if (flag == 1) {
            int exp1, coef1,exp2, coef2;
            polyn q,p;
            q = pa;
            FILE *a = fopen("input_a.txt", "r");
            if (a == NULL) {
                printf("打开文件失败!");
                return NULL;
            }
            while (fscanf(a, "%d %d", &coef1, &exp1) != EOF) {
                q = (polyn) malloc(sizeof(struct polynode));
                q->coef = coef1;
                q->exp = exp1;
                Insert(q, pa);
            }
            printf("多项式a:");
            printPolyn(pa);
             FILE *b = fopen("input_b.txt", "r");
            if (b == NULL) {
                printf("打开文件失败!");
                return NULL;
            }
            while (fscanf(b, "%d %d", &coef2, &exp2) != EOF) {
                p = (polyn) malloc(sizeof(struct polynode));
                p->coef = coef2;
                p->exp = exp2;
                Insert(p, pb);
            }
            printf("多项式b:");
            printPolyn(pb);
            continue;
        }
        if (flag == 2) {
            WriteFile(pa);
            WriteFile(pb);
            printf("多项式a,b：");
            fp = fopen("polyn.txt", "r");        /* 以读方式打开文本文件 */
            if (fp == NULL)
                printf("can not open!\n");
            else {
                while ((c = getc(fp)) != EOF)   /* 未读到文件结束标志时 */
                    putchar(c);/* 在显示器显示读出的字符 */
            }
            fclose(fp);
            continue;
        }
        if (flag == 3) {
            pc = AddPolyn(pa, pb);
            printf("多项式a+b：");
            printPolyn(pc);
            DestroyPolyn(pc);
            continue;
        }
        if (flag == 4) {
            pd = SubtractPolyn(pa, pb);
            printf("多项式a-b：");
            printPolyn(pd);
            DestroyPolyn(pd);
            continue;
        }
        if (flag == 5) {
            pf = MultiplyPolyn(pa, pb);
            printf("多项式a*b：");
            printPolyn(pf);
            DestroyPolyn(pf);
            continue;
        }
        if (flag == 6) {
            printf("多项式a/b：");
            Div(pa, pb);
            continue;
        }
        if (flag == 7) {
            printf("请输入x的值:");
            scanf("%f", &x);
            printf("%lf", Cal(x, pa));
            continue;
        }
        if (flag == 8) {
            printf("\nFnished!\n");
            break;
        }
        if (flag < 1 || flag > 8) printf("Error!!!\n");
        continue;
    }//for
    DestroyPolyn(pa);
    DestroyPolyn(pb);
    return 0;
}
