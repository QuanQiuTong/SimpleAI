#include <stdio.h>
int read()
{
    int x, ch;
    while ((ch = getchar()) < 48 || 57 < ch)
        ;
    for (x = ch ^ 48; 47 < (ch = getchar()) && ch < 58; x = x * 10 + (ch ^ 48))
        ;
    return x;
}
int main(int argc, char const *argv[])
{
    union
    {
        unsigned char byte[256];
        unsigned long dword[64];
    } foo;

    long efface = 0x1efface1;
    for (unsigned i = 0; i < 243u; ++i)
    {
        foo.byte[i] = read();
    }
    for (size_t i = 0; i < 64u; ++i)
    {
        foo.dword[i] ^= efface;
    }
    /* for (unsigned i = 0; i < 243u; ++i)
    {
        printf("%d, ", foo.byte[i]);
    } */
    for (unsigned i = 0; i < 64u; ++i)
    {
        printf("0X%X, ", foo.dword[i]);
    }

    return 0;
}
