//#pragma GCC optimize(2)
#include <cstdio>
#include <iostream>
using namespace std;
inline int read()
{
    int x, ch;
    while ((ch = getchar()) < 48 || 57 < ch)
        ;
    for (x = ch ^ 48; 47 < (ch = getchar()) && ch < 58; x = x * 10 + (ch ^ 48))
        ;
    return x;
}
int main()
{
    {
        int msgtype = 0;
        const char *s = msgtype-- ? (msgtype-- ? "2Successfully read the strategy list."
                                               : "1hu")
                                  : "0Failed. Check stdout for infromation. ";
        cout << s;
    }

    cout << endl;

    {
        int msgtype = 0;
        const char *s = msgtype-- ? msgtype-- ? "2Successfully read the strategy list."
                                              : "1hu"
                                  : "0Failed. Check stdout for infromation. ";
        cout << s;
    }

    return 0;
}
