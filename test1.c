#include <stdio.h>
#include <ctype.h>
#include <string.h>

//ȫ�ֱ���
FILE *fp;                                                                                   //�����ļ��ṹ������ָ��
char cbuffer;                                                                               //�ַ�����
char *key[9] = {"main", "if", "else", "for", "while", "do", "return", "break", "continue"}; //�ؼ���
int atype, id = 4;                                                                          //�ȼ���4

int search(char searchchar[], int wordtype) /*������������֣��������ǵ���.�жϵ����Ǳ����ֻ��Ǳ�ʶ��*/
{
    int i = 0;
    int p;
    switch (wordtype)
    {
    case 1:
        for (i = 0; i <= 7; i++)
        {
            // strcmp(str1,str2),�� str1 ��ָ����ַ����� str2 ��ָ����ַ������бȽϡ�����ֵС��0��str1<str2;����ֵ����0,str1>str2;����ֵ����0,str1=str2
            if (strcmp(key[i], searchchar) == 0)
            {
                p = i + 1; /*�Ǳ�������pΪ��0�Ҳ��ظ�������*/
                break;
            }
            else
                p = 0; /*���Ǳ����������ڷ��ص�p=0*/
        }
        return (p);
    }
}

//��ĸ����
char alphaprocess(char buffer)
{
    int atype; /*�����������е�λ��*/
    int i = -1;
    char alphatp[20];
    while ((isalpha(buffer)) || (isdigit(buffer)) || buffer == '_') // isalpha��isdigit����<ctype.h>�Ŀ⺯�����ж������ַ�Ϊ��ĸ������
    {
        alphatp[++i] = buffer;
        buffer = fgetc(fp);
    }                           /*��һ�������ĵ��ʷ���alphatp������*/
    alphatp[i + 1] = '\0';      //'\0' ���ַ����Ľ�����������һ�������ĵ��ʣ��Դ˽�β
    atype = search(alphatp, 1); /*�Դ˵��ʵ���search�����ж�����*/
    if (atype != 0)             // search�������ص�p��ֵ
    {
        printf("%s, (1,%d)\n", alphatp, atype - 1); //��������������ݣ��ȼ�Ϊ1
        id = 1;
    }
    else
    {
        printf("(%s ,2)\n", alphatp); //�Ǳ�������������ݣ��ȼ�Ϊ2
        id = 2;
    }
    return buffer;
}

//����
char digitprocess(char buffer)
{
    int i = -1;
    char digittp[20];
    while ((isdigit(buffer)))
    {
        digittp[++i] = buffer; //��������ַ�������������
        buffer = fgetc(fp);    //��ȡ��һ���ַ�
    }
    digittp[i + 1] = '\0';
    printf("(%s ,3)\n", digittp); //���ֵȼ�Ϊ3
    id = 3;
    return (buffer);
}

char otherprocess(char buffer)
{
    char ch[20];
    ch[0] = buffer;
    ch[1] = '\0';
    //�ָ���
    if (ch[0] == ',' || ch[0] == ';' || ch[0] == '{' || ch[0] == '}' || ch[0] == '(' || ch[0] == ')')
    {
        printf("(%s ,5)\n", ch);
        buffer = fgetc(fp);
        id = 4;
        return (buffer);
    }
    //�����*��/
    if (ch[0] == '*' || ch[0] == '/')
    {
        printf("(%s ,4)\n", ch);
        buffer = fgetc(fp);
        id = 4;
        return (buffer);
    }
    //�����=����,<,>
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
    //�����+��-
    if (ch[0] == '+' || ch[0] == '-')
    {
        if (id == 4) /*�ڵ�ǰ������ǰ������������ʱΪ������*/
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
    if ((fp = fopen("example1.c","r")) == NULL) /*ֻ����ʽ��һ���ļ�*/
        printf("error");                         /*���ļ�Ϊ�գ����error*/
    else
    {
        cbuffer = fgetc(fp); /*fgetc( )�������Ӵ����ļ���ȡһ���ַ�*/
        while (cbuffer != EOF)
        {
            if (cbuffer == ' ' || cbuffer == '\n') /*�ӹ��ո�ͻس���*/
                cbuffer = fgetc(fp);               /*��ȡ��һ���ַ�*/
            else                                   /*��ȡ���ַ�Ϊ��������*/
                if (isalpha(cbuffer))              /*��ȡ���ַ�Ϊ��ĸ�����Ĵ���*/
                    cbuffer = alphaprocess(cbuffer);
                else if (isdigit(cbuffer)) /*��ȡ���ַ�Ϊ���������Ĵ���*/
                    cbuffer = digitprocess(cbuffer);
                else
                    cbuffer = otherprocess(cbuffer);
        }
        getchar();
    }
}
