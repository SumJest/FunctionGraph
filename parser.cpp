#include <string>
#include <cmath>
#include <ctype.h>
#include <sstream>


#include "stack.h"
#include "parser.h"

using namespace std;

double degrees_to_radian(double deg)
{
    return deg * 3.141592 / 180.0;
}

std::string to_string(const double &f)
{
    std::stringstream ss;
    ss << f;
    return ss.str();
}

int getfloatlength(float number)
{
    int i = 0;
    while(number>=1)
    {
        number=number/10;
        i++;
    }
    return i;
}

void replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos=0;

    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
             str.replace(start_pos, from.length(), to);
             start_pos += to.length();
    }
}

int getPriority(char op)
{
    switch(op)
    {
        case '(':return -1;
        case '.':return 0;
        case '^':return 1;
        case '*':return 2;
        case '/':return 2;
        case '+':return 3;
        case '-':return 3;
        default: return -2;
    }
}

void popFunction(stack<float> *f_stk,stack<char> *c_stk)
{
    float a = f_stk->f_pop();
    float b = f_stk->f_pop();
    float result;
    char op = c_stk->f_pop();
    switch(op)
    {
        case '^':
            float i;
            if(modf(a,&i)!=0&&b<0){throw exception();}
            result=pow(b,a);
            break;
        case '*':
            result=b*a;
            break;
        case '/':
            if(!a){throw exception();}
            result=b/a;
            break;
        case '+':
            result=b+a;
            break;
        case '-':
            result=b-a;
            break;
        case '.':
            result=a/(pow(10,getfloatlength(a)))+b;
            break;
        default:
            result=0;
            break;
    }
    f_stk->f_push(result);

}

bool canPop(stack<char> *c_stk,char op)
{
    if(!c_stk->getSize()) return false;
    int p1 = getPriority(op);
    int p2 = getPriority(c_stk->f_top());
    return (p1>=0)&&(p2>=0)&&(p1>=p2);
}
float parse(QString ss)
{
    stack<float> f_stk;
    stack<char> c_stk;

    string from = "sin(";
    string s = ss.toUtf8().constData();
    size_t start_pos=s.find(from,0);
    string s_sin = "";

    while(start_pos!=std::string::npos)
    {
        int j = 0;
        for(int i = start_pos+from.length();i<s.length();i++)
        {
            if(s[i]=='(') j++;
            else if(s[i]==')')
            {
                if(j==0)
                {
                    float f_sin =parse(QString::fromStdString("("+s_sin+")"));
                    QString q_replacement = QString::number(sin(f_sin));
                    string replacement = q_replacement.toUtf8().constData();

                    replace(s,from+s_sin+")","("+replacement+")");

                    break;
                }
                else j--;
            }
            s_sin+=s[i];
        }
        s_sin="";
        start_pos=s.find(from,0);
    }

    from = "cos(";
    start_pos=s.find(from,0);
    s_sin = "";

    while(start_pos!=std::string::npos)
    {
        int j = 0;
        for(int i = start_pos+from.length();i<s.length();i++)
        {
            if(s[i]=='(') j++;
            else if(s[i]==')')
            {
                if(j==0)
                {
                    float f_sin =parse(QString::fromStdString("("+s_sin+")"));
                    QString q_replacement = QString::number(cos(f_sin));
                    string replacement = q_replacement.toUtf8().constData();

                    replace(s,from+s_sin+")","("+replacement+")");

                    break;
                }
                else j--;
            }
            s_sin+=s[i];
        }
        s_sin="";
        start_pos=s.find(from,0);
    }

    from = "tg(";
    start_pos=s.find(from,0);
    s_sin = "";

    while(start_pos!=std::string::npos)
    {
        int j = 0;
        for(int i = start_pos+from.length();i<s.length();i++)
        {
            if(s[i]=='(') j++;
            else if(s[i]==')')
            {
                if(j==0)
                {
                    float f_sin =parse(QString::fromStdString("("+s_sin+")"));
                    QString q_replacement = QString::number(sin(f_sin)/cos(f_sin));
                    string replacement = q_replacement.toUtf8().constData();

                    replace(s,from+s_sin+")","("+replacement+")");

                    break;
                }
                else j--;
            }
            s_sin+=s[i];
        }
        s_sin="";
        start_pos=s.find(from,0);
    }
    from = "|";
    start_pos=s.find(from,0);
    s_sin = "";
    while(start_pos!=std::string::npos)
    {
        for(int i = start_pos+from.length();i<s.length();i++)
        {
            if(s[i]=='|')
            {
                float f_sin = abs(parse(QString::fromStdString("("+s_sin+")")));
                string replacement = to_string(f_sin);
                replace(s,from+s_sin+from,"("+replacement+")");

                break;
            }
            s_sin+=s[i];
        }
        s_sin="";
        start_pos=s.find(from,0);
    }
    char token,prevToken;
    c_stk.f_push('(');
    prevToken = '(';
    for(int i = 1; i < s.length();i++)
    {
        token=s[i];

        if(prevToken=='('&&(token=='+'||token=='-')) f_stk.f_push(0);
        if(isdigit(token))
        {
            float ftoken = token-'0';
            if(isdigit(prevToken))f_stk.f_push(f_stk.f_pop()*10+ftoken);
            else f_stk.f_push(ftoken);
        }
        else
        {
            if(token==')')
            {
                while(c_stk.getSize()>0&&c_stk.f_top()!='(') popFunction(&f_stk,&c_stk);
                c_stk.f_pop();
            }else
            {
                while(canPop(&c_stk,token)){popFunction(&f_stk,&c_stk);}
                c_stk.f_push(token);
            }
        }
        prevToken=token;
    }
    return f_stk.f_pop();
}

