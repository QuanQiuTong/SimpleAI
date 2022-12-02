/**
 * @file library.h
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief Universal and useful functions in the project.
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

typedef enum filemode
{
    read_strategy = 0,
    write_strategy = 1,
    write_log = 2,
} filemode;

void __cdecl set_random(unsigned long long);
unsigned __cdecl random(void);
size_t probability(size_t);
FILE *openfile(filemode);
unsigned long long read(void);

unsigned long long holdrand = 1ull;
void __cdecl set_random(unsigned long long _Seed)
{
    holdrand = _Seed;
}
/**
 * @brief Generate an unsigned random integar.
 *
 * @return ** unsigned
 *
 * 尽量做得原汁原味，所以不写成内联函数，甚至用__cdecl修饰。
 */
unsigned __cdecl random(void)
{
    return (((holdrand = holdrand * 214013ull + 2531011ull) >> 32) & 0x7fffffffl);
}
/**
 * @brief 按照线性递增的概率取得[0,x)的自然数。
 *
 * @param x 要取得的自然数的最大值（不包含）。
 * @return 本次取到的自然数。
 */
inline size_t probability(size_t x)
{
    return sqrt(random() % (x * x));
}
/**
 * @brief Open the file needed, and make sure the file pointer is valid.
 *
 * @param mode  Read the given strategy, or output the generated strategy.
 * @return FILE*  A file-pointer.
 */
inline FILE *openfile(filemode mode)
{
    FILE *fp;
    switch (mode)
    {
    case read_strategy:
        fp = fopen("strategy.txt", "r");
        break;
    case write_strategy:
        fp = fopen("strategy.txt", "w");
        break;
    case write_log:
        fp = fopen("latest.log", "w");
        break;
    default:
        break;
    }
    assert(fp);
    return fp;
}

inline unsigned long long read(){
	unsigned long long x;int ch;while((ch=getchar())<48||57<ch);
	x=ch^48;while(47<(ch=getchar())&&ch<58)x=x*10+(ch^48);
	return x;
}

#endif // LIBRARY_H