#include <iostream>
#include <cmath>
#include <cstdio>
inline unsigned long long random() //废弃
{
    static unsigned long long x = (size_t)1; // this works like "srand(time(0))".
    return (x *= 998244353u);
}
long __cdecl rand2(void)
{
    static long holdrand = 1;
    return (((holdrand = holdrand * 214013L + 2531011L) >> 16) /*& 0x7fff*/);
}
unsigned __cdecl random2(void)
{
    static unsigned long long holdrand = 1;
    return (((holdrand = holdrand * 214013ULL + 2531011ULL) >> 32) & LONG_MAX);
}
inline size_t probability(size_t x)
{
    return sqrt(random2() % (x * x));
}
int main()
{
    // for (size_t i = 0; i < 20; i++)
    //     std::cout << rand() << ' ' << rand2() << std::endl;

    double cnt[200], times = 1e6;
    for (size_t i = 0; i < times; i++)
        cnt[probability(20u)] += 1;
    for (size_t i = 0; i < 30; i++)
        std::cout << probability(20) << ' ';
    std::cout << std::endl;

    for (size_t i = 0; i < 20; i++)
        printf("%d: %.3lf\n", i, cnt[i] / times * 400);
}