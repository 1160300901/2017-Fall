#include <stdio.h>
#include <malloc.h>
#include <math.h>

struct polynode {
    float coef;
    int exp;
    struct polynode *next;
};//�����
typedef struct polynode *polyn;

//���µĽڵ�p ���뵽��������ĺ��棬��ȷ������ʽ��ָ��exp ������
void Insert(polyn p, polyn h) {
    if (p->coef == 0) free(p);   //ϵ��Ϊ0�ͷŽ��
    else {
        polyn q1, q2;
        q1 = h;
        q2 = h->next;
        while (q2 && p->exp < q2->exp) { //���Ҳ���λ��
            q1 = q2;
            q2 = q2->next;
        }
        if (q2 && p->exp == q2->exp) { //��ָ����ͬ��ϲ�
            q2->coef += p->coef;
            free(p);
            if (!q2->coef) {           //���ϵ��Ϊ�㣬�ͷŽڵ�
                q1->next = q2->next;
                free(q2);
            }
        } else {       //ָ��Ϊ��ʱ��������
            p->next = q2;
            q1->next = p;
        }
    }
}

/*******************���º���ʵ����ʾ�������ʽ**************** ***/
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

//�ж���������ʽ��ͬһָ�����Ƿ�������һ��Ϊϵ��Ϊ0�����������ӷ��ͳ˷�����
int compare(polyn a, polyn b) {
    if (a && b) {
        if (!b || a->exp > b->exp) return 1;
        else if (!a || a->exp < b->exp) return -1;
        else return 0;
    } else if (!a && b)
        return -1;
    else return 1;//b ����ʽ�ѿգ���a ����ʽ�ǿ�
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

/*********************���º���ʵ�ּӷ�*********************/
//����Ľ��������½���qc ��ָ��������С�
polyn AddPolyn(polyn pa, polyn pb) {//��Ⲣ��������ʽa+b��������ͷָ��
    polyn qa = pa->next;
    polyn qb = pb->next;
    polyn headc, hc, qc;
    hc = (polyn) malloc(sizeof(struct polynode));//����ͷ���
    hc->next = NULL;
    headc = hc;
    while (qa || qb) {
        qc = (polyn) malloc(sizeof(struct polynode));
        switch (compare(qa, qb)) {
            case 1: {     //qa->exp>qb->exp ��qb��
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
            case -1: { // < ��pa��
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
        } else free(qc); //�����ϵ��Ϊ0ʱ���ͷŸý��
    }//while
    return headc;

}//AddPolyn

/********************���º���ʵ�ּ���***********************/
polyn SubtractPolyn(polyn pa, polyn pb) {//��Ⲣ��������ʽa-b��������ͷָ��
    polyn h = pb;
    polyn p = pb->next;
    polyn pd;
    while (p) { //��pb ��ϵ��ȡ��
        p->coef *= -1;
        p = p->next;
    }
    pd = AddPolyn(pa, h);
    for (p = h->next; p; p = p->next) //�ָ�pb ��ϵ��
        p->coef *= -1;
    return pd;
}//SubtractPolyn

/*******************���º���ʵ�ֳ˷�*********************/
polyn MultiplyPolyn(polyn pa, polyn pb) {//��Ⲣ��������ʽa*b��������ͷָ��,�ú���ʵ�ֳ˷�
    polyn hf, pf;
    polyn qa = pa->next;
    polyn qb;//= pb->next
    hf = (polyn) malloc(sizeof(struct polynode));//����ͷ���
    hf->next = NULL;
    for (; qa; qa = qa->next) {
        for (qb = pb->next; qb; qb = qb->next) {
            pf = (polyn) malloc(sizeof(struct polynode));
            pf->coef = qa->coef * qb->coef;
            pf->exp = qa->exp + qb->exp;
            Insert(pf, hf);//����Insert �����Ժϲ�ָ����ͬ����
        }
    }
    return hf;
}//MultiplyPolyn

/**********************���º���ʵ�ֶ���ʽ������**********************/
void DestroyPolyn(polyn p) {//���ٶ���ʽp
    polyn q1, q2;
    q1 = p->next;
    q2 = q1->next;
    while (q1->next) {
        free(q1);
        q1 = q2;//ָ�����
        q2 = q2->next;
    }
}

/**********************���º���ʵ�ֶ���ʽ����ֵ**********************/
float Cal(float x, polyn a) {
    float sum = 0;
    while (a->next != NULL) {
        a = a->next;
        sum += pow(x, a->exp) * (a->coef);
    }
    return sum;
}

/**********************���º���ʵ�ֶ���ʽ�ĳ���**********************/
void Div(polyn head1, polyn head2)            //����
{
    polyn temp1, head3, head4, templ;
    temp1 = (polyn) malloc(sizeof(struct polynode));
    temp1->next = NULL;
    head3 = temp1;           //��
    while (head1->next != NULL && head1->next->exp >= head2->next->exp)   //�ж�ָ����С
    {
        polyn z = (polyn) malloc(sizeof(struct polynode)); //�̵�һ��м�ڵ�
        polyn a = (polyn) malloc(sizeof(struct polynode)); //ÿ����������
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
    printf("\n�̣�");
    printPolyn(head3);
    printf("������");
    printPolyn(head1);
}
/********************������ʵ����ʾ�빦��ѡ��**********************/
int main() {
    int flag = 0;
    float x;
    FILE *fp;
    char c;
    polyn pa = 0, pb = 0, pc, pd, pf;//�����ʽ��ͷָ�룬pa ��pb ��ʹ��ǰ����ֵNULL
//pc ͷָ�����ڵĶ���ʽ���ڼӷ�����Ϊ�����pd ���ڼӷ��У�pf �˷���
    pa = (polyn) malloc(sizeof(struct polynode));
    pa->next = NULL;
    pb = (polyn) malloc(sizeof(struct polynode));
    pb->next = NULL;
    printf("**************** ��ӭʹ��һԪ����ʽ������� *****************\n");
    printf("��ѡ����Ҫ���еĲ�����\n\t1.���ļ��������ʽ\n\t2.���ļ��������ʽa ��b\n\t3.����ʽa+b\n\t4.����ʽa-b\n\t"
                   "5.�������ʽa*b ��ֵ\n\t6.�������ʽa/b\n\t7.�����һ������ʽ��x����ֵ\n\t8.�˳�\n");
    for (;; flag = 0) {
        printf("\n");
        scanf("%d", &flag);
        if (flag == 1) {
            int exp1, coef1,exp2, coef2;
            polyn q,p;
            q = pa;
            FILE *a = fopen("input_a.txt", "r");
            if (a == NULL) {
                printf("���ļ�ʧ��!");
                return NULL;
            }
            while (fscanf(a, "%d %d", &coef1, &exp1) != EOF) {
                q = (polyn) malloc(sizeof(struct polynode));
                q->coef = coef1;
                q->exp = exp1;
                Insert(q, pa);
            }
            printf("����ʽa:");
            printPolyn(pa);
             FILE *b = fopen("input_b.txt", "r");
            if (b == NULL) {
                printf("���ļ�ʧ��!");
                return NULL;
            }
            while (fscanf(b, "%d %d", &coef2, &exp2) != EOF) {
                p = (polyn) malloc(sizeof(struct polynode));
                p->coef = coef2;
                p->exp = exp2;
                Insert(p, pb);
            }
            printf("����ʽb:");
            printPolyn(pb);
            continue;
        }
        if (flag == 2) {
            WriteFile(pa);
            WriteFile(pb);
            printf("����ʽa,b��");
            fp = fopen("polyn.txt", "r");        /* �Զ���ʽ���ı��ļ� */
            if (fp == NULL)
                printf("can not open!\n");
            else {
                while ((c = getc(fp)) != EOF)   /* δ�����ļ�������־ʱ */
                    putchar(c);/* ����ʾ����ʾ�������ַ� */
            }
            fclose(fp);
            continue;
        }
        if (flag == 3) {
            pc = AddPolyn(pa, pb);
            printf("����ʽa+b��");
            printPolyn(pc);
            DestroyPolyn(pc);
            continue;
        }
        if (flag == 4) {
            pd = SubtractPolyn(pa, pb);
            printf("����ʽa-b��");
            printPolyn(pd);
            DestroyPolyn(pd);
            continue;
        }
        if (flag == 5) {
            pf = MultiplyPolyn(pa, pb);
            printf("����ʽa*b��");
            printPolyn(pf);
            DestroyPolyn(pf);
            continue;
        }
        if (flag == 6) {
            printf("����ʽa/b��");
            Div(pa, pb);
            continue;
        }
        if (flag == 7) {
            printf("������x��ֵ:");
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
