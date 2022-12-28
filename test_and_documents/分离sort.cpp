#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
using std::sort;
int main()
{
    int score[] = {30, 40, 50, 10, 20}, per[] = {0, 1, 2, 3, 4};
    sort(per, per + 5, [score](int a, int b)
         { return score[a] < score[b]; });
    //得到score[]中每个下标对应元素的排名
    for (size_t i = 0; i < 5; i++)
        cout << per[i] << ' ';
    cout << endl;
    //也可用作对score(可能复制开销很大)构造一个用于排序的辅助数组
    for (size_t i = 0; i < 5; i++)
        cout << score[per[i]] << ' ';
}