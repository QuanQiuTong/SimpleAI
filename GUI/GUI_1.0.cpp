/**
 * @file    GUI.cpp
 * @author  傅全通 (fu.quantong@foxmail.com)
 * @brief   A demo showing how Yellow-chan moves.
 * @version 1.0
 * @date    2022-12-2
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma GCC optimize(3)

#include <graphics.h>
#include <conio.h>
#include <time.h>

#include "library.h"
#include "grid.h"
#include "revolution.h"

enum draw_type
{
    NUL = 0x0,
    CAN = 0x1,
    OBJ = 0x2
};
unsigned width = 3;
char buf[4096];
const unsigned WIDTH = 640, HEIGHT = 640, TOP = 30, LEFT = 20, SIDELEN = 50;

int main(int argc, char *argv[]);
void getm(move_t *const);
void draw(const int x, const int y, const signed draw_type);

#define div_ceil(a, b) (a / b + !!(a % b))

#define get_stragegy(x, y, g) \
    strategy[(size_t)(x == 9 ? 2 : g.grid[x + 1][y]) + (x ? g.grid[x - 1][y] : 2) * 3 + (y == 9 ? 2 : g.grid[x][y + 1]) * 9 + (y ? g.grid[x][y - 1] : 2) * 27 + g.grid[x][y] * 81]

int main(int argc, char *argv[])
{
    //读入策略表
    move_t strategy[STRATEGY_LEN];
    getm(strategy);
    puts("Successfully read the strategy table from file \"strategy.txt\".");

    //输入随机数种子
    // puts("Enter the seed for random-generated numbers (default 1).");
    // set_random(atoi(fgets(buf, 4095, stdin)));
    set_random(time(NULL));

    //初始化图形用户界面
    initgraph(WIDTH, HEIGHT);
    settextstyle(30, 12, _T("Comic Sans MS"));
    settextcolor(GREEN);
    setfillcolor(LIGHTGRAY);
    rectangle(LEFT, TOP, (GRID_LEN + 2) * SIDELEN + LEFT, (GRID_WID + 2) * SIDELEN + TOP);

    //初始化并打印棋盘
    grid_t g = {0}; //任何东西都可以用{0}这个“初始化列表”（C++）或者“复合字面量”（C99）来置为0
    // for (size_t i = 0, j; i < GRID_WID; ++i) for (j = 0; j < GRID_LEN; ++j) draw(j,i,g.grid[j][i]);//Sleep(5000);
    unsigned cnt = refresh_grid(&g);
    for (size_t i = 0, j; i < GRID_WID; ++i) // representing line-number
        for (j = 0; j < GRID_LEN; ++j)       // representing col-number
            draw(j, i, g.grid[j][i]);
    // Sleep(100000);

    //逐步或者非逐步显示棋盘
    for (int i = STEP_NUM, x = 0, y = 0, score = 0, cmd; i--;)
    {
        clearrectangle(LEFT, 0, 600, 29);
        sprintf(buf + 0, "score:%5d", score);
        sprintf(buf + 11, "    stragegy: ");

        switch ((cmd = get_stragegy(x, y, g)) || (cmd = random() % MOVE_MAX + 1), cmd)
        {
        case up:
            sprintf(buf + 25, "   up  ");
            y ? (draw(x, y, g.grid[x][y]), --y, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case down:
            sprintf(buf + 25, " down  ");
            y != 9 ? (draw(x, y, g.grid[x][y]), ++y, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case left:
            sprintf(buf + 25, " left  ");
            x ? (draw(x, y, g.grid[x][y]), --x, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case right:
            sprintf(buf + 25, "right  ");
            x != 9 ? (draw(x, y, g.grid[x][y]), ++x, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case pick:
            sprintf(buf + 25, " pick  ");
            g.grid[x][y] ? (g.grid[x][y] = false, draw(x, y, OBJ), score += 10) : (score -= 2);
            break;
        default:
            sprintf(buf + 25, " skip  ");
            break;
        }
        outtextxy(LEFT, 0, buf);
        Sleep(150);
    }
    system("pause");
    return 0;
}

/// @brief 忽略以“ # ”开头的行注释，并读取策略表。
inline void getm(move_t strategy[STRATEGY_LEN])
{
    FILE *fp = openfile(read_strategy);
    fread(buf, 1, 4095, fp);
    size_t len = 0;
    for (char *s = buf; len < STRATEGY_LEN;)
    {
        //文件操作必须谨慎
        assert(("文件太大，或者策略表长度不正确", s - buf < 4096 && *s));
        switch (*s)
        {
        //空白字符
        case ' ':
        case '\t':
        case '\n':
        case ',':
            ++s;
            break;
        //行注释
        case '#':
            while (*++s != '\n')
                ;
            ++s;
            break;
            // case '\0':即缓冲区结束，判断过了
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            strategy[len++] = *s++ ^ '0';
            break;
        default:
            printf("策略表格式错误！\n");
            assert(NULL);
            break;
        }
    }
    assert(("Strategy length is wrong, which should be 243.\nOther information:\n", len == 243));
    fclose(fp);

    return;
}

/// @brief 在( %x , %y )位置，画 %draw_type 指定的图形样式
/// @param draw_type 指定该位置为空还是有罐子，以及小黄是否在上面
inline void draw(const int x, const int y, const signed draw_type)
{
    const int left = LEFT + (x + 1) * SIDELEN, top = TOP + (y + 1) * SIDELEN,
              right = left + SIDELEN - 1, bottom = top + SIDELEN - 1;
    clearrectangle(left, top, right, bottom); // 先清掉，不然画不了空方格
    (draw_type & CAN) ? fillrectangle(left, top, right, bottom) : rectangle(left, top, right, bottom);
    setfillcolor(YELLOW);
    (draw_type & OBJ) && (solidellipse((3 * left + right) >> 2, (3 * top + bottom) >> 2, (left + 3 * right) >> 2, (top + 3 * bottom) >> 2), NULL);
    setfillcolor(LIGHTGRAY);
}

#undef get_strategy
#undef div_ceil //防止污染公共命名空间
