#include<stdio.h>
struct test
{
    long long x;
    char s[0];
    char g[10][10];
}t;

int main(){
    printf("%p\n",t.s);
    struct test *p=&t;
    
    printf("%d",p->g[5][5]);
}