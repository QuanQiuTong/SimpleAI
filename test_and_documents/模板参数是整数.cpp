// #pragma GCC optimize(2)
#include <cstdio>
#include <iostream>
using namespace std;

int Result;
// 主模板
template <int N> // 模板
class Fibonacci
{
public:
    enum
    {
        Result = Fibonacci<N - 1>::Result + Fibonacci<N - 2>::Result
    };
    // 枚举，带有隐含计算
};
// 完全特化模板
template <>
class Fibonacci<1> // 带常参数1的构造函数
{
public:
    enum
    {
        Result = 1
    };
    // 给枚举赋初值1
};
// 完全特化模板
template <>
class Fibonacci<0> // 带参数0的模板
{
public:
    enum
    {
        Result = 0
    };
    // 给枚举赋初值0
};

template <int N>
struct fib
{
    enum
    {
        x = fib<N - 1>::x + fib<N - 2>::x
    };
};

template <>
struct fib<1>
{
    enum
    {
        x = 1
    };
};
template <>
struct fib<0>
{
    enum
    {
        x = 0
    };
};
int main()
{
    // std::cout << "第20项的Fibonacci数是：" << Fibonacci<20>::Result << std::endl;
    //  隐含计算

    cout << fib<20>::x;
    system("pause");
    return 1;
}
enum ChessPiece
{
    EMPTY_SQUARE,
    WHITE_PAWN /* 及其他 */
};
enum PlayerColor
{
    PC_WHITE,
    PC_BLACK
};
class ChessBoard
{
public:
    ChessPiece getPiece(int x, int y);
    PlayerColor getMove();
    void makeMove(int from_x, int from_y, int to_x, int to_y);

private:
    ChessPiece _board[8][8];
    PlayerColor _whose_move;
};
// 方法的定义和之前完全相同！
ChessPiece ChessBoard::getPiece(int x, int y)
{
    return _board[x][y];
}
PlayerColor ChessBoard::getMove()
{
    return _whose_move;
}
void ChessBoard::makeMove(int from_x, int from_y, int to_x, int to_y)
{
    // 通常情况下，首先需要写点代码验证移动棋子的合法性
    _board[to_x][to_y] = _board[from_x][from_y];
    _board[from_x][from_y] = EMPTY_SQUARE;
}