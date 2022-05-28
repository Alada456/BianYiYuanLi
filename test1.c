#include <stdio.h>
#include <ctype.h>
#include <string.h>

//全局变量
FILE *fp;                                                                                   //声明文件结构体类型指针
char cbuffer;                                                                               //字符变量
char *key[9] = {"main", "if", "else", "for", "while", "do", "return", "break", "continue"}; //关键字
int atype, id = 4;                                                                          //等级是4

int search(char searchchar[], int wordtype) /*传入数组和数字，数组中是单词.判断单词是保留字还是标识符*/
{
    int i = 0;
    int p;
    switch (wordtype)
    {
    case 1:
        for (i = 0; i <= 7; i++)
        {
            // strcmp(str1,str2),把 str1 所指向的字符串和 str2 所指向的字符串进行比较。返回值小于0，str1<str2;返回值大于0,str1>str2;返回值等于0,str1=str2
            if (strcmp(key[i], searchchar) == 0)
            {
                p = i + 1; /*是保留字则p为非0且不重复的整数*/
                break;
            }
            else
                p = 0; /*不是保留字则用于返回的p=0*/
        }
        return (p);
    }
}

//字母处理
char alphaprocess(char buffer)
{
    int atype; /*保留字数组中的位置*/
    int i = -1;
    char alphatp[20];
    while ((isalpha(buffer)) || (isdigit(buffer)) || buffer == '_') // isalpha和isdigit都是<ctype.h>的库函数，判断所传字符为字母或数字
    {
        alphatp[++i] = buffer;
        buffer = fgetc(fp);
    }                           /*读一个完整的单词放入alphatp数组中*/
    alphatp[i + 1] = '\0';      //'\0' 是字符串的结束符，读完一个完整的单词，以此结尾
    atype = search(alphatp, 1); /*对此单词调用search函数判断类型*/
    if (atype != 0)             // search函数返回的p的值
    {
        printf("%s, (1,%d)\n", alphatp, atype - 1); //保留字输出的内容，等级为1
        id = 1;
    }
    else
    {
        printf("(%s ,2)\n", alphatp); //非保留字输出的内容，等级为2
        id = 2;
    }
    return buffer;
}

//数字
char digitprocess(char buffer)
{
    int i = -1;
    char digittp[20];
    while ((isdigit(buffer)))
    {
        digittp[++i] = buffer; //将读入的字符放入数字数组
        buffer = fgetc(fp);    //读取下一个字符
    }
    digittp[i + 1] = '\0';
    printf("(%s ,3)\n", digittp); //数字等级为3
    id = 3;
    return (buffer);
}

char otherprocess(char buffer)
{
    char ch[20];
    ch[0] = buffer;
    ch[1] = '\0';
    //分隔符
    if (ch[0] == ',' || ch[0] == ';' || ch[0] == '{' || ch[0] == '}' || ch[0] == '(' || ch[0] == ')')
    {
        printf("(%s ,5)\n", ch);
        buffer = fgetc(fp);
        id = 4;
        return (buffer);
    }
    //运算符*和/
    if (ch[0] == '*' || ch[0] == '/')
    {
        printf("(%s ,4)\n", ch);
        buffer = fgetc(fp);
        id = 4;
        return (buffer);
    }
    //运算符=，！,<,>
    if (ch[0] == '=' || ch[0] == '!' || ch[0] == '<' || ch[0] == '>')
    {
        buffer = fgetc(fp);
        if (buffer == '=')
        {
            ch[1] = buffer;
            ch[2] = '\0';
            printf("(%s ,4)\n", ch);
        }
        else
        {
            printf("(%s ,4)\n", ch);
            id = 4;
            return (buffer);
        }
        buffer = fgetc(fp);
        id = 4;
        return (buffer);
    }
    //运算符+，-
    if (ch[0] == '+' || ch[0] == '-')
    {
        if (id == 4) /*在当前符号以前是运算符，则此时为正负号*/
        {
            buffer = fgetc(fp);
            ch[1] = buffer;
            ch[2] = '\0';
            printf("(%s ,3)\n", ch);
            id = 3;
            buffer = fgetc(fp);
            return (buffer);
        }
        ch[1] = '\0';
        printf("(%s ,4)\n", ch);
        buffer = fgetc(fp);
        id = 4;
        return (buffer);
    }
}

void main()
{
    if ((fp = fopen("example1.c","r")) == NULL) /*只读方式打开一个文件*/
        printf("error");                         /*若文件为空，输出error*/
    else
    {
        cbuffer = fgetc(fp); /*fgetc( )函数：从磁盘文件读取一个字符*/
        while (cbuffer != EOF)
        {
            if (cbuffer == ' ' || cbuffer == '\n') /*掠过空格和回车符*/
                cbuffer = fgetc(fp);               /*读取下一个字符*/
            else                                   /*读取的字符为其他可能*/
                if (isalpha(cbuffer))              /*读取的字符为字母所做的处理*/
                    cbuffer = alphaprocess(cbuffer);
                else if (isdigit(cbuffer)) /*读取的字符为数字所做的处理*/
                    cbuffer = digitprocess(cbuffer);
                else
                    cbuffer = otherprocess(cbuffer);
        }
        getchar();
    }
}
