#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;
#define maxlength 100


struct word    //����word�ṹ�����ͣ����а�����������
{
    char type;
    double num;
};

struct STACK {   //����ջ�Ľṹ������
    int top;
    word elements[maxlength];
};

void MakeNull(STACK &S)//��ʼ��ջ
{
    S.top = -1;
}

bool Empty(STACK S)//��ջ��
{
    if (S.top < 0)
        return 1;
    else
        return 0;
}

word Top(STACK S)//����ջ��Ԫ��
{
    word empty = {'0', 0};
    if (Empty(S))
        return empty;
    else
        return (S.elements[S.top]);
}

word Pop(STACK &S)//ɾ��ջ��Ԫ��
{
    if (Empty(S)) {
        std::cerr << "ջ�ղ��ܳ�ջ ";
    }
    return S.elements[S.top--];
}

void Push(word x, STACK &S)//��Ԫ��ѹ��ջ��
{
    if (S.top == maxlength - 1) {
        std::cerr << "ջ��������ջ ";
    } else {
        S.top = S.top + 1;
        S.elements[S.top] = x;
    }
}

word Display(STACK S) {
    word empty = {'0', 0};
    cout << "ջ��Ԫ���� ";
    while (!Empty(S)) {
        empty = Pop(S);
        cout << empty.type << " ";
    }
    cout << endl;
}

word iDisplay(STACK S) {
    word empty = {'0', 0};
    cout << "ջ��Ԫ���� ";
    while (!Empty(S)) {
        empty = Pop(S);
        cout << empty.num << " ";
    }
    cout << endl;
}

void analysis(char infix[], word infix2[], int m) {           //��char�������׺���ʽת��Ϊword���͵���׺���ʽ
    int i = 0, j = 0, flag = 0, Flag = 0;  //i ��¼�ж��ٸ����ʽ���ж��ٸ��ַ���j ��¼word�������ж��ٸ�Ԫ��
    double sum;
    while (infix[i] != '#') {
        sum = 0;
        if (infix[i] >= '0' && infix[i] <= '9') {
            while (infix[i] >= '0' && infix[i] <= '9') {
                sum *= 10;
                sum += (double) (infix[i] - '0');//����λ���洢
                i++;
            }
            if (infix[i] == '.') {
                i++;
                float k = 10.0;
                while (infix[i] >= '0' && infix[i] <= '9')//��С���洢
                {
                    sum += (double) (infix[i] - '0') / k;
                    i++;
                    k *= 10.0;
                }
            }
            infix2[j].type = '0';
            infix2[j].num = sum;
            j++;
        } else {
                infix2[j].type = infix[i];
                i++;
                j++;
            }
        }
    }


void Transform(word infix2[], word suffix[], int m, int &len) {      //����׺���ʽת��Ϊ��׺���ʽ
    STACK S;     //�����ջO
    MakeNull(S);
    int i;
    for (i = 0; i < m; i++) {
        if (infix2[i].type == '0')//���Ϊ����ʱ
        {
            suffix[len] = infix2[i];//ֱ�Ӵ����׺���ʽ����
            len++;                  //��׺���ʽ�����Ԫ�ظ���
        }
        if (infix2[i].type == '(') {
            Push(infix2[i], S);
            Display(S);
        }
        if (infix2[i].type == '+' ) {
            while (!Empty(S) && Top(S).type != '(') {
                suffix[len++] = Pop(S);
                Display(S);
            }
            Push(infix2[i], S);
            Display(S);
        }
        if ( infix2[i].type == '-') {
            if(i==0){   //�����ʽ�ĵ�һ��Ϊ����������-9*6+4
                infix2[1].num=-infix2[1].num;
            } else if(infix2[i-1].type=='('){  //�����ʽ����"(-",����9+(-3*4)
                infix2[i+1].num=-infix2[i+1].num;
            } else{
                while (!Empty(S) && Top(S).type != '(') {
                    suffix[len++] = Pop(S);
                    Display(S);
                }
                Push(infix2[i], S);
                Display(S);
            }
        }

        if (infix2[i].type == '/' || infix2[i].type == '*' || infix2[i].type == '%') {
            while (!Empty(S) && Top(S).type != '-' && Top(S).type != '+' && Top(S).type != '(')
                suffix[len++] = Pop(S);
            Display(S);
            Push(infix2[i], S);
            Display(S);
        }
        if (infix2[i].type == '^') {
            while (!Empty(S) && Top(S).type != '-' && Top(S).type != '+' && Top(S).type != '(' && Top(S).type != '/' &&
                   infix2[i].type != '*' && infix2[i].type != '%')
                suffix[len++] = Pop(S);
            Display(S);
            Push(infix2[i], S);
            Display(S);
        }
        if (infix2[i].type == ')') {
            while (Top(S).type != '(') {
                suffix[len++] = Pop(S);
                Display(S);
            }
            Pop(S);
            Display(S);
        }
    }
    while (!Empty(S))//��ջ�ǿ�ʱ������ջ��Ԫ�ش����׺���ʽ����
    {
        suffix[len++] = Pop(S);
        Display(S);
    }
    int j;
    cout << "ת����ĺ�׺���ʽΪ��" << endl;
    for (j = 0; j < len; j++)//�����׺���ʽ����
    {
        if (suffix[j].type == '0') {
            cout << suffix[j].num << " ";
        } else
            cout << suffix[j].type << " ";
    }
    cout << endl;
}

double Sum(word suffix[], int len) {   //���
    STACK S;
    MakeNull(S);
    for (int i = 0; i < len; i++) {
        if (suffix[i].type == '0')//Ϊ����ʱ
        {
            Push(suffix[i], S);
            iDisplay(S);
        } else {
            double a, b;
            a = Pop(S).num;
            iDisplay(S);
            b = Pop(S).num;
            iDisplay(S);
            word c;
            c.type = '0';
            switch (suffix[i].type) {
                case '+':
                    c.num = b + a;
                    break;
                case '-':
                    c.num = b - a;
                    break;
                case '*':
                    c.num = b * a;
                    break;
                case '/':
                    c.num = b / a;
                    break;
                case '%':
                    c.num = fmod(b, a);
                    break;
                case '^':
                    c.num = pow(b, a);
                    break;
                default:
                    break;
            }
            Push(c, S);
            iDisplay(S);
        }
    }
    cout << "������Ϊ��" << endl;
    double result = Pop(S).num;
    return result;
}

int main() {
    double r;
    char infix1[100]; //�ļ������char�������͵���׺���ʽ
    word infix2[100];  //ת��Ϊword���͵���׺���ʽ
    int m = 0;

    ifstream infile("test.txt", ios::in);
    if (!infile) {
        cerr << "open error!" << endl;
        exit(1);
    }
    while (!infile.eof()) {
        infile >> infix1[m];
        if (infix1[m] == '#')
            break;
        m++;
    }
    infile.close();

    cout << "�������׺���ʽΪ��" << endl;
    for (int i = 0; i < m; i++) {
        cout << infix1[i];
    }
    cout << endl;
    cout << "\n";
    analysis(infix1, infix2, m);        //��char�������׺���ʽת��Ϊword���͵���׺���ʽ
    word suffix[100];                   //��׺���ʽ������
    int len = 0;                         //��׺���ʽ�����Ԫ�ظ���
    cout << "ջ��״̬�仯���̣�" << endl;
    Transform(infix2, suffix, m, len);//��׺���ʽת��׺���ʽ
    cout << "\n";
    cout << "������̣�" << endl;
    r = Sum(suffix, len);
    cout << r << endl;
}
