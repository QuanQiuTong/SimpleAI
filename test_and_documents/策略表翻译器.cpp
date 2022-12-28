#include <fstream>
using namespace std;

inline unsigned trans(unsigned x)
{
    return x == 2 ? 2 : 1-x;
}
int main()
{
    fstream fin("tobetranslate.txt", ios::in);
    unsigned buf[243];
    for (size_t i = 0; i < 243; i++)
        fin >> buf[i];
    fin.close();
    //上下左右中 -> 东西南北中 (01234)
    // 0空 1罐 2墙 -> 0罐 1空 2墙
    fstream fout("strategy.txt", ios::out);
    for (size_t i = 0, j, up, down, left, right, middle; i < 243; i++)
    {
        right = trans((j = i) % 3);
        left = trans((j /= 3) % 3);
        down = trans((j /= 3) % 3);
        up = trans((j /= 3) % 3);
        middle = trans((j / 3) % 3);
        fout << buf[up + down * 3 + left * 9 + right * 27 + middle * 81] << ' ';
    }
    fout.close();
    return 0;
}