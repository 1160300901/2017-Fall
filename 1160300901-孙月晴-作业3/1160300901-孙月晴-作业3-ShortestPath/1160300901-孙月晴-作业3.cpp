#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <iomanip>

using namespace std;
#define Max 15
#define up 999  //路径不存在
int cost[Max][Max];//Floyd 算法中最短路径矩阵
int P[Max][Max];//Floyd 算法中最短路径

/*=======================邻接矩阵存储============================*/
typedef struct {
    int vertlist[Max];
    int edge[Max][Max];
    int n, e;
} MTGraph;

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
        fscanf(fp, " %d", &G->vertlist[i]);
    for (i = 0; i < G->n; i++) {
        for (j = 0; j < G->n; j++)
            if (i == j)
                G->edge[i][j] = 0;
            else
                G->edge[i][j] = up;
    }
    for (k = 0; k < G->e; k++) {
        fscanf(fp, "%6d%6d%6d", &i, &j, &w);
        G->edge[i][j] = w;
    }
    fclose(fp);
}

/*=========================1 打印邻接矩阵==========================*/
void OutPutAdjMatrix(MTGraph gm) {
    cout << "图的各顶点间距离：" << endl;
    for (int i = 0; i < gm.n; i++) {
        for (int j = 0; j < gm.n; j++) {
            if (gm.edge[i][j] == up)
                printf("∞ ");
            else
                printf("%d ", gm.edge[i][j]);
        }
        cout << endl;
    }
}

/*=========================2 Dijkstra实现单源最短路径==========================*/
void DijkstraPath(MTGraph g, int *dist, int *path, int v0) {
    int i, j, k;
    bool *visited = (bool *) malloc(sizeof(bool) * g.n);
    for (i = 0; i < g.n; i++)     //初始化
    {
        if (g.edge[v0][i] > 0 && i != v0) {
            dist[i] = g.edge[v0][i];
            path[i] = v0;     //path记录最短路径上从v0到i的前一个顶点
        } else {
            dist[i] = up;    //若i不与v0直接相邻，则权值置为无穷大
            path[i] = -1;
        }
        visited[i] = false;
        path[v0] = v0;
        dist[v0] = 0;
    }
    visited[v0] = true;
    for (i = 1; i < g.n; i++)     //循环扩展n-1次
    {
        int min = up;
        int u;
        for (j = 0; j < g.n; j++)    //寻找未被扩展的权值最小的顶点
        {
            if (visited[j] == false && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        visited[u] = true;
        for (k = 0; k < g.n; k++)   //更新dist数组的值和路径的值
        {
            if (visited[k] == false && g.edge[u][k] > 0 && min + g.edge[u][k] < dist[k]) {
                dist[k] = min + g.edge[u][k];
                path[k] = u;
            }
        }
    }
}

void showPath(int *path, int v, int v0) { //打印最短路径上的各个顶点
    stack<int> s;
    int u = v;
    while (v != v0) {
        s.push(v);
        v = path[v];
    }
    s.push(v);
    printf("最短路径为： ");
    while (!s.empty()) {

        cout << s.top() << " ";
        s.pop();
    }
}

/*========================3 全局最短路径的Floyd-Warshall 算法=====================*/
void shortestpath(MTGraph &g) {
    int k, i, j, w;
    for (i = 0; i < g.n; i++) {
        for (j = 0; j < g.n; j++) {
            cost[i][j] = g.edge[i][j];
            P[i][j] = 0;
        }
    }
    for (k = 0; k < g.n; k++) {
        for (i = 0; i < g.n; i++) {
            for (j = 0; j < g.n; j++) {
                if (cost[i][k] + cost[k][j] < cost[i][j]) {
                    cost[i][j] = cost[i][k] + cost[k][j];
                    P[i][j] = k;
                }
            }
        }
    }
    printf("最短距离矩阵：\n");
    for (i = 0; i < g.n; i++) {
        for (j = 0; j < g.n; j++) {
            if (i == j) {
                printf("0 ");
            } else if (cost[i][j] == 999) {
                printf("∞ ");
            } else
                printf("%d ", cost[i][j]);
        }
        printf("\n");
    }
    cout << setw(2) << "\n任意两个顶点间最短距离和路径:\n";
    for (i = 0; i < g.n; i++)/*输出最短路径*/
        for (j = 0; j < g.n; j++)
            if (i != j) {
                cout << "" << i << "->" << j << ":";
                if (cost[i][j] == up) {
                    if (i != j)
                        cout << setw(2) << "不存在路径" << endl;
                } else {
                    cout << setw(2) << "路径长度为：" <<cost[i][j] << " ";
                    cout << setw(2) << " 路径为" << i << " ";
                    w = P[i][j];
                    while (w != 0) {
                        cout << w << " ";
                        w = P[w][j];
                    }
                    cout << j << endl;
                }
            }
}

void getPath(MTGraph G, int i, int j) {
    if (i == j) return;
    if (P[i][j] == 0) printf("%d ", j);
    else {
        getPath(G, i, P[i][j]);
        getPath(G, P[i][j], j);
    }
}

/*========================4 单目标最短路径的Dijkstra算法===========================*/
void danmubiao(MTGraph G, int v, int n) {
    int ds[Max], p[Max];
    int s[Max];
    int min, i, j, u, w;
    for (i = 0; i < n; i++) {
        ds[i] = G.edge[v][i];  //距离初始化
        s[i] = 0;          //S[ ]置空
        if (G.edge[v][i] < up)//路径初始化
            p[i] = v;
        else
            p[i] = -1;
    }
    s[v] = 1;
    p[v] = 0; //源点编号 V放入S中
    for (i = 0; i < n; i++) //循环直到所有顶点的最短路径都求出
    {
        min = up;
        u = -1;
        for (j = 0; j < n; j++)
            if (s[j] == 0 && ds[j] < min) //选取不在S中且具有最小距离的顶点U
            {
                u = j;
                min = ds[j];
            }
        if (u != -1) //找到最小距离的顶点U
        {
            s[u] = 1; //将顶点U加入S中
            for (j = 0; j < n; j++)//修改不在S中的顶点的距离
                if (s[j] == 0)
                    if (G.edge[u][j] < up && ds[u] + G.edge[u][j] < ds[j]) {
                        ds[j] = ds[u] + G.edge[u][j];
                        p[j] = u;
                    }
        }
    }
    cout << setw(2) << "\n单目标最短路径:\n";
    for (i = 0; i < n; i++)//输出最短路径的长度,路径逆序输出
    {
        if (i != v) {
            cout << "" << i << "->" << v << ":";
            if (s[i] == 1) {
                cout << setw(2) << "路径长度为：" << setw(2) << ds[i] << " ,";
                w = i;
                cout << setw(2) << "路径顺序为：";
                while (w != v) //一直加溯到初始顶点
                {
                    cout << w << ",";
                    w = p[w];
                }
                cout << w << endl;
            } else
                cout << "不存在路径" << endl;
        }
    }
}

/*=========================5 单顶点对间最短路径==========================*/
void floyed(MTGraph MTG, int n, int a, int b) {
    int G[Max][Max], p[Max][Max];
    int i, j, k, w;
    for (i = 0; i < n; i++)//置初值
        for (j = 0; j < n; j++) {
            G[i][j] = MTG.edge[i][j];
            p[i][j] = -1;
        }
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (G[i][j] > G[i][k] + G[k][j]) {
                    G[i][j] = G[i][k] + G[k][j];
                    p[i][j] = k;
                }
    }
    if (a != b) {
        cout << setw(2) << " " << a << "->" << b << ":";
        if (G[a][b] == up) {
            if (a != b)
                cout << setw(2) << "不存在路径" << endl;
        } else {
            cout << setw(2) << "路径长度为：" << G[a][b] << " ,";
            cout << setw(2) << "路径为：" << a << " ";
            w = p[a][b];
            while (w != -1) {
                cout << w << " ";
                w = p[w][b];
            }
            cout << b << endl;
        }
        cout << setw(2) << " " << b << "->" << a << ":";
        if (G[b][a] == up) {
            if (a != b)
                cout << setw(2) << "不存在路径" << endl;
        } else {
            cout << setw(2) << "路径长度为：" << G[b][a] << " ,";
            cout << setw(2) << "路径为：" << b << " ";
            w = p[b][a];
            while (w != -1) {
                cout << w << " ";
                w = p[w][a];
            }
            cout << a << endl;
        }
    }
}

/*========================6 有向图的可达矩阵===========================*/
void TransitiveClosure(MTGraph G, int t[Max][Max]) {//寻找可达矩阵
    for (int i = 0; i < G.n; i++)//进行遍历
        for (int j = 0; j < G.n; j++) {
            if ((i == j) || (G.edge[i][j]) != up)//发现可达
                t[i][j] = 1;
            else
                t[i][j] = 0;
        }
    for (int k = 0; k < G.n; k++)
        for (int i = 0; i < G.n; i++)
            for (int j = 0; j < G.n; j++)
                t[i][j] = t[i][j] || (t[i][k] && t[k][j]);
    printf("可达矩阵为：\n");
    for (int i = 0; i < G.n; i++) {
        for (int j = 0; j < G.n; j++)
            printf("%d ", t[i][j]);
        printf("\n");
    }
}

int main() {
    MTGraph G;
    int t[Max][Max];
    CreateMGragh(&G);

    int choice = 1;
    while (choice) {
        printf("************ 最 短 路 径 算 法 的 实 现 ************\n");
        printf("\t1.显示存储的图的各顶点间距离\n");
        printf("\t2.Dijkstra实现单源最短路径\n");
        printf("\t3.Floyd-Warshall实现全局最短路径\n");
        printf("\t4.Dijkstra实现单目标最短路径\n");
        printf("\t5.Floyd-Warshall实现单顶点对间最短路径\n");
        printf("\t6.Floyd-Warshall实现有向图的可达矩阵\n");
        printf("\t0.退出\n");
        printf("\n请输入0-5,选择要执行的操作:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                /*=====================1 显示存储的图的各顶点间距离===================*/
                OutPutAdjMatrix(G);
            }
                system("pause");
                break;
            case 2: {
                /*====================2 Dijkstra实现单源最短路径======================*/
                int *dist = (int *) malloc(sizeof(int) * G.n);
                int *path = (int *) malloc(sizeof(int) * G.n);
                printf("顶点0(默认)到各顶点的最短路径和最短距离：\n");
                DijkstraPath(G, dist, path, 0);
                for (int i = 0; i < G.n; i++) {
                    if (i != 0) {
                        showPath(path, i, 0);
                        printf(" 最短距离为： ");
                        if (dist[i] == up)
                            cout << "∞" << endl;
                        else
                            cout << dist[i] << endl;
                    }
                }
            }
                system("pause");
                break;
            case 3: {
                /*=================3 全局最短路径的Floyd-Warshall 算法===============*/
                shortestpath(G);
                int n1, n2;
                printf("请输入两个顶点求出它们之间的最短路径: ");
                scanf("%d %d", &n1, &n2);
                if (cost[n1][n2] == up) {
                    printf("路径不存在\n");
                } else {
                    printf("最短路径为：%d，具体如下： %d ", cost[n1][n2], n1);
                    getPath(G, n1, n2);
                }

                printf("\n");
            }
                system("pause");
                break;
            case 4: {
                /*====================4 单目标最短路径的Dijkstra算法=================*/
                int a2;
                cout << "输入单目标点:";
                cin >> a2;
                danmubiao(G, a2, G.n);
            }
                system("pause");
                break;
            case 5: {
                /*=======================5 Floyd-Warshall实现单顶点对间最短路径=======================*/
                int a, b;
                printf("请输入两个顶点：\n");
                scanf("%d %d", &a, &b);
                floyed(G, G.n, a, b);
            }
                system("pause");
                break;
            case 6: {
                /*=======================6 Floyd-Warshall实现有向图的可达矩阵=========================*/
                TransitiveClosure(G, t);
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
