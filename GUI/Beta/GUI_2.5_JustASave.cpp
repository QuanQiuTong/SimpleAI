/**
 * @file    GUI.cpp
 * @author  QuanQiuTong (https://github.com/QuanQiuTong)
 * @brief   Get the buttons in an array; implement a textbox instance; improved the safety of getm().
 * @version 2.5
 * @date    2022-12-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma GCC optimize(3)

#undef UNICODE
#undef _UNICODE

#include <conio.h>
#include <time.h>

#include "src_ver_1_0.h"
#include "GUI.h"

char buf[4096];
move_t strategy[STRATEGY_LEN];

int main(int argc, char const *argv[]);
int move();
void getm(void);

int main(int argc, char const *argv[])
{
    // "main()" version of the constants.
    const int M_LEFT = 20, M_TOP = 20, M_WIDTH = 600, M_BOTTOM = M_TOP + 240,
              BLANK = 20, TXT_HEI = 40, TXT_WID = 18,
              BODY = M_BOTTOM + BLANK,
              BODY_1 = BODY + BLANK + TXT_HEI,
              BODY_2 = BODY_1 + BLANK + TXT_HEI,
              BODY_3 = BODY_2 + BLANK + TXT_HEI;
    const TCHAR menu[] = _T("Menu  "),
                run_the_demo[] = _T("Run the Demo"),
                rsff[] = _T("Read Strategy from File"),
                rsfk[] = _T("Read Strategy from Keyboard"),
                srs[] = _T("Set Randomseed (default 1)");
    bool show; // Temporary variable, for emulating python and java's language feature.

    {
        // Initialize the GUI.
        initgraph(WIDTH, HEIGHT);
        setbkcolor(0xeeeeee);
        cleardevice();
        settextstyle(TXT_HEI, TXT_WID, _T("Comic Sans MS"));
        settextcolor(DARKGRAY);
        setlinecolor(BLACK);
        setfillcolor(WHITE);

        // Frame for interaction.
        fillrectangle(M_LEFT, M_TOP, M_LEFT + M_WIDTH, M_BOTTOM - 1);

        outtextxy(M_LEFT, BODY, menu);
    }

    textbox interframe(M_LEFT, M_TOP, M_LEFT + M_WIDTH, M_BOTTOM - 1, 500);
    
    button buttons[] =
        {button(
             M_LEFT + textwidth(menu), BODY, run_the_demo, [](void) -> void
             { puts("Clicked."); },
             show = true, RED),
         button(
             M_LEFT, BODY_1, rsff, getm, show = true),
         button(
             M_LEFT, BODY_2, rsfk, []() {}, show = true),
         button(
             M_LEFT, BODY_3, srs, []() {}, show = true)};

    

    for (ExMessage msg;;)
    {
        msg = getmessage(EM_MOUSE); // 获取消息输入
        if (msg.message == WM_LBUTTONDOWN)
        {
            // 判断控件
            // if (txtName.Check(msg.x, msg.y))	txtName.OnMessage();

            // 判断控件
            // if (txtPwd.Check(msg.x, msg.y))   txtPwd.OnMessage();

            // 判断控件
            for (auto &&i : buttons)
                if (i.check(msg.x, msg.y))
                    i.on_message();
        }
    }

    system("pause");
    closegraph();
    return 0;
}
#define div_ceil(a, b) (a / b + !!(a % b))

#define get_stragegy(x, y, g) \
    strategy[(size_t)(x == 9 ? 2 : g.grid[x + 1][y]) + (x ? g.grid[x - 1][y] : 2) * 3 + (y == 9 ? 2 : g.grid[x][y + 1]) * 9 + (y ? g.grid[x][y - 1] : 2) * 27 + g.grid[x][y] * 81]

int move()
{

    //输入随机数种子
    // puts("Enter the seed for random-generated numbers (default 1).");
    // set_random(atoi(fgets(buf, 4095, stdin)));
    set_random(time(NULL));

    settextstyle(30, 12, _T("Comic Sans MS"));
    settextcolor(GREEN);
    setfillcolor(LIGHTGRAY);
    rectangle(LEFT, TOP, (GRID_LEN + 2) * SIDELEN + LEFT, (GRID_WID + 2) * SIDELEN + TOP);

    //初始化并打印棋盘
    grid_t g = {0};
    unsigned cnt = refresh_grid(&g);
    for (size_t i = 0, j; i < GRID_WID; ++i)
        for (j = 0; j < GRID_LEN; ++j)
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

    return 0;
}

/// @brief 忽略以“ # ”开头的行注释，并读取策略表。
inline void getm()
{
    move_t strategy[STRATEGY_LEN];

    FILE *fp = fopen("strategy.txt", "r");
    if (!fp)
    {
        puts("Failed to open file \"strategy.txt\".");
        return;
    }
    fread(buf, 1, 4095, fp);
    size_t len = 0;
    for (char *s = buf; len < STRATEGY_LEN;)
    {
        //文件操作必须谨慎
        if (s - buf >= 4096 || !*s)
        {
            puts("The file is too long,\nor the length of the strategy list is incorrect.");
            return;
        }
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
            puts("Your strategy list has a wrong format!\n");
            return;
            break;
        }
    }
    if (len != STRATEGY_LEN)
    {
        printf("Strategy length is wrong, which should be %u.\n", STRATEGY_LEN);
        return;
    }
    fclose(fp);

    memcpy_s(::strategy, STRATEGY_LEN, strategy, STRATEGY_LEN);
    puts("Successfully read the strategy list from file \"strategy.txt\".");
    return;
}

#undef get_strategy
#undef div_ceil //防止污染公共命名空间
