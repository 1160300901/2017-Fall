#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <iomanip>

using namespace std;
#define Max 15
#define up 999  //·��������
int cost[Max][Max];//Floyd �㷨�����·������
int P[Max][Max];//Floyd �㷨�����·��

/*=======================�ڽӾ���洢============================*/
typedef struct {
    int vertlist[Max];
    int edge[Max][Max];
    int n, e;
} MTGraph;

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

/*=========================1 ��ӡ�ڽӾ���==========================*/
void OutPutAdjMatrix(MTGraph gm) {
    cout << "ͼ�ĸ��������룺" << endl;
    for (int i = 0; i < gm.n; i++) {
        for (int j = 0; j < gm.n; j++) {
            if (gm.edge[i][j] == up)
                printf("�� ");
            else
                printf("%d ", gm.edge[i][j]);
        }
        cout << endl;
    }
}

/*=========================2 Dijkstraʵ�ֵ�Դ���·��==========================*/
void DijkstraPath(MTGraph g, int *dist, int *path, int v0) {
    int i, j, k;
    bool *visited = (bool *) malloc(sizeof(bool) * g.n);
    for (i = 0; i < g.n; i++)     //��ʼ��
    {
        if (g.edge[v0][i] > 0 && i != v0) {
            dist[i] = g.edge[v0][i];
            path[i] = v0;     //path��¼���·���ϴ�v0��i��ǰһ������
        } else {
            dist[i] = up;    //��i����v0ֱ�����ڣ���Ȩֵ��Ϊ�����
            path[i] = -1;
        }
        visited[i] = false;
        path[v0] = v0;
        dist[v0] = 0;
    }
    visited[v0] = true;
    for (i = 1; i < g.n; i++)     //ѭ����չn-1��
    {
        int min = up;
        int u;
        for (j = 0; j < g.n; j++)    //Ѱ��δ����չ��Ȩֵ��С�Ķ���
        {
            if (visited[j] == false && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        visited[u] = true;
        for (k = 0; k < g.n; k++)   //����dist�����ֵ��·����ֵ
        {
            if (visited[k] == false && g.edge[u][k] > 0 && min + g.edge[u][k] < dist[k]) {
                dist[k] = min + g.edge[u][k];
                path[k] = u;
            }
        }
    }
}

void showPath(int *path, int v, int v0) { //��ӡ���·���ϵĸ�������
    stack<int> s;
    int u = v;
    while (v != v0) {
        s.push(v);
        v = path[v];
    }
    s.push(v);
    printf("���·��Ϊ�� ");
    while (!s.empty()) {

        cout << s.top() << " ";
        s.pop();
    }
}

/*========================3 ȫ�����·����Floyd-Warshall �㷨=====================*/
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
    printf("��̾������\n");
    for (i = 0; i < g.n; i++) {
        for (j = 0; j < g.n; j++) {
            if (i == j) {
                printf("0 ");
            } else if (cost[i][j] == 999) {
                printf("�� ");
            } else
                printf("%d ", cost[i][j]);
        }
        printf("\n");
    }
    cout << setw(2) << "\n���������������̾����·��:\n";
    for (i = 0; i < g.n; i++)/*������·��*/
        for (j = 0; j < g.n; j++)
            if (i != j) {
                cout << "" << i << "->" << j << ":";
                if (cost[i][j] == up) {
                    if (i != j)
                        cout << setw(2) << "������·��" << endl;
                } else {
                    cout << setw(2) << "·������Ϊ��" <<cost[i][j] << " ";
                    cout << setw(2) << " ·��Ϊ" << i << " ";
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

/*========================4 ��Ŀ�����·����Dijkstra�㷨===========================*/
void danmubiao(MTGraph G, int v, int n) {
    int ds[Max], p[Max];
    int s[Max];
    int min, i, j, u, w;
    for (i = 0; i < n; i++) {
        ds[i] = G.edge[v][i];  //�����ʼ��
        s[i] = 0;          //S[ ]�ÿ�
        if (G.edge[v][i] < up)//·����ʼ��
            p[i] = v;
        else
            p[i] = -1;
    }
    s[v] = 1;
    p[v] = 0; //Դ���� V����S��
    for (i = 0; i < n; i++) //ѭ��ֱ�����ж�������·�������
    {
        min = up;
        u = -1;
        for (j = 0; j < n; j++)
            if (s[j] == 0 && ds[j] < min) //ѡȡ����S���Ҿ�����С����Ķ���U
            {
                u = j;
                min = ds[j];
            }
        if (u != -1) //�ҵ���С����Ķ���U
        {
            s[u] = 1; //������U����S��
            for (j = 0; j < n; j++)//�޸Ĳ���S�еĶ���ľ���
                if (s[j] == 0)
                    if (G.edge[u][j] < up && ds[u] + G.edge[u][j] < ds[j]) {
                        ds[j] = ds[u] + G.edge[u][j];
                        p[j] = u;
                    }
        }
    }
    cout << setw(2) << "\n��Ŀ�����·��:\n";
    for (i = 0; i < n; i++)//������·���ĳ���,·���������
    {
        if (i != v) {
            cout << "" << i << "->" << v << ":";
            if (s[i] == 1) {
                cout << setw(2) << "·������Ϊ��" << setw(2) << ds[i] << " ,";
                w = i;
                cout << setw(2) << "·��˳��Ϊ��";
                while (w != v) //һֱ���ݵ���ʼ����
                {
                    cout << w << ",";
                    w = p[w];
                }
                cout << w << endl;
            } else
                cout << "������·��" << endl;
        }
    }
}

/*=========================5 ������Լ����·��==========================*/
void floyed(MTGraph MTG, int n, int a, int b) {
    int G[Max][Max], p[Max][Max];
    int i, j, k, w;
    for (i = 0; i < n; i++)//�ó�ֵ
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
                cout << setw(2) << "������·��" << endl;
        } else {
            cout << setw(2) << "·������Ϊ��" << G[a][b] << " ,";
            cout << setw(2) << "·��Ϊ��" << a << " ";
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
                cout << setw(2) << "������·��" << endl;
        } else {
            cout << setw(2) << "·������Ϊ��" << G[b][a] << " ,";
            cout << setw(2) << "·��Ϊ��" << b << " ";
            w = p[b][a];
            while (w != -1) {
                cout << w << " ";
                w = p[w][a];
            }
            cout << a << endl;
        }
    }
}

/*========================6 ����ͼ�Ŀɴ����===========================*/
void TransitiveClosure(MTGraph G, int t[Max][Max]) {//Ѱ�ҿɴ����
    for (int i = 0; i < G.n; i++)//���б���
        for (int j = 0; j < G.n; j++) {
            if ((i == j) || (G.edge[i][j]) != up)//���ֿɴ�
                t[i][j] = 1;
            else
                t[i][j] = 0;
        }
    for (int k = 0; k < G.n; k++)
        for (int i = 0; i < G.n; i++)
            for (int j = 0; j < G.n; j++)
                t[i][j] = t[i][j] || (t[i][k] && t[k][j]);
    printf("�ɴ����Ϊ��\n");
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
        printf("************ �� �� · �� �� �� �� ʵ �� ************\n");
        printf("\t1.��ʾ�洢��ͼ�ĸ���������\n");
        printf("\t2.Dijkstraʵ�ֵ�Դ���·��\n");
        printf("\t3.Floyd-Warshallʵ��ȫ�����·��\n");
        printf("\t4.Dijkstraʵ�ֵ�Ŀ�����·��\n");
        printf("\t5.Floyd-Warshallʵ�ֵ�����Լ����·��\n");
        printf("\t6.Floyd-Warshallʵ������ͼ�Ŀɴ����\n");
        printf("\t0.�˳�\n");
        printf("\n������0-5,ѡ��Ҫִ�еĲ���:");
        scanf("%d", &choice);
        system("CLS");
        switch (choice) {
            case 1: {
                /*=====================1 ��ʾ�洢��ͼ�ĸ���������===================*/
                OutPutAdjMatrix(G);
            }
                system("pause");
                break;
            case 2: {
                /*====================2 Dijkstraʵ�ֵ�Դ���·��======================*/
                int *dist = (int *) malloc(sizeof(int) * G.n);
                int *path = (int *) malloc(sizeof(int) * G.n);
                printf("����0(Ĭ��)������������·������̾��룺\n");
                DijkstraPath(G, dist, path, 0);
                for (int i = 0; i < G.n; i++) {
                    if (i != 0) {
                        showPath(path, i, 0);
                        printf(" ��̾���Ϊ�� ");
                        if (dist[i] == up)
                            cout << "��" << endl;
                        else
                            cout << dist[i] << endl;
                    }
                }
            }
                system("pause");
                break;
            case 3: {
                /*=================3 ȫ�����·����Floyd-Warshall �㷨===============*/
                shortestpath(G);
                int n1, n2;
                printf("���������������������֮������·��: ");
                scanf("%d %d", &n1, &n2);
                if (cost[n1][n2] == up) {
                    printf("·��������\n");
                } else {
                    printf("���·��Ϊ��%d���������£� %d ", cost[n1][n2], n1);
                    getPath(G, n1, n2);
                }

                printf("\n");
            }
                system("pause");
                break;
            case 4: {
                /*====================4 ��Ŀ�����·����Dijkstra�㷨=================*/
                int a2;
                cout << "���뵥Ŀ���:";
                cin >> a2;
                danmubiao(G, a2, G.n);
            }
                system("pause");
                break;
            case 5: {
                /*=======================5 Floyd-Warshallʵ�ֵ�����Լ����·��=======================*/
                int a, b;
                printf("�������������㣺\n");
                scanf("%d %d", &a, &b);
                floyed(G, G.n, a, b);
            }
                system("pause");
                break;
            case 6: {
                /*=======================6 Floyd-Warshallʵ������ͼ�Ŀɴ����=========================*/
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
