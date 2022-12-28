# SimpleAI 暨 2022秋程序设计Project2

**小黄和它的罐子**——遗传算法、GUI、强化学习

历史原因，src下文件都注©，但`GUI/src.h`采用`CC BY-NC-ND 4.0`。

本markdown文件介绍心路历程会很多，  
因为具体做法和各处的思路（详细的实现细节和原理解释）等等  
通通有各自对应的文档，或以`Doxygen注释`的形式附在源码中。  

本文采用`CC BY-NC-ND 4.0`。

---

## 遗传算法——物竞天择，适者生存

使用C99以及更新的语言标准编写。


### 总范式

- 尝试用C语言做到面向对象乃至泛型
- 尽量多地使用C99新加入的语言规范当中好用的部分-
- 尽可能提高程序运行效率  
  保持参与OI和ACM习得的良好习惯

### 总思路

自下向上编写，因为大体思路早已清晰；  
这种情况下自下而上利于稳扎稳打。

其实这相当于于先写一个个class，封装好成员对象，设计高可读性高复用性的接口（即`grid.h`和`object.h`），  
然后把它们组合。这个过程会抽象掉很多东西，不必事无巨细地思考到底层。

类似于数学家们解题使用的传统方法：综合法。

**大体经过** 详见src文件夹下的`README.md`。

### 特别关注

随机函数的质量很大程度影响最终结果！

以下是用标准随机函数（214013L 和 2531011L）小改后的随机函数，  
分别代替srand()和rand()。
```c
unsigned long long holdrand = 1ull;
void __cdecl set_random(unsigned long long _Seed)
{
    holdrand = _Seed;
}
/**
 * @brief Generate an unsigned random integar.
 *
 * @return ** unsigned
 *
 * 尽量做得原汁原味，所以不写成内联函数，甚至用__cdecl修饰。
 */
size_t __cdecl random(void)
{
    return (((holdrand = holdrand * 214013ull + 2531011ull) >> 32) & 0x7fffffff);
}
```

---

## GUI

### 尝试使用SDL2

由于文档的匮乏，以及它对MinGW系编译器支持不佳，  
在学习了数个晚上终于向屏幕显示出了一张hello world后……

最终放弃。

### 原打算使用Javax.swing

在`Java_src/test_GUI`文件夹下可以看到一些GUI测试代码，  
但是其学习成本依然比较高：

一些简单的过程仍需要设计独立的类来完成，  
稍微复杂一点点的行为  
即出现继承、多态、虚方法、实现接口等等繁多的OOP设计

这迫使我回归面向过程，或者说，转而有节制地面向对象。

### 转向EasyX

多年前在网上发现了`EasyX`，终于有用武之地了！

其简单使用模式以及用它构筑面向对象式GUI的方法已经总结起来写了一篇Pre，  
**详见Pre**就好了（是长篇大论，**放在Pre文件夹下**）~  
源码是markdown，推荐阅读PDF或者pptx

---

## 强化学习

### 人工神经网络

参阅了PJ文档给的“人工神经网络”维基百科后……嗯，毫无收获呢~  

于是自己搜寻相关资料并且构建了一个ANN，放在`typical_ANN.py`里。

高高兴兴拿着ANN问大家，问了一圈被说了一圈……

听到的不外乎“zxr写过，问她”（戴老师语）  
“这不应该跑有监督的算法吧”（tjw同学语）  
“这边建议还是无监督学习”（讨论版上得到zxr姐姐的亲口答复）

于是搜寻资料，做

### 强化学习

强势回复！“已经在Anaconda里建好文件夹装OpenAI了[玫瑰]”

#### 配置环境

对，我们用`gym库`，这样可以直接调用它写好了的**马尔科夫决策过程**。

在此之前需要类似的代码（

```ps1
conda create --name gymlab python=3.10
activate gymlab
git clone https://github.com/openai/gym.git
cd gym
pip install -e '.[all]'
export PYTHONPATH=你的gym⽬录：$PYTHONPATH
```
完成之后每次`activate gymlab`  
然后启动python，调用gym.xx()就好了

#### 编写代码

自己实现一个Env类，提供`__init__()`、`_step()`、`_reset()`、`render()`等方法  
注意`__init__()`中给出状态空间、动作集合、回报函数、转移函数等列表或字典，    
然后调用gym.make()即可生成这样的训练对象。

---

## 额外说明

高分策略是使用Java_src生成的。
