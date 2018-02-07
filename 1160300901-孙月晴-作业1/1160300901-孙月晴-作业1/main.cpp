#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;
#define maxlength 100


struct word    //定义word结构体类型，其中包含两个变量
{
    char type;
    double num;
};

struct STACK {   //定义栈的结构体类型
    int top;
    word elements[maxlength];
};

void MakeNull(STACK &S)//初始化栈
{
    S.top = -1;
}

bool Empty(STACK S)//判栈空
{
    if (S.top < 0)
        return 1;
    else
        return 0;
}

word Top(STACK S)//弹出栈顶元素
{
    word empty = {'0', 0};
    if (Empty(S))
        return empty;
    else
        return (S.elements[S.top]);
}

word Pop(STACK &S)//删除栈顶元素
{
    if (Empty(S)) {
        std::cerr << "栈空不能出栈 ";
    }
    return S.elements[S.top--];
}

void Push(word x, STACK &S)//将元素压入栈中
{
    if (S.top == maxlength - 1) {
        std::cerr << "栈满不能入栈 ";
    } else {
        S.top = S.top + 1;
        S.elements[S.top] = x;
    }
}

word Display(STACK S) {
    word empty = {'0', 0};
    cout << "栈中元素有 ";
    while (!Empty(S)) {
        empty = Pop(S);
        cout << empty.type << " ";
    }
    cout << endl;
}

word iDisplay(STACK S) {
    word empty = {'0', 0};
    cout << "栈中元素有 ";
    while (!Empty(S)) {
        empty = Pop(S);
        cout << empty.num << " ";
    }
    cout << endl;
}

void analysis(char infix[], word infix2[], int m) {           //将char数组的中缀表达式转换为word类型的中缀表达式
    int i = 0, j = 0, flag = 0, Flag = 0;  //i 记录有多少个表达式中有多少个字符，j 记录word型数组有多少个元素
    double sum;
    while (infix[i] != '#') {
        sum = 0;
        if (infix[i] >= '0' && infix[i] <= '9') {
            while (infix[i] >= '0' && infix[i] <= '9') {
                sum *= 10;
                sum += (double) (infix[i] - '0');//按多位数存储
                i++;
            }
            if (infix[i] == '.') {
                i++;
                float k = 10.0;
                while (infix[i] >= '0' && infix[i] <= '9')//按小数存储
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


void Transform(word infix2[], word suffix[], int m, int &len) {      //将中缀表达式转换为后缀表达式
    STACK S;     //定义空栈O
    MakeNull(S);
    int i;
    for (i = 0; i < m; i++) {
        if (infix2[i].type == '0')//如果为数字时
        {
            suffix[len] = infix2[i];//直接存入后缀表达式数组
            len++;                  //后缀表达式数组的元素个数
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
            if(i==0){   //当表达式的第一项为负数，例如-9*6+4
                infix2[1].num=-infix2[1].num;
            } else if(infix2[i-1].type=='('){  //当表达式带有"(-",例如9+(-3*4)
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
    while (!Empty(S))//当栈非空时，弹出栈顶元素存入后缀表达式数组
    {
        suffix[len++] = Pop(S);
        Display(S);
    }
    int j;
    cout << "转换后的后缀表达式为：" << endl;
    for (j = 0; j < len; j++)//输出后缀表达式数组
    {
        if (suffix[j].type == '0') {
            cout << suffix[j].num << " ";
        } else
            cout << suffix[j].type << " ";
    }
    cout << endl;
}

double Sum(word suffix[], int len) {   //求和
    STACK S;
    MakeNull(S);
    for (int i = 0; i < len; i++) {
        if (suffix[i].type == '0')//为数字时
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
    cout << "运算结果为：" << endl;
    double result = Pop(S).num;
    return result;
}

int main() {
    double r;
    char infix1[100]; //文件输入的char数组类型的中缀表达式
    word infix2[100];  //转换为word类型的中缀表达式
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

    cout << "输入的中缀表达式为：" << endl;
    for (int i = 0; i < m; i++) {
        cout << infix1[i];
    }
    cout << endl;
    cout << "\n";
    analysis(infix1, infix2, m);        //将char数组的中缀表达式转换为word类型的中缀表达式
    word suffix[100];                   //后缀表达式的数组
    int len = 0;                         //后缀表达式数组的元素个数
    cout << "栈的状态变化过程：" << endl;
    Transform(infix2, suffix, m, len);//中缀表达式转后缀表达式
    cout << "\n";
    cout << "运算过程：" << endl;
    r = Sum(suffix, len);
    cout << r << endl;
}
