#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <afxres.h>
#include <queue>
#include <stack>

using namespace std;

/*=======================�ڽӾ���洢============================*/
#define NumVertices 15
typedef struct {
    char vertlist[NumVertices];
    int edge[NumVertices][NumVertices];
    int n, e;
} MTGraph;

int visited1[NumVertices];//���ʱ������
int visited2[NumVertices];
int dfn[NumVertices];//������
int Count = 1;//����
int s[NumVertices + 1];//�ǵݹ�ջ����
int Queue[NumVertices + 1];

/*======================ͼ���ڽӱ�洢��ʾ=======================*/
#define MAX 20
typedef struct ArcNode {
    int adjvex;       //�����ڽӶ���
    char info;        //�ڽӵ�ֵ
    struct ArcNode *nextarc;   //ָ����һ������ָ��
} ArcNode;

typedef struct Vnode {          //�ڵ���Ϣ
    char data;
    ArcNode *firstedge;
} AdjList[MAX];

typedef struct {
    AdjList vertices;
    int vexnum;              //�ڵ���
    int arcnum;              //����
} ALGraph;

/*=========================�ڽӶ��ر�洢========================*/
#define MAX_VERTEX_NUM 20
typedef enum {
    unvisited, visited
} VisitIf;

typedef struct EBox {
    VisitIf mark; //�߷��ʱ��
    int ivex, jvex; //�ñ����������������λ��
    struct EBox *ilink, *jlink; //�ֱ�ָ�������������������һ����
    int *info; //�ñ���Ϣָ��
} EBox;

typedef struct VexBox {
    char data;
    EBox *firstedge; //ָ���һ�������ڸö���ı�
} VexBox;

typedef struct {
    VexBox adjmulist[MAX_VERTEX_NUM];
    int vexnum, edgenum; //����ͼ�ĵ�ǰ�������ͱ���
} AMLGraph;

/*========================���ļ������ڽӾ���=====================*/
void CreateMGragh(MTGraph *G) {
    int i, j, k, w;
    FILE *fp;
    fp = fopen("MTgraph.txt", "r");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit(0);
    }
    fscanf(fp, "%6d%6d", &G->n, &G->e);
    for (i = 0; i < G->n; i++)
        fscanf(fp, " %c", &G->vertlist[i]);
    for (i = 0; i < G->n; i++) {
        for (j = 0; j < G->n; j++)
            G->edge[i][j] = 0;
    }
    for (k = 0; k < G->e; k++) {
        fscanf(fp, "%6d%6d%6d", &i, &j, &w);
        G->edge[i][j] = w;
        G->edge[j][i] = w;
    }
    fclose(fp);
}

/*=========================��ӡ�ڽӾ���==========================*/
void OutPutAdjMatrix(MTGraph gm) {
    cout << "ͼ���ڽӾ������£�" << endl;
    for (int i = 0; i < gm.n; i++) {
        for (int j = 0; j < gm.n; j++) {
            printf("%d ", gm.edge[i][j]);
        }
        cout << endl;
    }
}

/*====================�ڽӾ���ĵݹ������������=================*/
void DFSArr(MTGraph *G, int i) {
    int j;
    cout << G->vertlist[i] << " ";
    visited1[i] = 1;  //���ʹ����1
    dfn[i] = Count;
    Count++;
    for (j = 0; j < G->n; j++) //��������vi���ڽӵ�
    {
        if ((G->edge[i][j] == 1) && !visited1[j])
            DFSArr(G, j);
    }
}

void DFSTraverse(MTGraph G) { //��ȵݹ�������
    int i;
    for (i = 0; i < G.n; i++)
        visited1[i] = 0;
    for (i = 0; i < G.n; i++) {
        if (!visited1[i])
            DFSArr(&G, i);
    }
}

/*==================�ڽӾ���ķǵݹ������������=================*/
void DFS(MTGraph *G) {
    int top = -1, v = 0, i;
    for (int k = 0; k < G->n; k++) {
        visited1[k] = false;
    }
    visited1[0] = true;
    s[++top] = v;
    cout << G->vertlist[v] << " ";
    while (top != -1) {
        v = s[top];
        for (i = 0; i < G->n; i++) {
            if (G->edge[v][i] == 1 && !visited1[i]) {
                cout << G->vertlist[i] << " ";
                visited1[i] = true;
                s[++top] = i;
                break;
            }
        }
        if (i == G->n) {
            top--;
        }
    }
}

/*==================�ڽӾ���Ĺ����������=======================*/
void BFSArr(MTGraph *G, int k) {
    int i, j, Front = 0, Rear = 0;
    cout << G->vertlist[k] << " ";
    visited2[k] = 1;
    Queue[++Rear] = k;  //��ǰ�������
    while (Front != Rear) {
        i = Queue[++Front]; //�������
        for (j = 0; j < G->n; j++) {
            if (G->edge[i][j] == 1 && !visited2[j]) {
                cout << G->vertlist[j] << " ";
                visited2[j] = 1;
                Queue[++Rear] = j; //�������
            }
        }
    }
}

void BFSTraverse(MTGraph G) { //������ȱ���������
    int i;
    for (i = 0; i < G.n; i++)
        visited2[i] = 0;
    for (i = 0; i < G.n; i++) {
        if (!visited2[i])
            BFSArr(&G, i);
    }
}

/*=========================�����ڽӱ�============================*/
void CreateAdjList(ALGraph *G) {
    int i, s, d;
    ArcNode *p1, *p2;
    FILE *fp;                                /*ָ���ļ���ָ��*/
    if ((fp = fopen("ALgraph.txt", "r")) == NULL)  /*���ļ������жϴ��Ƿ�����*/
    {
        printf("���ܴ��ļ�\n");    /*û��*/
        exit(1);                          /*�˳�*/
    }
    fscanf(fp, "%2d%2d", &G->vexnum, &G->arcnum);                          /*�����¼��*/
    for (i = 0; i < G->vexnum; i++) {
        fscanf(fp, " %c", &G->vertices[i].data);
        G->vertices[i].firstedge = NULL;
    }
    for (i = 0; i < G->arcnum; i++) {
        fscanf(fp, "%d %d", &s, &d);
        p1 = new ArcNode;
        p1->adjvex = d;
        p1->info = G->vertices[d].data;
        p1->nextarc = G->vertices[s].firstedge;
        G->vertices[s].firstedge = p1;

        p2 = new ArcNode;
        p2->adjvex = s;
        p2->info = G->vertices[s].data;
        p2->nextarc = G->vertices[d].firstedge;
        G->vertices[d].firstedge = p2;
        //��Ϊ������ͼ�����������ν����߱�Ĺ���
    }
    fclose(fp);
}

/*=========================��ӡ�ڽӱ�============================*/
void OutPutAdjList(ALGraph *G) {
    int i;
    ArcNode *p;
    cout << "ͼ���ڽӱ����£�" << endl;
    for (i = 0; i < G->vexnum; i++) {
        cout << G->vertices[i].data;
        p = G->vertices[i].firstedge;
        while (p != NULL) {
            cout << "---->(" << p->adjvex << " " << p->info << ")";
            p = p->nextarc;
        }
        cout << endl;
    }
}

/*=================�ڽӱ�ĵݹ�������ȱ���======================*/
void DFS(ALGraph *G, int i, int visited[]) {//�ӵ�v����������ݹ���������������������ͼG��

    cout << G->vertices[i].data << " ";
    visited[i] = 1;               //���÷��ʱ�־���飨��ʼ��δ����Ϊ0���ѷ���Ϊ1��
    ArcNode *p = new ArcNode;
    p = G->vertices[i].firstedge;  //ָ���һ�������ڸö���vi�ıߵ�ָ��

    if (G->vertices[i].firstedge && !visited[p->adjvex])
    //ָ���һ�������ڸö���vi�ıߵ�ָ��ǿգ��Ҷ���δ������
    {
        DFS(G, p->adjvex, visited);//�ݹ飺���ҵ���
    }
}

void DFStraversal(ALGraph *G, char c) { //������ȱ���
    cout << "�ڽӱ�ĵݹ�������ȱ������Ϊ:" << endl;
    int visited[MAX];
    for (int i = 0; i < G->vexnum; i++) {
        visited[i] = 0;  //ȫ����ʼ��Ϊ0����δ����״̬
    }
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vertices[i].data == c)//�����ַ��������
        {
            DFS(G, i, visited);  //��������ĺ���
            break;
        }
    }
    //��������δ�����ʵĽ�㣬ֱ��ͼ�����ж��㶼���������
    for (int i = 0; i < G->vexnum; i++) {
        if (visited[i] == 0)
            DFS(G, i, visited);
    }
    cout << endl;
}

/*===================�ڽӱ�ķǵݹ�������ȱ���==================*/
void st_DFS(ALGraph *G, int k) {
    bool Visited[MAX] = {false};
    int stack[MAX];        //ջ
    int top = -1;

    ArcNode *pEdge = NULL;
    int Pre[MAX];
    int index = -1;        //Pre������±�

    Visited[k] = true;            //��ʼ�������ѷ���
    cout << G->vertices[k].data << " ";            //�����ʼ����

    //����������ʼ��������Ķ������Per������
    pEdge = G->vertices[k].firstedge;
    while (NULL != pEdge) {
        Pre[++index] = pEdge->adjvex;
        pEdge = pEdge->nextarc;
    }
    //�����Pre�����еĶ������ջ��
    for (int i = index; i >= 0; i--) {
        top++;
        stack[top] = Pre[i];
        Visited[Pre[i]] = 1;        //��ջ�Ķ������ѷ���
    }

    while (top > -1) {
        int TopVertex = stack[top];
        top--;
        cout << G->vertices[TopVertex].data << " ";        //���ջ���ڵ�

        pEdge = G->vertices[TopVertex].firstedge;        //��������ջ�׶��������δ���ʹ��Ķ�����ջ
        index = -1;    //����Pre�����±�
        while (NULL != pEdge) {
            if (Visited[pEdge->adjvex] == 0)
                Pre[++index] = pEdge->adjvex;
            pEdge = pEdge->nextarc;
        }
        for (int i = index; i >= 0; i--)        //�����Pre�����еĶ�����뵽ջ��
        {
            top++;
            stack[top] = Pre[i];
            Visited[Pre[i]] = 1;
        }
    }//end while(top > -1)
}

/*======================�ڽӱ�Ĺ�����ȱ���=====================*/
int visited4[MAX];//���ʱ�־������ȫ����
void BFS(ALGraph *&G, int v) { //�����������
    queue<int> Q;
    int j;
    ArcNode *p;
    if (!visited4[v]) {
        cout << G->vertices[v].data << " ";
        visited4[v] = 1;
        Q.push(v); //�����ʶ������
    }
    while (!Q.empty()) //���в��ս�ѭ��
    {
        v = Q.front();//����ͷ
        Q.pop(); //����
        p = G->vertices[v].firstedge;
        while (p) {
            j = p->adjvex;
            if (!visited4[j]) {
                cout << G->vertices[j].data << " ";
                visited4[j] = 1;
                Q.push(j);
            }
            p = p->nextarc;
        }
    }
}

/*======================���ļ������ڽӶ��ر�=====================*/
int LocateVex(AMLGraph G, char u) { /* ��ʼ����������ͼG���ڣ�u��G�ж�������ͬ���� */
    /* �����������G�д��ڶ���u���򷵻ظö���������ͼ��λ�ã����򷵻�-1 */
    int i;
    for (i = 0; i < G.vexnum; ++i)
        if (u == G.adjmulist[i].data)
            return i;
    return -1;
}

void GreateAMLGraph(AMLGraph *G) {
    int i, j, k;
    char va, vb;
    EBox *p;
    FILE *fp;                                /*ָ���ļ���ָ��*/
    if ((fp = fopen("AMLgraph.txt", "rb")) == NULL)  /*���ļ������жϴ��Ƿ�����*/
    {
        printf("���ܴ��ļ�\n");    /*û��*/
        exit(1);                          /*�˳�*/
    }
    fscanf(fp, "%2d %2d", &G->vexnum, &G->edgenum);                          /*�����¼��*/
    for (i = 0; i < (*G).vexnum; ++i)         /* ���춥������ */
    {
        fscanf(fp, " %c", &G->adjmulist[i].data);
        (*G).adjmulist[i].firstedge = NULL;
    }
    for (k = 0; k <= (*G).edgenum; ++k)        /* ����������� */
    {
        fscanf(fp, "%c%c ", &va, &vb);
        i = LocateVex(*G, va);            /* һ�� */
        j = LocateVex(*G, vb);            /* ��һ�� */
        p = (EBox *) malloc(sizeof(EBox));
        p->mark = unvisited;                /* ���ֵ */
        p->ivex = i;
        p->ilink = (*G).adjmulist[i].firstedge; /* ���ڱ�ͷ */
        (*G).adjmulist[i].firstedge = p;
        p->jvex = j;
        p->jlink = (*G).adjmulist[j].firstedge; /* ���ڱ�ͷ */
        (*G).adjmulist[j].firstedge = p;
    }
    fclose(fp);                                  /*�ر��ļ�*/
}

/*=========================��ӡ�ڽӶ��ر�========================*/
void MarkUnvizited(AMLGraph &G) { /* �ñߵķ��ʱ��Ϊδ������ */
    int i;
    EBox *p;
    for (i = 0; i < G.vexnum; i++) {
        p = G.adjmulist[i].firstedge;
        while (p) {
            p->mark = unvisited;
            if (p->ivex == i)
                p = p->ilink;
            else
                p = p->jlink;
        }
    }
}

void Display(AMLGraph G) { /* �������ͼ���ڽӶ��ر�G */
    printf("ͼ���ڽӶ��ر����£�\n");
    int i;
    EBox *p;
    MarkUnvizited(G); /* �ñߵķ��ʱ��Ϊδ������ */
    printf("��%d�����㣺\n", G.vexnum);
    for (i = 0; i < G.vexnum; ++i)
        printf("%c ", G.adjmulist[i].data);
    printf("\n��%d����:\n", G.edgenum);
    for (i = 0; i < G.vexnum; i++) {
        p = G.adjmulist[i].firstedge;
        while (p)
            if (p->ivex == i) /* �ߵ�i����ö����й� */
            {
                if (!p->mark) /* ֻ���һ�� */
                {
                    printf("%c��%c   \n", G.adjmulist[i].data, G.adjmulist[p->jvex].data);
                    p->mark = visited;
                }
                p = p->ilink;
            } else /* �ߵ�j����ö����й� */
            {
                if (!p->mark) /* ֻ���һ�� */
                {
                    printf("%c��%c   \n", G.adjmulist[p->ivex].data, G.adjmulist[i].data);
                    p->mark = visited;
                }
                p = p->jlink;
            }
    }
}

/*=========================�ڽӶ��ر�ĵݹ�����===================*/
int visited3[MAX_VERTEX_NUM];//���ʱ�־����

void AMLDFS(AMLGraph G, int i) { //�ݹ�ʵ��������ȱ���
    int j;
    EBox *p;

    printf("%c ", G.adjmulist[i].data);
    visited3[i] = TRUE;

    p = G.adjmulist[i].firstedge;

    while (p) {
        j = p->ivex == i ? p->jvex : p->ivex;
        if (!visited3[j])
            AMLDFS(G, j);
        p = p->ivex == i ? p->ilink : p->jlink;
    }

}

void DFSTraverse2(AMLGraph G) {
    int i;

    for (i = 0; i < G.vexnum; i++)
        visited3[i] = FALSE;
    //ѭ����Ϊ��֤ÿ�����㶼�ܱ����ʵ�
    for (i = 0; i < G.vexnum; i++)
        if (!visited3[i])
            AMLDFS(G, i);

}

/*=======================�ڽӶ��ر�ķǵݹ�����===================*/
bool visited5[MAX];

int AdjVexNotVisit(AMLGraph &G, int i){//����δ���ʵ��ڽӶ���
    EBox *p = G.adjmulist[i].firstedge; //pָ�������ڶ���i�ĵ�һ����
    while (p) {
        if (i == p->ivex)   //i���Ǹñߵ�ͷ���
        {
            if (!visited5[p->jvex])
                return p->jvex;  //����δ���ʵĽ�㣬���ظõ�Ĵ洢λ��
            else {
                p = p->ilink; //�ñ߲�����δ���ʵĽ�㣬p��hlink��path1���ƶ�
            }
        } else    //i���Ǹñߵ�β���
        {
            if (!visited5[p->ivex])
                return p->ivex;  //����δ���ʵĽ�㣬���ظõ�Ĵ洢λ��
            else {
                p = p->jlink; //�ñ߲�����δ���ʵĽ�㣬p��tlink��path2���ƶ�
            }
        }
    }
    return -1;  //�ö��㲻����δ���ʵ��ڽӶ��㣬����-1
}

void DFSTraverse_non_recursive(AMLGraph &G, int start) {
    int i;

    for (i = 0; i < G.vexnum; i++) {
        visited5[i] = false;   //�������ʱ�־�����ʼ��
    }
    for (i = 0; i < G.vexnum; ++i) {
        if (!visited5[start]) //��δ����
        {
            visited5[start] = true;    //���÷��ʱ�־
            cout << G.adjmulist[start].data << " ";   //��������õ������
            stack<int> s;  //�ÿյĸ���ջs
            int pos, w;
            EBox *p;
            s.push(start);  //���õ��ջ
            while (!s.empty())   //��ջ�ǿյ�ʱ��ѭ��
            {
                pos = s.top();    //����posΪs��ջ����ŵĵ�
                w = AdjVexNotVisit(G, pos);    //�ҵ�����pos��δ���ʵ��ڽӵ�
                if (w >= 0)    //������������ĵ�
                {
                    visited5[w] = true;    //���÷��ʱ�־
                    cout << G.adjmulist[w].data << " ";   //��������õ������
                    s.push(w);  //���õ��ջ
                } else    //��������������ĵ�
                {
                    s.pop();    //���õ��ջ����Ϊû����ѭ�����ʵı�Ҫ��
                }
                //����ѭ����ֱ�����еĶ��㶼�����ʹ�
            }
        }
    }
    cout << endl;
}

/*==========================�ڽӶ��ر�Ĺ���=======================*/
void BFSTraverse(AMLGraph &G, int start) {
    bool visited[MAX];
    int i;
    for (i = 0; i < G.vexnum; i++)
        visited[i] = false;   //�������ʱ�־�����ʼ��
    for (i = 0; i < G.vexnum; i++) {
        if (!visited[start]) //��δ����
        {
            visited[start] = true;    //���÷��ʱ�־
            cout << G.adjmulist[start].data << " ";   //��������õ������
            queue<int> q;   //�ÿյĸ�������Q
            EBox *p;
            int pos;
            q.push(start);  //�����
            while (!q.empty())   //���ӷǿ�
            {
                pos = q.front();  //��ͷԪ�س��Ӳ���Ϊpos
                q.pop();
                p = G.adjmulist[pos].firstedge;    //pָ�������ڶ���pos�ĵ�һ����
                while (p) {
                    if (p->ivex == pos) //�������pos���Ǹñߵ�ͷ���
                    {
                        if (!visited[p->jvex])    //����ñߵ�β���δ����
                        {
                            visited[p->jvex] = true;   //���÷��ʱ�־
                            cout << G.adjmulist[p->jvex].data << " ";   //���������β��������
                            q.push(p->jvex); //��β���������
                        }
                        p = p->ilink; //p��hlink��path1)�ƶ�
                    } else    //�������pos���Ǹñߵ�β���
                    {
                        if (!visited[p->ivex])    //����ñߵ�ͷ���δ����
                        {
                            visited[p->ivex] = true;   //���÷��ʱ�־
                            cout << G.adjmulist[p->ivex].data << " ";  //���������ͷ��������
                            q.push(p->ivex);//��ͷ���������
                        }
                        p = p->jlink; //p��tlink��path2)�ƶ�
                    }
                }
            }
        }
    }
    cout << endl;
}

/*===============================�ڽӾ������ڽӱ��໥ת��=====================================*/
void AdjMatrixToAdjList(MTGraph gm, ALGraph &gl) { //�ڽӾ���ת�ڽӱ�
    int i, j, k;
    ArcNode *p;
    k = 0;
    gl.vexnum = gm.n;
    for (i = 0; i < gm.n; i++) {
        gl.vertices[i].data = gm.vertlist[i]; //�ڵ㸳ֵ
        gl.vertices[i].firstedge = NULL; //������ͷָ���ÿ�
    }
    for (i = 0; i < gm.n; i++)
        for (j = 0; j < gm.n; j++)
            if (gm.edge[i][j] == 1) {  //���ڱ�
                k++;
                p = new ArcNode;
                p->adjvex = j;
                p->info = gm.vertlist[j];
                p->nextarc = gl.vertices[i].firstedge;
                gl.vertices[i].firstedge = p;
            }
    gl.arcnum = k / 2;
}

void AdjListToAdjMatrix(ALGraph gl, MTGraph &gm) { //�ڽӱ�ת�ڽӾ���
    int i, j, k;
    ArcNode *p;
    gm.n = gl.vexnum;
    for (k = 0; k < gl.vexnum; k++)
        gm.vertlist[k] = gl.vertices[k].data;  //�ڵ㸳ֵ
    for (i = 0; i < gl.vexnum; i++)
        for (j = 0; j < gl.vexnum; j++)
            gm.edge[i][j] = 0;  //�����ʼ��Ϊ0
    for (i = 0; i < gl.vexnum; i++) {
        p = gl.vertices[i].firstedge;   //ȡ��һ���ڽӶ���
        while (p != NULL) {   //ȡ��һ���ڽӶ���
            gm.edge[i][p->adjvex] = 1;
            p = p->nextarc;
        }
    }
}

/*===============================�ڽӾ������ڽӶ��ر��໥ת��==================================*/
void MTtoAML(MTGraph MT, AMLGraph &AML) { //�ڽӾ���ת�ڽӶ��ر�
    int i, j, k;
    char va, vb;
    EBox *p;
    AML.vexnum = MT.n;
    AML.edgenum = MT.e;
    for (i = 0; i < AML.vexnum; i++)         /* ���춥������ */
    {
        AML.adjmulist[i].data = MT.vertlist[i];
        AML.adjmulist[i].firstedge = NULL;
        //printf("%c ", AML.adjmulist[i].data);
    }
    for (i = 0; i < AML.vexnum; i++) {       /* ����������� */
        for (j = 0; j < AML.vexnum; j++) {
            if (MT.edge[i][j] == 1 && i <= j) {
                p = (EBox *) malloc(sizeof(EBox));
                p->mark = unvisited;                /* ���ֵ */
                p->ivex = i;
                p->ilink = AML.adjmulist[i].firstedge; /* ���ڱ�ͷ */
                AML.adjmulist[i].firstedge = p;

                p->jvex = j;
                p->jlink = AML.adjmulist[j].firstedge; /* ���ڱ�ͷ */
                AML.adjmulist[j].firstedge = p;
            }
        }
    }
}

void AMLtoMT(AMLGraph AML, MTGraph &MT) { //�ڽӶ��ر�ת�ڽӾ���
    int i, j;
    EBox *p;
    MT.n = AML.vexnum;
    MT.e = AML.edgenum;
    for (i = 0; i < MT.n; i++) {
        MT.vertlist[i] = AML.adjmulist[i].data;
    }
    for (i = 0; i < MT.n; i++) {
        for (j = 0; j < MT.n; j++)
            MT.edge[i][j] = 0;
    }
    for (i = 0; i < AML.vexnum; i++) {
        p = AML.adjmulist[i].firstedge;
        while (p)
            if (p->ivex == i) /* �ߵ�i����ö����й� */
            {
                if (!p->mark) /* ֻ���һ�� */
                {
                    j = p->jvex;
                    MT.edge[i][j] = 1;
                    MT.edge[j][i] = 1;
                    p->mark = visited;
                }
                p = p->ilink;
            } else /* �ߵ�j����ö����й� */
            {
                if (!p->mark) /* ֻ���һ�� */
                {
                    j = p->ivex;
                    MT.edge[i][j] = 1;
                    MT.edge[j][i] = 1;
                    p->mark = visited;
                }
                p = p->jlink;
            }
    }
}

/*===============================�ڽӱ����ڽӶ��ر��໥ת��====================================*/
void AdjtoAML(ALGraph Adj1, AMLGraph &AML1) { //�ڽӱ�ת�ڽӶ��ر�
    int i;
    EBox *p2;
    ArcNode *p1;
    AML1.vexnum = Adj1.vexnum;
    AML1.edgenum = Adj1.arcnum;
    for (i = 0; i < AML1.vexnum; ++i)         /* ���춥������ */
    {
        AML1.adjmulist[i].data = Adj1.vertices[i].data;
        //printf("%c ", AML1.adjmulist[i].data);
        AML1.adjmulist[i].firstedge = NULL;
    }
    for (i = 0; i < Adj1.vexnum; i++) {
        p1 = Adj1.vertices[i].firstedge;
        while (p1 != NULL && i < p1->adjvex) {
            p2 = (EBox *) malloc(sizeof(EBox));
            p2->mark = unvisited;                /* ���ֵ */
            p2->ivex = i;
            p2->ilink = AML1.adjmulist[i].firstedge; /* ���ڱ�ͷ */
            AML1.adjmulist[i].firstedge = p2;

            p2->jvex = p1->adjvex;
            p2->jlink = AML1.adjmulist[p1->adjvex].firstedge; /* ���ڱ�ͷ */
            AML1.adjmulist[p1->adjvex].firstedge = p2;

            p1 = p1->nextarc;
        }
    }
}

void AMLtoAdj(AMLGraph AML2, ALGraph &Adj2) { //�ڽӶ��ر�ת�ڽӱ�
    int i;
    ArcNode *p1, *p2;
    Adj2.vexnum = AML2.vexnum;
    Adj2.arcnum = AML2.edgenum;
    for (i = 0; i < Adj2.vexnum; i++) {
        Adj2.vertices[i].data = AML2.adjmulist[i].data;
        Adj2.vertices[i].firstedge = NULL;
        //printf("%c ", Adj2.vertices[i].data);
    }
    EBox *p;
    MarkUnvizited(AML2); /* �ñߵķ��ʱ��Ϊδ������ */
    for (i = 0; i < AML2.vexnum; i++) {
        p = AML2.adjmulist[i].firstedge;
        while (p)
            if (p->ivex == i) /* �ߵ�i����ö����й� */
            {
                if (!p->mark) /* ֻ���һ�� */
                {
                    p1 = new ArcNode;
                    p1->adjvex = p->jvex;
                    p1->info = Adj2.vertices[p->jvex].data;
                    p1->nextarc = Adj2.vertices[i].firstedge;
                    Adj2.vertices[i].firstedge = p1;

                    p2 = new ArcNode;
                    p2->adjvex = i;
                    p2->info = Adj2.vertices[i].data;
                    p2->nextarc = Adj2.vertices[p->jvex].firstedge;
                    Adj2.vertices[p->jvex].firstedge = p2;
                    p->mark = visited;
                }
                p = p->ilink;
            } else /* �ߵ�j����ö����й� */
            {
                if (!p->mark) /* ֻ���һ�� */
                {
                    p1 = new ArcNode;
                    p1->adjvex = i;
                    p1->info = Adj2.vertices[i].data;
                    p1->nextarc = Adj2.vertices[p->ivex].firstedge;
                    Adj2.vertices[p->ivex].firstedge = p1;

                    p2 = new ArcNode;
                    p2->adjvex = p->ivex;
                    p2->info = Adj2.vertices[p->ivex].data;
                    p2->nextarc = Adj2.vertices[i].firstedge;
                    Adj2.vertices[i].firstedge = p2;
                    p->mark = visited;
                }
                p = p->jlink;
            }
    }
}

/*===============================������=============================*/
int main() {
    int choice = 1;
    while (choice) {
        printf("************ �� �� ͼ �� �� �� �� �� �� �� �� �� �� �� ************\n");
        printf("\t1.�ڽӾ���Ĳ���\n");
        printf("\t2.�ڽӱ�Ĳ���\n");
        printf("\t3.�ڽӶ��ر�Ĳ���\n");
        printf("\t4.�ڽӾ������ڽӱ��໥ת��\n");
        printf("\t5.�ڽӾ������ڽӶ��ر��໥ת��\n");
        printf("\t6.�ڽӱ����ڽӶ��ر��໥ת��\n");
        printf("\t0.�˳�\n");
        printf("\n������0-6,ѡ��Ҫִ�еĲ���:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                /*=============================�ڽӾ���Ĳ���===============================*/
                MTGraph MTgraph;
                CreateMGragh(&MTgraph); //��ͼ
                OutPutAdjMatrix(MTgraph);
                cout << endl << "*************************************" << endl;
                cout << "�ڽӾ���ĵݹ�������ȱ���˳��Ϊ��" << endl;
                DFSTraverse(MTgraph); //��ȱ���
                cout << endl << "*************************************" << endl;
                cout << "�ڽӾ���ķǵݹ�������ȱ���˳��Ϊ��" << endl;
                //DFSArr1(&MTgraph, 0);
                DFS(&MTgraph);
                cout << endl << "*************************************" << endl;
                cout << "�ڽӾ���Ĺ�����ȱ���˳��Ϊ��" << endl;
                BFSTraverse(MTgraph); //������ȱ���
                cout << endl << "*************************************" << endl;
            }
                system("pause");
                break;
            case 2: {
                /*===============================�ڽӱ�Ĳ���=================================*/
                ALGraph *G = new ALGraph;
                CreateAdjList(G);
                OutPutAdjList(G);
                char Vi;
                Vi = 'a';
                cout << endl << "*************************************" << endl;
                DFStraversal(G, Vi);
                cout << endl << "*************************************" << endl;
                st_DFS(G, 0);
                cout << endl << "*************************************" << endl;
                BFS(G, 0);
                cout << endl << "*************************************" << endl;
                printf("--------------------------------------\n");
            }
                system("pause");
                break;
            case 3: {
                /*===============================�ڽӶ��ر�Ĳ���=================================*/
                AMLGraph AML;
                GreateAMLGraph(&AML);
                Display(AML);
                cout << endl << "*************************************" << endl;
                cout << "�ڽӶ��ر�ĵݹ�������ȱ���˳��Ϊ��" << endl;
                DFSTraverse2(AML);
                cout << endl << "*************************************" << endl;
                cout << "�ڽӶ��ر�ķǵݹ�������ȱ���˳��Ϊ��" << endl;
                DFSTraverse_non_recursive(AML, 0);
                cout << "*************************************" << endl;
                cout << "�ڽӶ��ر�Ĺ�����ȱ���˳��Ϊ��" << endl;
                BFSTraverse(AML, 0);
                printf("\n*************************************\n");

            }
                system("pause");
                break;
            case 4: {
                /*=======================�ڽӾ������ڽӱ��໥ת��=======================*/
                ALGraph gl1, gl2;
                MTGraph gm1, gm2;
                //�ڽӱ�ת�ڽӾ���
                CreateAdjList(&gl1);
                AdjListToAdjMatrix(gl1, gm1);
                //printf("�ڽӾ���ת��Ϊ�ڽӱ�Ϊ��\n");
                OutPutAdjList(&gl1);
                OutPutAdjMatrix(gm1);
                //�ڽӾ���ת�ڽӱ�
                CreateMGragh(&gm2);
                AdjMatrixToAdjList(gm2, gl2);
                OutPutAdjMatrix(gm2);
                OutPutAdjList(&gl2);
                printf("--------------------------------------\n");
            }
                system("pause");
                break;
            case 5: {
                /*=======================�ڽӾ������ڽӶ��ر��໥ת��=======================*/
                MTGraph MT1, MT2;
                AMLGraph AML1, AML2;

                CreateMGragh(&MT1);
                MTtoAML(MT1, AML1);
                OutPutAdjMatrix(MT1);
                Display(AML1);
                printf("**********************************\n");
                GreateAMLGraph(&AML2);
                AMLtoMT(AML2, MT2);
                Display(AML2);
                OutPutAdjMatrix(MT2);
            }
                system("pause");
                break;
            case 6: {
                /*=======================�ڽӱ����ڽӶ��ر��໥ת��=======================*/
                ALGraph Adj1, Adj2;
                AMLGraph AML3, AML4;
                CreateAdjList(&Adj1);
                AdjtoAML(Adj1, AML3);
                OutPutAdjList(&Adj1);
                Display(AML3);
                printf("************************************\n");
                GreateAMLGraph(&AML4);
                AMLtoAdj(AML4, Adj2);
                Display(AML4);
                OutPutAdjList(&Adj2);
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
