#include <graphics.h>
int __cdecl system(const char *_Command);
int main()
{
	initgraph(600, 400);int left, top, right, bottom;

	setbkcolor(YELLOW); // 背景色
	cleardevice();

	setlinecolor(BLACK); // 划线和边框的颜色
	rectangle(left = 250, top = 250, right = 349, bottom = 349);

	setfillcolor(LIGHTGRAY); // 填充的颜色
	fillrectangle(left = 20, top = 20, right = 199, bottom = 199);

	settextcolor(RED);					   // 文本的颜色
	setfont(40, 16, "Microsoft YaHei UI"); // 字体：微软雅黑
	/*等同于*/ settextstyle(40, 16, "Microsoft YaHei UI");
	outtextxy(20, 300, "Not Hello_world"); // 指定位置输出文本

	setfillcolor(GREEN); // 每次fill之前要重新填色
	solidellipse(left = 300, top = 20, right = 399, bottom = 119);























	loadimage(NULL, "Yellow.jpeg");

	system("pause");
}