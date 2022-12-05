/**
 * @file    GUI.hpp
 * @author  QuanQiuTong (https://github.com/QuanQiuTong)
 * @brief   Implement button and textbox class. Notice that std must be C++11 or later.
 *          实现按钮和文本框类，具有范例参考价值。需要使用C++11及以后的标准来编译。
 * @version 2.0 (Release)
 * @date    2022-12-05
 *
 * @copyright Copyright © 2022. 此作品采用 CC BY-NC-ND 4.0 许可授权. 转载请注明作者和出处.
 *
 * CC BY-NC-ND 4.0: https://creativecommons.org/licenses/by-nc-nd/4.0/
 *
 * 限于作者对规范等等熟悉程度不足，此作品中英文注释并存，还请海涵。
 *
 * Release版本：
 *     完善大量细节，补充许多注释。
 */


#pragma once
#ifndef _INC_GUI
#define _INC_GUI // include guard for old interop

#ifndef __cplusplus
#error Class needs C++ to work properly.
#endif

#undef UNICODE
#undef _UNICODE

#include "graphics.h"
#include <string>
#include <algorithm>

// Notice:
// Color in the screen buffer is represented as "BGR"(in form of "0xBBGGRR").
#define LIGHT_YELLOW ((COLORREF)0xBFFFFF)
#define BG_COLOR ((COLORREF)0xEEEEEE)

#if __cplusplus // >=201103L
#include <functional>
typedef std::function<void(void)> invoker_t;
#else // C and C++98
typedef void (*invoker_t)(void);
#endif

// An empty function for innitializing invokers.
void do_nothing()
{
    // Literally "do nothing".
}

class button
{
    // typedef std::function<void(void)> invoker_t;

private:
    int left, top, right, bottom;
    std::string title;
    invoker_t invoker;
    COLORREF textcolor;

public:
    // The most straightforward ctor; but it's difficult to call.
    button(int l, int t, int r, int b, std::string title, invoker_t invoke_func = do_nothing, bool show = false, COLORREF textcolor = BLACK) : left(l), top(t), right(r), bottom(b), title(title), invoker(invoke_func), textcolor(textcolor)
    {
        if (show)
            this->show();
    }

    // Two simplified versions.
    button(int left, int top, TCHAR const *title, invoker_t invoke_func = do_nothing, bool show = false, COLORREF textcolor = BLACK) : left(left), top(top), right(left + textwidth(title)), bottom(top + textheight(title)), title(title), invoker(invoke_func), textcolor(textcolor)
    {
        if (show)
            this->show();
    }
    button(int left, int top, std::string title, invoker_t invoke_func = do_nothing, bool show = false, COLORREF textcolor = BLACK) : left(left), top(top), right(left + textwidth(title.c_str())), bottom(top + textheight(title.c_str())), title(title), invoker(invoke_func), textcolor(textcolor)
    {
        if (show)
            this->show();
    }

    // Class button is not POD, so let the compiler destruct it.
    // ~button() { }

    // Check if the mouse is in the object's zone.
    bool check(int x, int y)
    {
        return (left <= x && x <= right && top <= y && y <= bottom);
    }

    // Draw the graphic user interface.
    void show() const
    {
        const TCHAR *text = title.c_str(); //权宜之计，为了适配C-API

        COLORREF oldlinecolor = getlinecolor();
        COLORREF oldbkcolor = getbkcolor();
        COLORREF oldfillcolor = getfillcolor();
        COLORREF oldtextcolor = gettextcolor();

        setlinecolor(BLACK);      // 设置画线颜色
        setbkcolor(LIGHT_YELLOW); // 设置背景颜色
        setfillcolor(WHITE);      // 设置填充颜色
        settextcolor(textcolor);

        fillrectangle(left - 1, top - 1, right, bottom); // No need to do so: // rectangle(left, top, right, bottom);
        outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);

        setlinecolor(oldlinecolor);
        setbkcolor(oldbkcolor);
        setfillcolor(oldfillcolor);
        settextcolor(oldtextcolor);
    }

    // Call the invoker. Or rather, deal with the click-event.
    void on_message() { invoker(); }
};

class textbox
{
private:
    int left, top, right, bottom;
    std::string text;
    size_t maxlen; // Though I did not use it much, it might be used somewhere else.

public:
    textbox(int l, int t, int r, int b, size_t maxlen, std::string text = "") : left(l), top(t), right(r), bottom(b), text(text), maxlen(maxlen) {}

    // Class textbox is not POD, so let the compiler destruct it.
    // ~textbox() { }

    std::string get_text()
    {
        return text;
    }

    // Check if the mouse is in the object's zone.
    bool check(int x, int y)
    {
        return (left <= x && x <= right && top <= y && y <= bottom);
    }

    /**
     * @brief Draw the graphic user interface.
     *
     * Though I did not use this function,
     * it should be shown here as a good sample.
     */
    void show()
    {
        const TCHAR *text = this->text.c_str();

        COLORREF oldlinecolor = getlinecolor();
        COLORREF oldbkcolor = getbkcolor();
        COLORREF oldfillcolor = getfillcolor();

        setlinecolor(LIGHTGRAY); // 设置画线颜色
        setbkcolor(0xeeeeee);    // 设置背景颜色
        setfillcolor(0xeeeeee);  // 设置填充颜色
        fillrectangle(left, top, right, bottom);
        outtextxy(left + 10, top + 5, text);

        setlinecolor(oldlinecolor);
        setbkcolor(oldbkcolor);
        setfillcolor(oldfillcolor);
    }

    // Call the invoker. Or rather, deal with the inputting-event.
    void on_message()
    {
        TCHAR *text = new TCHAR[maxlen];
        memset(text, 0, maxlen * sizeof(TCHAR));

        COLORREF oldlinecolor = getlinecolor();
        COLORREF oldbkcolor = getbkcolor();
        COLORREF oldfillcolor = getfillcolor();

        setlinecolor(BLACK); // 设置画线颜色
        setbkcolor(WHITE);   // 设置背景颜色
        setfillcolor(WHITE); // 设置填充颜色
        fillrectangle(left, top, right, bottom);
        outtextxy(left + 10, top + 5, text);

        int width = textwidth(text); // 字符串总宽度
        int counter = 0;             // 光标闪烁计数器
        bool binput = true;          // 是否输入中

        ExMessage msg;
        while (binput)
        {
            while (binput && peekmessage(&msg, EM_MOUSE | EM_CHAR, false)) // 获取消息，但不从消息队列拿出
            {
                if (msg.message == WM_LBUTTONDOWN)
                {
                    // 如果鼠标点击文本框外面，结束文本输入
                    if (msg.x < left || msg.x > right || msg.y < top || msg.y > bottom)
                    {
                        binput = false;
                        break;
                    }
                }
                else if (msg.message == WM_CHAR)
                {
                    size_t len = strlen(text);
                    switch (msg.ch)
                    {
                    case '\b': // 用户按退格键，删掉一个字符
                        if (len > 0)
                        {
                            text[len - 1] = 0;
                            width = textwidth(text);
                            counter = 0;
                            clearrectangle(left + 10 + width, top + 1, right - 1, bottom - 1);
                        }
                        break;
                    case '\r': // 用户按回车键，结束文本输入
                    case '\n':
                        binput = false;
                        break;
                    default: // 用户按其它键，接受文本输入
                        if (len < maxlen - 1)
                        {
                            text[len++] = msg.ch;
                            text[len] = 0;

                            clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // 清除画的光标
                            width = textwidth(text);                                                           // 重新计算文本框宽度
                            counter = 0;
                            outtextxy(left + 10, top + 5, text); // 输出新的字符串
                        }
                    }
                }
                peekmessage(NULL, EM_MOUSE | EM_CHAR); // 从消息队列抛弃刚刚处理过的一个消息
            }

            // 绘制光标（光标闪烁周期为 20ms * 32）
            counter = (counter + 1) % 32;
            if (counter < 16)
                line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // 画光标
            else
                clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // 擦光标

            // 延时 20ms
            Sleep(20);
        }

        clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3); // 擦光标

        setlinecolor(oldlinecolor);
        setbkcolor(oldbkcolor);
        setfillcolor(oldfillcolor);

        show();

        // Notice that '=' is actually "std::string::operator=", so this will work nicely.
        this->text = text;
        delete[] text;
    }
};

#define WIDTH 640
#define HEIGHT 640
#define TOP 30
#define LEFT 20
#define SIDELEN 50

void draw(const int x, const int y, const signed draw_type);

enum draw_type
{
    NUL = 0x0,
    CAN = 0x1,
    OBJ = 0x2
};
// 在( %x , %y )位置，画 %draw_type 指定的图形样式
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

#endif
