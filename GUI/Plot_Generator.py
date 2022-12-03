import numpy as npy
from matplotlib import pyplot as plt
import re
from math import floor, ceil


def isfloat(x):
    '''简单粗暴（迫真）判断是浮点数'''
    if x in ['inf', 'infinity', 'INF', 'INFINITY', True, 'NAN', 'nan', False, '-inf', '-INF', '-INFINITY', '-infinity', 'NaN', 'Nan']:
        return False
    try:
        float(x)
        return True
    except:
        return False


# arr = npy.arange(start=0, stop=1000, step=1,)
with open("latest.log") as fobj:
    lines = fobj.readlines()
# end with

'''
# 测试用代码，检查是否能够正确读入
testd =[]
for line in lines:
    for str in re.split('\'|\s', line[:-2]):
        # 把\n和句号都吃了
        # print(str,end=' ')
        if str.isdigit():
            testd.append(int(str))
#    print(end='\n')
print(testd)
'''

x = []
y = []
n = 0
nn = 0

# 解读log文件
for line in lines:
    for str in re.split('\'|\s', line[:-2]):
        if str.isdigit():
            x.append(int(str))
            nn += 1
        elif isfloat(str):
            y.append(float(str))
            n += 1

# 异常处理
# 这一行测试用，如非必要不要启用： ## n=0;
try:
    if n != nn:
        raise Exception
except:
    print('日志文件中散点的横纵坐标个数不相等！\n已经自动终止程序')
    raise Exception
    # while True:
    #    input()

fig = plt.figure(1)

# 3. 设置颜色 color 值【可选参数，即可填可不填】，方式有几种
# colors = npy.random.rand(n) # 随机产生10个0~1之间的颜色值，或者
colors = ['r', 'g', 'y', 'b', 'r', 'c', 'g', 'b', 'k', 'm']  # 可设置随机数取
color = 'y'

# 4. 设置点的面积大小 area 值 【可选参数】
areas = 20*npy.arange(1, n+1)
area = 1

# 5. 设置点的边界线宽度 【可选参数】
widths = npy.arange(n)  # 0-9的数字
width = 1

# 6. 正式绘制散点图：scatter
plt.scatter(x, y, s=area, c=color, linewidths=width, alpha=0.5, marker='o')

# 7. 设置轴标签：xlabel、ylabel
# 设置X轴标签
plt.xlabel('Generation')
# 设置Y轴标签
plt.ylabel('Score')
# 8. 设置图标题：title
plt.title('Plot')

# 9. 设置轴的上下限显示值：xlim、ylim
# 设置横轴的上下限值
plt.xlim(0, n+1)
# 设置纵轴的上下限值
ymin = floor(npy.min(y)/10)*10-10
ymax = ceil(npy.max(y)/10)*10+10
plt.ylim(ymin, ymax)

# 10. 设置轴的刻度值：xticks、yticks
# 设置横轴精准刻度
## plt.xticks(npy.arange(npy.min(x)-0.2, npy.max(x)+0.2, step=0.3))
# 设置纵轴精准刻度
## plt.yticks(npy.arange(npy.min(y)-0.2, npy.max(y)+0.2, step=0.3))
# 也可按照xlim和ylim来设置
# 设置横轴精准刻度
plt.xticks(npy.arange(0, n+1, step=100))
# 设置纵轴精准刻度
plt.yticks(npy.arange(ymin, ymax, step=50))

# 11. 在图中某些点上（位置）显示标签：annotate
# plt.annotate("(" + str(round(x[2], 2)) + ", " + str(round(y[2], 2)) + ")", xy=(x[2], y[2]), fontsize=10, xycoords='data')# 或者
## plt.annotate("({0},{1})".format(round(x[2], 2), round(y[2], 2)), xy=(x[2], y[2]), fontsize=10, xycoords='data')
# xycoords='data' 以data值为基准
# 设置字体大小为 10

# 12. 在图中某些位置显示文本：text
# plt.text(round(x[6], 2), round(y[6], 2), "good point", fontdict={'size': 10, 'color': 'red'})  # fontdict设置文本字体
# Add text to the axes.

# 13. 设置显示中文
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 14. 设置legend，【注意，'绘图测试'：一定要是可迭代格式，例如元组或者列表，要不然只会显示第一个字符，也就是legend会显示不全】
plt.legend(['小黄的分数'], loc=2, fontsize=10)
# plt.legend(['绘图测试'], loc='upper left', markerscale = 0.5, fontsize = 10) #这个也可
# markerscale：The relative size of legend markers compared with the originally drawn ones.

# 15. 保存图片 savefig
plt.savefig('Scatter.png', dpi=300, bbox_inches='tight', transparent=False)
# dpi: The resolution in dots per inch，设置分辨率，用于改变清晰度
# If *True*, the axes patches will all be transparent

# 16. 显示图片 show
plt.show()
