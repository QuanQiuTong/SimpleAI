/**
 * @file	GUI.cpp
 * @author	QuanQiuTong (https://github.com/QuanQiuTong)
 * @brief	A demo with GUI showing how the strategy works.
 *          具有图形界面的演示程序，展示策略表的效果。
 * @version	3.0 (Release)
 * @date	2022-12-05
 *
 * @copyright Copyright © 2022. 此作品采用 CC BY-NC-ND 4.0 许可授权. 转载请注明作者和出处.
 *
 * CC BY-NC-ND 4.0: https://creativecommons.org/licenses/by-nc-nd/4.0/
 *
 * 限于作者对规范等等熟悉程度不足，此作品中英文注释并存，还请海涵。
 *
 * Release-3.0版本：
 *     完善功能，如在GUI内增加了退出按钮；修正bug。
 */

#pragma GCC optimize(3)

#undef UNICODE
#undef _UNICODE

#include <conio.h>
#include <ctime>
#include <cstdlib>

#include "src.h"
#include "GUI.hpp"

static char buf[4096];
move_t strategy[STRATEGY_LEN] = {1, 4, 3, 3, 3, 3, 2, 4, 2, 2, 1, 2, 2, 3, 3, 2, 4, 2, 4, 4, 1, 1, 1, 1, 4, 4, 1, 3, 4, 3, 3, 3, 1, 1, 4, 1, 2, 1, 3, 2, 1, 1, 1, 1, 2, 1, 4, 1, 4, 3, 3, 1, 4, 1, 3, 4, 3, 3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 3, 3, 3, 3, 4, 4, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 4, 3, 3, 5, 2, 2, 2, 2, 3, 6, 2, 4, 5, 2, 2, 2, 2, 6, 6, 3, 1, 4, 3, 6, 1, 1, 4, 4, 2, 3, 5, 2, 2, 6, 6, 6, 4, 6, 3, 6, 2, 1, 2, 6, 3, 4, 3, 3, 1, 3, 1, 1, 1, 2, 2, 2, 4, 2, 6, 2, 2, 6, 3, 2, 2, 6, 2, 2, 2, 4, 2, 4, 4, 6, 3, 4, 3, 4, 2, 5};

int main(int argc, char const *argv[]);
int move(void);
void sgetm(void);
void decode(BYTE *);

int main(int argc, char const *argv[])
{
    decode(strategy);
    // "main()" version of the constants.
    const int M_LEFT = 20, M_TOP = 20, M_WIDTH = 600, M_BOTTOM = M_TOP + 240,
              BLANK = 20, TXT_HEI = 40, TXT_WID = 18,
              BODY = M_BOTTOM + BLANK,
              BODY_1 = BODY + BLANK + TXT_HEI,
              BODY_2 = BODY_1 + BLANK + TXT_HEI,
              BODY_3 = BODY_2 + BLANK + TXT_HEI,
              BODY_4 = BODY_3 + BLANK + TXT_HEI;
    const TCHAR menu[] = _T("Menu  ");

    {
        // Initialize the GUI.
        initgraph(WIDTH, HEIGHT);
        setbkcolor(BG_COLOR);
        cleardevice();
        settextstyle(TXT_HEI, TXT_WID, _T("Comic Sans MS"));
        settextcolor(DARKGRAY);
        setlinecolor(BLACK);
        setfillcolor(WHITE);
    }

    textbox iframe(M_LEFT, M_TOP, M_LEFT + M_WIDTH, M_BOTTOM - 1, 500);

    // A functor that refreshes the main screen. Make sure it's properly initialized.
    invoker_t main_screen = do_nothing;

    button buttons[] =
        {button(
             M_LEFT + textwidth(menu), BODY, _T("Run the Demo"), [&main_screen](void) -> void
             { puts("Clicked \"Run the Demo\"."); move(); main_screen(); },
             false, RED),
         button(
             M_LEFT, BODY_1, _T("Read Strategy from File"), []
             { FILE *fp = fopen("strategy.txt", "w+"); *buf=0; fread(buf, 1, sizeof buf, fp); sgetm(); fclose(fp); }),
         button(
             M_LEFT, BODY_2, _T("Read Strategy from Keyboard"), [&iframe]
             { memcpy_s(buf, sizeof buf, iframe.get_text().c_str(),iframe.get_text().length()); sgetm(); }),
         button(
             M_LEFT, BODY_3, _T("Set Randomseed (default 1)"), [&iframe]
             { int s = atoi(iframe.get_text().c_str()); set_random(s = s <= 0 ? time(NULL) : s); printf("Set randomseed to %d.\n", holdrand); }),
         button(M_LEFT, BODY_4, _T("Exit the Demo"), []
                { closegraph(); puts("\n\tWindow closed.\nIf needed, you can save the information above."); system("pause"); exit(0); })};

    main_screen = [menu, buttons]
    {
        setbkcolor(BG_COLOR);
        cleardevice();
        settextstyle(TXT_HEI, TXT_WID, _T("Comic Sans MS"));
        settextcolor(DARKGRAY);
        setlinecolor(BLACK);
        setfillcolor(WHITE);
        // Main Screen.
        fillrectangle(M_LEFT, M_TOP, M_LEFT + M_WIDTH, M_BOTTOM - 1);
        outtextxy(M_LEFT, BODY, menu);
        for (auto &&i : buttons)
            i.show();
    };
    main_screen();
    for (ExMessage msg;;)
    {
        msg = getmessage(EM_MOUSE); // 获取消息输入
        if (msg.message == WM_LBUTTONDOWN)
        {
            // 判断控件
            for (auto &&i : buttons)
                if (i.check(msg.x, msg.y))
                    i.on_message();

            // 判断控件
            if (iframe.check(msg.x, msg.y))
                iframe.on_message();
        }
    }

    /**
     * Actually the following sentences will never be executed,
     * they are here only to show how this demo-program ends properly.
     */
    system("pause");
    closegraph();
    return 0;
}

int move()
{
#define TXT_RIGHT_BOARDER (LEFT + (GRID_LEN + 1) * SIDELEN)
#define get_stragegy(x, y, g) \
    strategy[(size_t)(x == 9 ? 2 : g.grid[x + 1][y]) + (x ? g.grid[x - 1][y] : 2) * 3 + (y == 9 ? 2 : g.grid[x][y + 1]) * 9 + (y ? g.grid[x][y - 1] : 2) * 27 + g.grid[x][y] * 81]

    // Initialize the GRAPHIC panel.
    setbkcolor(BG_COLOR);
    cleardevice();
    settextstyle(30, 12, _T("Comic Sans MS"));
    settextcolor(GREEN);
    setfillcolor(LIGHTGRAY);
    rectangle(LEFT, TOP, LEFT + (GRID_LEN + 2) * SIDELEN, TOP + (GRID_WID + 2) * SIDELEN);
    button backward(TXT_RIGHT_BOARDER, 0, TXT_RIGHT_BOARDER + SIDELEN, TOP - 1, "Back");
    backward.show();

    // Initialize and SHOW the grid.
    grid_t g /*= {0}*/;
    unsigned cnt = refresh_grid(&g);
    for (size_t i = 0, j; i < GRID_WID; ++i)
        for (j = 0; j < GRID_LEN; ++j)
            draw(j, i, g.grid[j][i]);

    // SHOW step by step
    for (int i = STEP_NUM, x = 0, y = 0, score = 0, cmd; i--;)
    {
        sprintf(buf, "score:%5d    stragegy: ", score);

        switch ((cmd = get_stragegy(x, y, g)) || (cmd = random() % MOVE_MAX + 1), cmd)
        {
        case up:
            sprintf(buf + 25, "   up  ");
            y ? (draw(x, y, g.grid[x][y]), --y, draw(x, y, OBJ | g.grid[x][y]), NUL) : (score -= 5);
            break;
        case down:
            sprintf(buf + 25, " down  ");
            y != 9 ? (draw(x, y, g.grid[x][y]), ++y, draw(x, y, OBJ | g.grid[x][y]), NUL) : (score -= 5);
            break;
        case left:
            sprintf(buf + 25, " left  ");
            x ? (draw(x, y, g.grid[x][y]), --x, draw(x, y, OBJ | g.grid[x][y]), NUL) : (score -= 5);
            break;
        case right:
            sprintf(buf + 25, "right  ");
            x != 9 ? (draw(x, y, g.grid[x][y]), ++x, draw(x, y, OBJ | g.grid[x][y]), NUL) : (score -= 5);
            break;
        case pick:
            sprintf(buf + 25, " pick  ");
            g.grid[x][y] ? (g.grid[x][y] = false, draw(x, y, OBJ), score += 10) : (score -= 2);
            break;
        default:
            sprintf(buf + 25, " skip  ");
            break;
        }

        clearrectangle(LEFT, 0, TXT_RIGHT_BOARDER - 5, 29);
        outtextxy(LEFT, 0, buf);

        {
            ExMessage msg;
            for (clock_t start = clock(); clock() - start < clock_t(0.15 * CLOCKS_PER_SEC);)
                if (peekmessage(&msg, EM_MOUSE) && msg.lbutton && backward.check(msg.x, msg.y))
                    return 0;
        }
    }
#undef TXT_RIGHT_BOARDER
#undef get_strategy // 防止污染公共命名空间
    return 0;
}

/// @brief 忽略以“ # ”开头的行注释，并从buf读取策略表。
inline void sgetm()
{
    move_t strategy[STRATEGY_LEN];

    size_t len = 0;
    for (char *s = buf; len < STRATEGY_LEN;)
    {
        //文件操作必须谨慎
        if (s - buf >= 4096 || !*s)
        {
            puts("The file is too long,\nor the length of the strategy list is incorrect,\nor the file wasn't exist (and has been automatically created).\n");
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

    memcpy_s(::strategy, STRATEGY_LEN, strategy, STRATEGY_LEN);
    puts("Successfully read the strategy list from file \"strategy.txt\".");
    return;
}
