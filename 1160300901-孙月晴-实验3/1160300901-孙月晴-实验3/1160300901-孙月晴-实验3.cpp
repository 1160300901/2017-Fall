#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <afxres.h>
#include <queue>
#include <stack>

using namespace std;

/*=======================邻接矩阵存储============================*/
#define NumVertices 15
typedef struct {
    char vertlist[NumVertices];
    int edge[NumVertices][NumVertices];
    int n, e;
} MTGraph;

int visited1[NumVertices];//访问标记数组
int visited2[NumVertices];
int dfn[NumVertices];//顶点编号
int Count = 1;//计数
int s[NumVertices + 1];//非递归栈定义
int Queue[NumVertices + 1];

/*======================图的邻接表存储表示=======================*/
#define MAX 20
typedef struct ArcNode {
    int adjvex;       //弧的邻接定点
    char info;        //邻接点值
    struct ArcNode *nextarc;   //指向下一条弧的指针
} ArcNode;

typedef struct Vnode {          //节点信息
    char data;
    ArcNode *firstedge;
} AdjList[MAX];

typedef struct {
    AdjList vertices;
    int vexnum;              //节点数
    int arcnum;              //边数
} ALGraph;

/*=========================邻接多重表存储========================*/
#define MAX_VERTEX_NUM 20
typedef enum {
    unvisited, visited
} VisitIf;

typedef struct EBox {
    VisitIf mark; //边访问标记
    int ivex, jvex; //该边依附的两个顶点的位置
    struct EBox *ilink, *jlink; //分别指向依附这两个顶点的下一条边
    int *info; //该边信息指针
} EBox;

typedef struct VexBox {
    char data;
    EBox *firstedge; //指向第一条依附于该顶点的边
} VexBox;

typedef struct {
    VexBox adjmulist[MAX_VERTEX_NUM];
    int vexnum, edgenum; //无向图的当前顶点数和边数
} AMLGraph;

/*========================从文件建立邻接矩阵=====================*/
void CreateMGragh(MTGraph *G) {
    int i, j, k, w;
    FILE *fp;
    fp = fopen("MTgraph.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败！\n");
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

/*=========================打印邻接矩阵==========================*/
void OutPutAdjMatrix(MTGraph gm) {
    cout << "图的邻接矩阵如下：" << endl;
    for (int i = 0; i < gm.n; i++) {
        for (int j = 0; j < gm.n; j++) {
            printf("%d ", gm.edge[i][j]);
        }
        cout << endl;
    }
}

/*====================邻接矩阵的递归深度优先搜索=================*/
void DFSArr(MTGraph *G, int i) {
    int j;
    cout << G->vertlist[i] << " ";
    visited1[i] = 1;  //访问过标记1
    dfn[i] = Count;
    Count++;
    for (j = 0; j < G->n; j++) //依次搜索vi的邻接点
    {
        if ((G->edge[i][j] == 1) && !visited1[j])
            DFSArr(G, j);
    }
}

void DFSTraverse(MTGraph G) { //深度递归主函数
    int i;
    for (i = 0; i < G.n; i++)
        visited1[i] = 0;
    for (i = 0; i < G.n; i++) {
        if (!visited1[i])
            DFSArr(&G, i);
    }
}

/*==================邻接矩阵的非递归深度优先搜索=================*/
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

/*==================邻接矩阵的广度优先搜索=======================*/
void BFSArr(MTGraph *G, int k) {
    int i, j, Front = 0, Rear = 0;
    cout << G->vertlist[k] << " ";
    visited2[k] = 1;
    Queue[++Rear] = k;  //当前顶点入队
    while (Front != Rear) {
        i = Queue[++Front]; //顶点出队
        for (j = 0; j < G->n; j++) {
            if (G->edge[i][j] == 1 && !visited2[j]) {
                cout << G->vertlist[j] << " ";
                visited2[j] = 1;
                Queue[++Rear] = j; //顶点入队
            }
        }
    }
}

void BFSTraverse(MTGraph G) { //广度优先遍历主程序
    int i;
    for (i = 0; i < G.n; i++)
        visited2[i] = 0;
    for (i = 0; i < G.n; i++) {
        if (!visited2[i])
            BFSArr(&G, i);
    }
}

/*=========================创建邻接表============================*/
void CreateAdjList(ALGraph *G) {
    int i, s, d;
    ArcNode *p1, *p2;
    FILE *fp;                                /*指向文件的指针*/
    if ((fp = fopen("ALgraph.txt", "r")) == NULL)  /*打开文件，并判断打开是否正常*/
    {
        printf("不能打开文件\n");    /*没打开*/
        exit(1);                          /*退出*/
    }
    fscanf(fp, "%2d%2d", &G->vexnum, &G->arcnum);                          /*读入记录数*/
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
        //因为是无向图，所以有两次建立边表的过程
    }
    fclose(fp);
}

/*=========================打印邻接表============================*/
void OutPutAdjList(ALGraph *G) {
    int i;
    ArcNode *p;
    cout << "图的邻接表如下：" << endl;
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

/*=================邻接表的递归深度优先遍历======================*/
void DFS(ALGraph *G, int i, int visited[]) {//从第v个顶点出发递归地深度优先搜索遍历无向图G。

    cout << G->vertices[i].data << " ";
    visited[i] = 1;               //设置访问标志数组（初始化未访问为0，已访问为1）
    ArcNode *p = new ArcNode;
    p = G->vertices[i].firstedge;  //指向第一条依附于该顶点vi的边的指针

    if (G->vertices[i].firstedge && !visited[p->adjvex])
    //指向第一条依附于该顶点vi的边的指针非空，且顶点未被访问
    {
        DFS(G, p->adjvex, visited);//递归：自我调用
    }
}

void DFStraversal(ALGraph *G, char c) { //深度优先遍历
    cout << "邻接表的递归深度优先遍历结果为:" << endl;
    int visited[MAX];
    for (int i = 0; i < G->vexnum; i++) {
        visited[i] = 0;  //全部初始化为0，即未访问状态
    }
    for (int i = 0; i < G->vexnum; i++) {
        if (G->vertices[i].data == c)//根据字符查找序号
        {
            DFS(G, i, visited);  //调用上面的函数
            break;
        }
    }
    //继续访问未被访问的结点，直到图中所有顶点都被访问完毕
    for (int i = 0; i < G->vexnum; i++) {
        if (visited[i] == 0)
            DFS(G, i, visited);
    }
    cout << endl;
}

/*===================邻接表的非递归深度优先遍历==================*/
void st_DFS(ALGraph *G, int k) {
    bool Visited[MAX] = {false};
    int stack[MAX];        //栈
    int top = -1;

    ArcNode *pEdge = NULL;
    int Pre[MAX];
    int index = -1;        //Pre数组的下标

    Visited[k] = true;            //起始顶点标记已访问
    cout << G->vertices[k].data << " ";            //输出起始顶点

    //把所有与起始顶点关联的顶点存入Per数组中
    pEdge = G->vertices[k].firstedge;
    while (NULL != pEdge) {
        Pre[++index] = pEdge->adjvex;
        pEdge = pEdge->nextarc;
    }
    //逆序把Pre数组中的顶点加入栈中
    for (int i = index; i >= 0; i--) {
        top++;
        stack[top] = Pre[i];
        Visited[Pre[i]] = 1;        //入栈的顶点标记已访问
    }

    while (top > -1) {
        int TopVertex = stack[top];
        top--;
        cout << G->vertices[TopVertex].data << " ";        //输出栈顶节点

        pEdge = G->vertices[TopVertex].firstedge;        //把所有与栈底顶点关联且未访问过的顶点入栈
        index = -1;    //重置Pre数组下标
        while (NULL != pEdge) {
            if (Visited[pEdge->adjvex] == 0)
                Pre[++index] = pEdge->adjvex;
            pEdge = pEdge->nextarc;
        }
        for (int i = index; i >= 0; i--)        //逆序把Pre数组中的顶点加入到栈中
        {
            top++;
            stack[top] = Pre[i];
            Visited[Pre[i]] = 1;
        }
    }//end while(top > -1)
}

/*======================邻接表的广度优先遍历=====================*/
int visited4[MAX];//访问标志向量是全局量
void BFS(ALGraph *&G, int v) { //广度优先搜索
    queue<int> Q;
    int j;
    ArcNode *p;
    if (!visited4[v]) {
        cout << G->vertices[v].data << " ";
        visited4[v] = 1;
        Q.push(v); //被访问顶点入队
    }
    while (!Q.empty()) //队列不空进循环
    {
        v = Q.front();//读队头
        Q.pop(); //出队
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

/*======================从文件建立邻接多重表=====================*/
int LocateVex(AMLGraph G, char u) { /* 初始条件：无向图G存在，u和G中顶点有相同特征 */
    /* 操作结果：若G中存在顶点u，则返回该顶点在无向图中位置；否则返回-1 */
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
    FILE *fp;                                /*指向文件的指针*/
    if ((fp = fopen("AMLgraph.txt", "rb")) == NULL)  /*打开文件，并判断打开是否正常*/
    {
        printf("不能打开文件\n");    /*没打开*/
        exit(1);                          /*退出*/
    }
    fscanf(fp, "%2d %2d", &G->vexnum, &G->edgenum);                          /*读入记录数*/
    for (i = 0; i < (*G).vexnum; ++i)         /* 构造顶点向量 */
    {
        fscanf(fp, " %c", &G->adjmulist[i].data);
        (*G).adjmulist[i].firstedge = NULL;
    }
    for (k = 0; k <= (*G).edgenum; ++k)        /* 构造表结点链表 */
    {
        fscanf(fp, "%c%c ", &va, &vb);
        i = LocateVex(*G, va);            /* 一端 */
        j = LocateVex(*G, vb);            /* 另一端 */
        p = (EBox *) malloc(sizeof(EBox));
        p->mark = unvisited;                /* 设初值 */
        p->ivex = i;
        p->ilink = (*G).adjmulist[i].firstedge; /* 插在表头 */
        (*G).adjmulist[i].firstedge = p;
        p->jvex = j;
        p->jlink = (*G).adjmulist[j].firstedge; /* 插在表头 */
        (*G).adjmulist[j].firstedge = p;
    }
    fclose(fp);                                  /*关闭文件*/
}

/*=========================打印邻接多重表========================*/
void MarkUnvizited(AMLGraph &G) { /* 置边的访问标记为未被访问 */
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

void Display(AMLGraph G) { /* 输出无向图的邻接多重表G */
    printf("图的邻接多重表如下：\n");
    int i;
    EBox *p;
    MarkUnvizited(G); /* 置边的访问标记为未被访问 */
    printf("有%d个顶点：\n", G.vexnum);
    for (i = 0; i < G.vexnum; ++i)
        printf("%c ", G.adjmulist[i].data);
    printf("\n有%d条边:\n", G.edgenum);
    for (i = 0; i < G.vexnum; i++) {
        p = G.adjmulist[i].firstedge;
        while (p)
            if (p->ivex == i) /* 边的i端与该顶点有关 */
            {
                if (!p->mark) /* 只输出一次 */
                {
                    printf("%c－%c   \n", G.adjmulist[i].data, G.adjmulist[p->jvex].data);
                    p->mark = visited;
                }
                p = p->ilink;
            } else /* 边的j端与该顶点有关 */
            {
                if (!p->mark) /* 只输出一次 */
                {
                    printf("%c－%c   \n", G.adjmulist[p->ivex].data, G.adjmulist[i].data);
                    p->mark = visited;
                }
                p = p->jlink;
            }
    }
}

/*=========================邻接多重表的递归深搜===================*/
int visited3[MAX_VERTEX_NUM];//访问标志数组

void AMLDFS(AMLGraph G, int i) { //递归实现深度优先遍历
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
    //循环是为保证每个顶点都能被访问到
    for (i = 0; i < G.vexnum; i++)
        if (!visited3[i])
            AMLDFS(G, i);

}

/*=======================邻接多重表的非递归深搜===================*/
bool visited5[MAX];

int AdjVexNotVisit(AMLGraph &G, int i){//查找未访问的邻接顶点
    EBox *p = G.adjmulist[i].firstedge; //p指向依附于顶点i的第一条边
    while (p) {
        if (i == p->ivex)   //i就是该边的头结点
        {
            if (!visited5[p->jvex])
                return p->jvex;  //存在未访问的结点，返回该点的存储位置
            else {
                p = p->ilink; //该边不存在未访问的结点，p沿hlink（path1）移动
            }
        } else    //i就是该边的尾结点
        {
            if (!visited5[p->ivex])
                return p->ivex;  //存在未访问的结点，返回该点的存储位置
            else {
                p = p->jlink; //该边不存在未访问的结点，p沿tlink（path2）移动
            }
        }
    }
    return -1;  //该顶点不存在未访问的邻接顶点，返回-1
}

void DFSTraverse_non_recursive(AMLGraph &G, int start) {
    int i;

    for (i = 0; i < G.vexnum; i++) {
        visited5[i] = false;   //辅助访问标志数组初始化
    }
    for (i = 0; i < G.vexnum; ++i) {
        if (!visited5[start]) //尚未访问
        {
            visited5[start] = true;    //设置访问标志
            cout << G.adjmulist[start].data << " ";   //访问输出该点的数据
            stack<int> s;  //置空的辅助栈s
            int pos, w;
            EBox *p;
            s.push(start);  //将该点进栈
            while (!s.empty())   //当栈非空的时候循环
            {
                pos = s.top();    //顶点pos为s的栈顶存放的点
                w = AdjVexNotVisit(G, pos);    //找到顶点pos尚未访问的邻接点
                if (w >= 0)    //如果存在这样的点
                {
                    visited5[w] = true;    //设置访问标志
                    cout << G.adjmulist[w].data << " ";   //访问输出该点的数据
                    s.push(w);  //将该点进栈
                } else    //如果不存在这样的点
                {
                    s.pop();    //将该点出栈（因为没有了循环访问的必要）
                }
                //继续循环，直到所有的顶点都被访问过
            }
        }
    }
    cout << endl;
}

/*==========================邻接多重表的广搜=======================*/
void BFSTraverse(AMLGraph &G, int start) {
    bool visited[MAX];
    int i;
    for (i = 0; i < G.vexnum; i++)
        visited[i] = false;   //辅助访问标志数组初始化
    for (i = 0; i < G.vexnum; i++) {
        if (!visited[start]) //尚未访问
        {
            visited[start] = true;    //设置访问标志
            cout << G.adjmulist[start].data << " ";   //访问输出该点的数据
            queue<int> q;   //置空的辅助队列Q
            EBox *p;
            int pos;
            q.push(start);  //入队列
            while (!q.empty())   //当队非空
            {
                pos = q.front();  //队头元素出队并置为pos
                q.pop();
                p = G.adjmulist[pos].firstedge;    //p指向依附于顶点pos的第一条边
                while (p) {
                    if (p->ivex == pos) //如果顶点pos就是该边的头结点
                    {
                        if (!visited[p->jvex])    //如果该边的尾结点未访问
                        {
                            visited[p->jvex] = true;   //设置访问标志
                            cout << G.adjmulist[p->jvex].data << " ";   //访问输出该尾结点的数据
                            q.push(p->jvex); //该尾结点点入队列
                        }
                        p = p->ilink; //p沿hlink（path1)移动
                    } else    //如果顶点pos就是该边的尾结点
                    {
                        if (!visited[p->ivex])    //如果该边的头结点未访问
                        {
                            visited[p->ivex] = true;   //设置访问标志
                            cout << G.adjmulist[p->ivex].data << " ";  //访问输出该头结点的数据
                            q.push(p->ivex);//该头结点点入队列
                        }
                        p = p->jlink; //p沿tlink（path2)移动
                    }
                }
            }
        }
    }
    cout << endl;
}

/*===============================邻接矩阵与邻接表相互转化=====================================*/
void AdjMatrixToAdjList(MTGraph gm, ALGraph &gl) { //邻接矩阵转邻接表
    int i, j, k;
    ArcNode *p;
    k = 0;
    gl.vexnum = gm.n;
    for (i = 0; i < gm.n; i++) {
        gl.vertices[i].data = gm.vertlist[i]; //节点赋值
        gl.vertices[i].firstedge = NULL; //边链表头指针置空
    }
    for (i = 0; i < gm.n; i++)
        for (j = 0; j < gm.n; j++)
            if (gm.edge[i][j] == 1) {  //存在边
                k++;
                p = new ArcNode;
                p->adjvex = j;
                p->info = gm.vertlist[j];
                p->nextarc = gl.vertices[i].firstedge;
                gl.vertices[i].firstedge = p;
            }
    gl.arcnum = k / 2;
}

void AdjListToAdjMatrix(ALGraph gl, MTGraph &gm) { //邻接表转邻接矩阵
    int i, j, k;
    ArcNode *p;
    gm.n = gl.vexnum;
    for (k = 0; k < gl.vexnum; k++)
        gm.vertlist[k] = gl.vertices[k].data;  //节点赋值
    for (i = 0; i < gl.vexnum; i++)
        for (j = 0; j < gl.vexnum; j++)
            gm.edge[i][j] = 0;  //矩阵初始化为0
    for (i = 0; i < gl.vexnum; i++) {
        p = gl.vertices[i].firstedge;   //取第一个邻接顶点
        while (p != NULL) {   //取下一个邻接顶点
            gm.edge[i][p->adjvex] = 1;
            p = p->nextarc;
        }
    }
}

/*===============================邻接矩阵与邻接多重表相互转化==================================*/
void MTtoAML(MTGraph MT, AMLGraph &AML) { //邻接矩阵转邻接多重表
    int i, j, k;
    char va, vb;
    EBox *p;
    AML.vexnum = MT.n;
    AML.edgenum = MT.e;
    for (i = 0; i < AML.vexnum; i++)         /* 构造顶点向量 */
    {
        AML.adjmulist[i].data = MT.vertlist[i];
        AML.adjmulist[i].firstedge = NULL;
        //printf("%c ", AML.adjmulist[i].data);
    }
    for (i = 0; i < AML.vexnum; i++) {       /* 构造表结点链表 */
        for (j = 0; j < AML.vexnum; j++) {
            if (MT.edge[i][j] == 1 && i <= j) {
                p = (EBox *) malloc(sizeof(EBox));
                p->mark = unvisited;                /* 设初值 */
                p->ivex = i;
                p->ilink = AML.adjmulist[i].firstedge; /* 插在表头 */
                AML.adjmulist[i].firstedge = p;

                p->jvex = j;
                p->jlink = AML.adjmulist[j].firstedge; /* 插在表头 */
                AML.adjmulist[j].firstedge = p;
            }
        }
    }
}

void AMLtoMT(AMLGraph AML, MTGraph &MT) { //邻接多重表转邻接矩阵
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
            if (p->ivex == i) /* 边的i端与该顶点有关 */
            {
                if (!p->mark) /* 只输出一次 */
                {
                    j = p->jvex;
                    MT.edge[i][j] = 1;
                    MT.edge[j][i] = 1;
                    p->mark = visited;
                }
                p = p->ilink;
            } else /* 边的j端与该顶点有关 */
            {
                if (!p->mark) /* 只输出一次 */
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

/*===============================邻接表与邻接多重表相互转化====================================*/
void AdjtoAML(ALGraph Adj1, AMLGraph &AML1) { //邻接表转邻接多重表
    int i;
    EBox *p2;
    ArcNode *p1;
    AML1.vexnum = Adj1.vexnum;
    AML1.edgenum = Adj1.arcnum;
    for (i = 0; i < AML1.vexnum; ++i)         /* 构造顶点向量 */
    {
        AML1.adjmulist[i].data = Adj1.vertices[i].data;
        //printf("%c ", AML1.adjmulist[i].data);
        AML1.adjmulist[i].firstedge = NULL;
    }
    for (i = 0; i < Adj1.vexnum; i++) {
        p1 = Adj1.vertices[i].firstedge;
        while (p1 != NULL && i < p1->adjvex) {
            p2 = (EBox *) malloc(sizeof(EBox));
            p2->mark = unvisited;                /* 设初值 */
            p2->ivex = i;
            p2->ilink = AML1.adjmulist[i].firstedge; /* 插在表头 */
            AML1.adjmulist[i].firstedge = p2;

            p2->jvex = p1->adjvex;
            p2->jlink = AML1.adjmulist[p1->adjvex].firstedge; /* 插在表头 */
            AML1.adjmulist[p1->adjvex].firstedge = p2;

            p1 = p1->nextarc;
        }
    }
}

void AMLtoAdj(AMLGraph AML2, ALGraph &Adj2) { //邻接多重表转邻接表
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
    MarkUnvizited(AML2); /* 置边的访问标记为未被访问 */
    for (i = 0; i < AML2.vexnum; i++) {
        p = AML2.adjmulist[i].firstedge;
        while (p)
            if (p->ivex == i) /* 边的i端与该顶点有关 */
            {
                if (!p->mark) /* 只输出一次 */
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
            } else /* 边的j端与该顶点有关 */
            {
                if (!p->mark) /* 只输出一次 */
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

/*===============================主函数=============================*/
int main() {
    int choice = 1;
    while (choice) {
        printf("************ 无 向 图 的 存 储 结 构 的 建 立 与 搜 索 ************\n");
        printf("\t1.邻接矩阵的操作\n");
        printf("\t2.邻接表的操作\n");
        printf("\t3.邻接多重表的操作\n");
        printf("\t4.邻接矩阵与邻接表相互转化\n");
        printf("\t5.邻接矩阵与邻接多重表相互转化\n");
        printf("\t6.邻接表与邻接多重表相互转化\n");
        printf("\t0.退出\n");
        printf("\n请输入0-6,选择要执行的操作:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                /*=============================邻接矩阵的操作===============================*/
                MTGraph MTgraph;
                CreateMGragh(&MTgraph); //建图
                OutPutAdjMatrix(MTgraph);
                cout << endl << "*************************************" << endl;
                cout << "邻接矩阵的递归深度优先遍历顺序为：" << endl;
                DFSTraverse(MTgraph); //深度遍历
                cout << endl << "*************************************" << endl;
                cout << "邻接矩阵的非递归深度优先遍历顺序为：" << endl;
                //DFSArr1(&MTgraph, 0);
                DFS(&MTgraph);
                cout << endl << "*************************************" << endl;
                cout << "邻接矩阵的广度优先遍历顺序为：" << endl;
                BFSTraverse(MTgraph); //广度优先遍历
                cout << endl << "*************************************" << endl;
            }
                system("pause");
                break;
            case 2: {
                /*===============================邻接表的操作=================================*/
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
                /*===============================邻接多重表的操作=================================*/
                AMLGraph AML;
                GreateAMLGraph(&AML);
                Display(AML);
                cout << endl << "*************************************" << endl;
                cout << "邻接多重表的递归深度优先遍历顺序为：" << endl;
                DFSTraverse2(AML);
                cout << endl << "*************************************" << endl;
                cout << "邻接多重表的非递归深度优先遍历顺序为：" << endl;
                DFSTraverse_non_recursive(AML, 0);
                cout << "*************************************" << endl;
                cout << "邻接多重表的广度优先遍历顺序为：" << endl;
                BFSTraverse(AML, 0);
                printf("\n*************************************\n");

            }
                system("pause");
                break;
            case 4: {
                /*=======================邻接矩阵与邻接表相互转化=======================*/
                ALGraph gl1, gl2;
                MTGraph gm1, gm2;
                //邻接表转邻接矩阵
                CreateAdjList(&gl1);
                AdjListToAdjMatrix(gl1, gm1);
                //printf("邻接矩阵转化为邻接表为：\n");
                OutPutAdjList(&gl1);
                OutPutAdjMatrix(gm1);
                //邻接矩阵转邻接表
                CreateMGragh(&gm2);
                AdjMatrixToAdjList(gm2, gl2);
                OutPutAdjMatrix(gm2);
                OutPutAdjList(&gl2);
                printf("--------------------------------------\n");
            }
                system("pause");
                break;
            case 5: {
                /*=======================邻接矩阵与邻接多重表相互转化=======================*/
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
                /*=======================邻接表与邻接多重表相互转化=======================*/
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
