#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

long double_length_nodec(double f);
int rfind(char exp[], int len, char c);
int find(char exp[], int len, char c);
long max(long a, long b);

double eval(char exp[], int size)
{
    // take care of something like 4+-3 -- actually DONT NEED
    /*for (int i = 0; i < size-1; i++)
    {
        if(exp[i] == '+' && exp[i+1] == '-')
        {
            memcpy(exp, exp, i);
            exp[i] = '-';
            memcpy(&exp[i+1], &exp[i+2], size-(i+2));
            size--;
        }
    }*/
    
    //deal with parenthesis
    //locate paren and corresponding close paren, keeping track of how many parens were found
    int start = -1;
    int end = -1;
    int numparen = 0;
    for(int i = 0; i < size; i++)
    {
        if(exp[i] == '(')
        {
            if (numparen == 0) start = i;
            numparen++;
        }
        if(exp[i] == ')')
        {
            if(numparen == 1)
            {
                end = i;
                break;
            }
            numparen--;
        }
    }
    if(start != -1)
    {
        //get the expression inside the parenthesis
        char inside[end - start];
        memcpy(inside, &exp[start+1], sizeof(inside)/sizeof(char)-1);
        inside[sizeof(inside)/sizeof(char)-1] = '\0';
        //evaluate the extracted expression
        double val = eval(inside, sizeof(inside));
        
        //if expression is negative, allocate space for the negative sign too
        int negative = 0;
        if (val < 0)
        {
            negative = 1;
        }
        
        //create new expression allocation. the size variable already includes the necessary space for the \0 char
        //size-(end-start+1) takes care of everything that is not the extracted/evaluated piece
        //8 refers to .0000000
        char newexp[size - (end-start+1) + double_length_nodec(val) + 8 + negative];
        
        //extract first part of expression
        char exp1[start+1];
        memcpy(exp1, exp, start);
        exp1[sizeof(exp1)/sizeof(char)-1] = '\0';//dont forget to close string
        
        //extract second part of expression
        char exp3[size - end - 1];
        memcpy(exp3, &exp[end+1], sizeof(exp3)/sizeof(char)-1);
        exp3[sizeof(exp3)/sizeof(char)-1] = '\0';//close string
        
        sprintf(newexp, "%s%.7f%s",exp1 , val, exp3);//NO NEED TO CLOSE STRING HERE because sprintf auto format
        //but make sure the array passed into the function has enough space to accomodate \0 char
        
        return eval(newexp, sizeof(newexp));//recursion
    }
    
    int divpnt = rfind(exp, size, '+');
    int divpnt2= rfind(exp, size, '-');
    
    if(divpnt != -1 || divpnt2 != -1)
    {
        divpnt = max(divpnt, divpnt2);
        
        char pt1[divpnt + 1];
        char pt2[size - divpnt - 1];
        
        memcpy(pt1, exp, divpnt);
        pt1[sizeof(pt1)-1] = '\0';
        
        memcpy(pt2, &exp[divpnt+1], sizeof(pt2)-1);
        pt2[sizeof(pt2)-1] = '\0';
                
        if(exp[divpnt] == '+')
        {
            return eval(pt1, sizeof(pt1)) + eval(pt2, sizeof(pt2));
        }
        else
        {
            return eval(pt1, sizeof(pt1)) - eval(pt2, sizeof(pt2));
        }
    }
    
    divpnt = rfind(exp, size, '*');
    divpnt2= rfind(exp, size, '/');
    if( divpnt != -1 || divpnt2 != -1 )
    {
        divpnt = max(divpnt, divpnt2);
        
        char pt1[divpnt + 1];
        char pt2[size - divpnt - 1];
        
        memcpy(pt1, exp, divpnt);
        pt1[sizeof(pt1)-1] = '\0';
        
        memcpy(pt2, &exp[divpnt+1], sizeof(pt2)-1);
        pt2[sizeof(pt2)-1] = '\0';
        
        if(exp[divpnt] == '*')
        {
            return eval(pt1, sizeof(pt1)) * eval(pt2, sizeof(pt2));
        }
        else
        {
            return eval(pt1, sizeof(pt1)) / eval(pt2, sizeof(pt2));
        }
    }
    
    divpnt = find(exp, size, '^');
    if(divpnt != -1)
    {
        char pt1[divpnt + 1];
        char pt2[size - divpnt - 1];
        
        memcpy(pt1, exp, divpnt);
        pt1[sizeof(pt1)-1] = '\0';
        
        memcpy(pt2, &exp[divpnt+1], sizeof(pt2)-1);
        pt2[sizeof(pt2)-1] = '\0';
                        
        return pow(eval(pt1, sizeof(pt1)),eval(pt2, sizeof(pt2)));
    }
    
    return atof(exp);
}

long max(long a, long b)
{
    if(a >= b)
    {
        return a;
    }
    if(b > a)
    {
        return b;
    }
}

int rfind(char exp[], int len, char c)
{
    for (int i = len-1; i >= 0; i--)
    {
        if(exp[i] == c)
        {
            return i;
        }
    }
    
    return -1;
}

int find(char exp[], int len, char c)
{
    for (int i = 0; i < len; i++)
    {
        if(exp[i] == c)
        {
            return i;
        }
    }
    
    return -1;
}

long double_length_nodec(double f)
{
    //take absolute value because finding number of digits is like finding magnitude in 10th power sense
    //thinking: devide number until it approach 0, either from positive or negative dir
    f = fabs(f);
    long num = (long)f;
    long i = 0;
    while(num > 0)
    {
        num = num / 10;
        i++;
    }
    return max(i,1);
}

int main()
{
    char exp[] = "3-5-8+3+(3-6)+((2^4)^2+3/2-3*5+((3)))+3*((3+2)+(3+4+(3/4^4)^2/4+(3)))/2+(3-3)*2-3+(3/4*((3)/4))+((3))/2+(3-334444)*2*3+-5+2+(3-3)*2+2+(3+4)+3+(2+3.234234125412234234234234234)*3+2/4/(4/4)+3";
    printf("%f\n",eval(exp, sizeof(exp)));
    //eval(exp, sizeof(exp));
}
