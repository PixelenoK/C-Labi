#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

typedef unsigned long ul;
typedef unsigned char uc;

typedef struct list
{
    struct list* pNext;
    struct list* pPrev;
    uc staff;
} list;

void FreeList(list*, list*);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
5.1 (19). Разработать функцию, переводящую многоразрядное десятичное
число в двоичную систему. Для хранения многоразрядного числа
использовать динамический двунаправленный список.
*/
int main()
{
    FILE* data;
    FILE* rez;
    data = fopen("D:\\BSUIR\\Tapping\\C\\LR_5\\num.txt", "r");  //number to convert
    rez = fopen("D:\\BSUIR\\Tapping\\C\\LR_5\\rez.bin", "w");   //result

    if (!data)
    {
        return 1;
    }
    if (!rez)
    {
        fclose(data);
        return 1;
    }

    fseek(data, 0, SEEK_END);
    ul len = ftell(data);
    char* str = (char*)malloc(sizeof(char) * (len + 1));

    fseek(data, 0, SEEK_SET);
    fgets(str, len + 1, data);

    fclose(data);

    list* num = (list*)malloc(sizeof(list));
    if ((str[0] < '0') || (str[0] > '9'))
    {
        num->staff = 0;
    }
    else
    {
        num->staff = str[0] - '0';
    }

    num->pPrev = (list*)malloc(sizeof(list));
    num->pPrev->pNext = num;

    list* tale = num;
    {
        unsigned int numOfNaN = 0;
        for (ul i = 1; i < len; i++, tale = tale->pNext)
        {
            tale->pNext = (list*)malloc(sizeof(list));
            tale->pNext->pPrev = tale;
            if ((str[i] < '0') || (str[i] > '9'))
            {
                numOfNaN++;
                tale = tale->pPrev;
                continue;
            }
            tale->pNext->staff = str[i] - '0';
        }
        len -= numOfNaN;
    }
    free(num->pPrev);
    num->pPrev = NULL;
    tale->pNext = NULL;

    free(str);

    list* strS = (list*)malloc(sizeof(list));
    strS->pPrev = NULL;
    list* taleStr = strS;
    {
        for (list* zero = num; zero;)
        {
            taleStr->staff = tale->staff % 2 + '0';
            taleStr->pNext = (list*)malloc(sizeof(list));
            taleStr->pNext->pPrev = taleStr;
            taleStr = taleStr->pNext;

            for (list* pN = zero; pN; pN = pN->pNext)
            {
                if ((pN->staff % 2 == 1) && (pN->pNext))
                {
                    pN->pNext->staff += 10;
                }

                pN->staff /= 2;
            }

            if (!zero->staff)
            {
                zero = zero->pNext;
            }
        }

        taleStr = taleStr->pPrev;
        free(taleStr->pNext);
    }

    for (list* p = taleStr; p; p = p->pPrev)
    {
        fprintf(rez, "%c", p->staff);
    }

    FreeList(num, tale);
    FreeList(strS, taleStr);
    fclose(rez);

    printf("end...");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void FreeList(list* listS, list* tale)
{
    for (list* p = tale; p->pPrev;)
    {
        p = p->pPrev;
        free(p->pNext);
    }
    free(listS);
}
