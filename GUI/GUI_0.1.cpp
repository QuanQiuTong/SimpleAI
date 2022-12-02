/**
 * @file    GUI_0.1.cpp
 * @author  傅全通 (fu.quantong@foxmail.com)
 * @brief   尝试绘制GUI的大体形状（核心部分）
 * @version 0.1
 * @date    2022-12-2
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma GCC optimize(3)

#include <graphics.h>
#include <conio.h>
#include <stdlib.h>

#include "library.h"
#include "object.h"
#include "grid.h"
#include "revolution.h"

unsigned width = 3;
char buf[4096];

int main(int argc, char *argv[]);
void getm(move_t *const);

#define get_stragegy(x, y, g) \
    strategy[(size_t)(x == 9 ? 2 : g.grid[x + 1][y]) + (x ? g.grid[x - 1][y] : 2) * 3 + (y == 9 ? 2 : g.grid[x][y + 1]) * 9 + (y ? g.grid[x][y - 1] : 2) * 27 + g.grid[x][y] * 81]

int main(int argc, char *argv[])
{
    //读入策略表
    move_t strategy[STRATEGY_LEN];
    getm(strategy);
    puts("Successfully inputted the strategy table.");

    //输入随机数种子
    puts("Enter the seed for random-generated numbers (default 1).");
    set_random(atoi(fgets(buf, 4095, stdin)));

    const unsigned WIDTH = 640, HEIGHT = 640, TOP = 30, LEFT = 20, SIZE = 50;

    initgraph(WIDTH, HEIGHT);
    settextstyle(30, 12, _T("Comic Sans MS"));
    settextcolor(YELLOW);

    rectangle(LEFT, TOP, 600 + LEFT, 600 + TOP);

    grid_t g;

    for (size_t i = 0, j; i < GRID_WID; ++i) // representing line-number
    {
        for (j = 0; j < GRID_LEN; ++j) // representing col-number
            if (g.grid[j][i])
                fillrectangle(LEFT + (j + 1) * SIZE, TOP + (i + 1) * SIZE, LEFT + (j + 2) * SIZE - 1, TOP + (i + 2) * SIZE - 1);
            else
                rectangle(LEFT + (j + 1) * SIZE, TOP + (i + 1) * SIZE, LEFT + (j + 2) * SIZE - 1, TOP + (i + 2) * SIZE - 1);
    }
    Sleep(100000);

    //逐步或者非逐步显示棋盘
    for (int i = 0, x = 0, y = 0, cnt = refresh_grid(&g), score = 0, cmd; i < STEP_NUM; ++i)
    {
        clearrectangle(LEFT, 0, 200, 29);
        sprintf(buf, "score:%5d\n", score);
        outtextxy(LEFT, 0, buf);
        // Sleep(100);

        printf("next step: ");
        (cmd = get_stragegy(x, y, g)) || (cmd = random() % MOVE_MAX + 1);
        switch (cmd)
        {
        case up:
            puts("   up  ");
            y ? --y : (score -= 5);
            break;
        case down:
            puts(" down  ");
            y != 9 ? ++y : (score -= 5);
            break;
        case left:
            puts(" left  ");
            x ? --x : (score -= 5);
            break;
        case right:
            puts("right  ");
            x != 9 ? ++x : (score -= 5);
            break;
        case pick:
            puts(" pick  ");
            if (g.grid[x][y])
            {
                g.grid[x][y] = false;
                score += 10;
            }
            else
                score -= 2;
            break;
        default:
            puts(" skip  ");
            break;
        }
        for (int j = 0; j <= width * (GRID_LEN + 1); ++j)
            putchar('_');
        puts("\n");
    }
    return 0;
}
#undef get_stragegy

/// @brief 忽略以“ # ”开头的行注释，并读取策略表。
inline void getm(move_t strategy[STRATEGY_LEN])
{
    FILE *fp = openfile(read_strategy);
    static char *s = buf;
    fread(buf, 1, 4095, fp);
    size_t len = 0;
    for (; len < STRATEGY_LEN;)
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
#ifdef ECHO
    // puts(buf);
    puts("echo:");
    for (size_t i = 0; i < STRATEGY_LEN; ++i)
        printf("%d\t", strategy[i]);
    putchar('\n');
#endif
    return;
}
