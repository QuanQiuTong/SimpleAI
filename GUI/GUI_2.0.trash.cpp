/**
 * @file	GUI.cpp
 * @author	傅全通 (fu.quantong@foxmail.com)
 * @brief	A demo showing how Yellow-chan moves, using EasyX.
 * @version	2.0
 * @date	2022-12-2
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma GCC optimize(3)

#define _UNICODE // for crtlib
#define UNICODE  // for WINAPI

#include <graphics.h>
#include <conio.h>
#include <time.h>

#include "library.h"
#include "grid.h"
#include "revolution.h"

// 实现按钮控件
class EasyButton
{
private:
	int left , top , right , bottom ;	// 控件坐标
	wchar_t* text ;							// 控件内容
	// 为了展示，不使用std::function<>了，还是用传统的函数指针
	void (*userfunc)() ;						// 控件消息

public:
	EasyButton(int left, int top, int right, int bottom, const wchar_t* title, void (*func)()=NULL):
		left(left),top(top),right(right),bottom(bottom),userfunc(func)
	{
		text = new wchar_t[wcslen(title) + 1];
		wcscpy_s(text, wcslen(title) + 1, title);

		// 绘制用户界面
		Show();
	}

	~EasyButton()
	{
		if (text != NULL)
			delete[] text;
	}

	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	// 绘制界面
	void Show()
	{
		COLORREF oldlinecolor = getlinecolor();
		COLORREF oldbkcolor = getbkcolor();
		COLORREF oldfillcolor = getfillcolor();

		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);
		
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
	}

	void OnMessage()
	{
		if (userfunc != NULL)
			userfunc();
	}
};

void button_clicked(){
	//fuck!
}

enum draw_type
{
    NUL = 0x0,
    CAN = 0x1,
    OBJ = 0x2
};
unsigned width = 3;
char buf[4096];
TCHAR wbuf[4096];
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
    move_t strategy[STRATEGY_LEN]={1,4,3,3,3,3,2,4,2,2,1,2,2,3,3,2,4,2,4,4,1,1,1,1,4,4,1,3,4,3,3,3,1,1,4,1,2,1,3,2,1,1,1,1,2,1,4,1,4,3,3,1,4,1,3,4,3,3,4,2,2,2,2,2,2,2,2,2,2,2,2,2,3,4,3,3,3,3,4,4,4,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,4,4,3,3,5,2,2,2,2,3,6,2,4,5,2,2,2,2,6,6,3,1,4,3,6,1,1,4,4,2,3,5,2,2,6,6,6,4,6,3,6,2,1,2,6,3,4,3,3,1,3,1,1,1,2,2,2,4,2,6,2,2,6,3,2,2,6,2,2,2,4,2,4,4,6,3,4,3,4,2,5};
    getm(strategy);
    puts("Successfully read the strategy table from file \"strategy.txt\".");

    //输入随机数种子
    // puts("Enter the seed for random-generated numbers (default 1).");
    // set_random(atoi(fgets(buf, 4095, stdin)));
    set_random(time(NULL));

    //初始化图形用户界面
    initgraph(WIDTH, HEIGHT);
    setbkcolor(0xdddddd);
    cleardevice();
    settextstyle(30, 12, _T("Comic Sans MS"));
    settextcolor(GREEN);
    setfillcolor(LIGHTGRAY);
    
    EasyButton btn(LEFT+8*SIDELEN,TOP,LEFT+10*SIDELEN-1,TOP+29,L"Test",button_clicked);
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
        swprintf(wbuf + 0, _T("score:%5d"), score);
        swprintf(wbuf + 11,_T("    stragegy: "));

        switch ((cmd = get_stragegy(x, y, g)) || (cmd = random() % MOVE_MAX + 1), cmd)
        {
        case up:
            swprintf(wbuf + 25,_T("   up  "));
            y ? (draw(x, y, g.grid[x][y]), --y, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case down:
            swprintf(wbuf + 25, _T(" down  "));
            y != 9 ? (draw(x, y, g.grid[x][y]), ++y, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case left:
            swprintf(wbuf + 25, _T(" left  "));
            x ? (draw(x, y, g.grid[x][y]), --x, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case right:
            swprintf(wbuf + 25, _T("right  "));
            x != 9 ? (draw(x, y, g.grid[x][y]), ++x, draw(x, y, OBJ | g.grid[x][y]), NULL) : (score -= 5);
            break;
        case pick:
            swprintf(wbuf + 25, _T(" pick  "));
            g.grid[x][y] ? (g.grid[x][y] = false, draw(x, y, OBJ), score += 10) : (score -= 2);
            break;
        default:
            swprintf(wbuf + 25, _T(" skip  "));
            break;
        }
        outtextxy(LEFT, 0, wbuf);
        Sleep(150);
    }
    system("pause");
    return 0;
}


/// @brief 忽略以“ # ”开头的行注释，并读取策略表。
inline void getm(move_t strategy[STRATEGY_LEN])
{
    FILE *fp = _wfopen(_T("strategy.txt"),L"r");
    assert(fp);
    fread(buf, 1, 4095, fp);
    size_t len = 0;
    for (char *s = buf; len < STRATEGY_LEN;)
    {
        //文件操作必须谨慎
        assert((L"文件太大，或者策略表长度不正确", s - buf < 4096 && *s));
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
            wprintf(L"策略表格式错误！\n");
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
