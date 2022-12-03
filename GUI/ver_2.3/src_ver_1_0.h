/**
 * @file    src_ver_1_0.h
 * @author  QuanQiuTong (https://github.com/QuanQiuTong)
 * @brief   Integrate all the headers in "version-1.0 source code".
 * @version 1.0
 * @date    2022-12-04
 *
 * @copyright Copyright (c) 2022
 *
 */


#pragma once

/**
 * @file library.h
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief Universal and useful functions in the project.
 * @version 1.1
 * @date 2022-11-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

void __cdecl set_random(unsigned long long);
size_t __cdecl random(void);
size_t __cdecl random2(void);
size_t probability(size_t);
unsigned long long read(void);

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
    return (((holdrand = holdrand * 214013ull + 2531011ull) /*>> 32*/) & 0xaaaffffffffu);
}
/**
 * @brief 按照大概是三次增长的概率取得[0,x)的自然数。
 *
 * @param x 要取得的自然数的最大值（不包含）。
 * @return 本次取到的自然数。
 */
inline size_t probability(size_t x)
{
    return sqrt(sqrt(random() % (1ull *x * x * x * x)));
}
inline size_t probability2(size_t x)
{
    return sqrt(random() % (x * x));
}


/**
 * @file object.h
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief The main object (Yellow-chan) in the project.
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <string.h>

typedef unsigned char move_t;
#define MOVE_MAX 6u //要用在inline-function里面就不能static（别的文件调用时源码展开而导致不可见）
enum Movements
{
    up = 1,
    down = 2,
    left = 3,
    right = 4,
    _ = 5,
    pick = 6
};
#define STRATEGY_LEN 243u

typedef struct
{
    move_t strategy[STRATEGY_LEN];
} obj, *pOBJ;

void random_generate(const size_t, obj *const);
void copulation(obj *const, obj *const, obj *const, obj *const);

/**
 * @brief Generate the initial strategy.
 *
 * @param x  A pointer to the object which is to be initialized ("given birth").
 */
inline void random_generate(const size_t num, obj x[/*num*/])
{
    for (size_t i = 0; i < num; ++i)
    {
        move_t *const strategy = (x + i)->strategy;
        for (size_t j = 0; j < STRATEGY_LEN; ++j)
            strategy[j] = random() % (MOVE_MAX + 1);
    }
}

inline void copulation(obj *const fa, obj *const mo, obj *const son, obj *const dau)
{
    //遗传 - heredity
    memcpy_s(son->strategy, 60, fa->strategy, 60);
    memcpy_s(dau->strategy, 60, mo->strategy, 60);
    memcpy_s(dau->strategy + 60, 60, fa->strategy + 60, 60);
    memcpy_s(son->strategy + 60, 60, mo->strategy + 60, 60);
    memcpy_s(son->strategy + 120, 60, fa->strategy + 120, 60);
    memcpy_s(dau->strategy + 120, 60, mo->strategy + 120, 60);
    memcpy_s(dau->strategy + 180, 63, fa->strategy + 180, 63);
    memcpy_s(son->strategy + 180, 63, mo->strategy + 180, 63);

    //变异 - mutation
    extern const unsigned MU;
    for (size_t i = 0; i < STRATEGY_LEN; ++i)
    {
        if (random() % MU == 0)
            son->strategy[i] = random() % (MOVE_MAX + 1);
        if (random() % MU == 0)
            dau->strategy[i] = random() % (MOVE_MAX + 1);
    }
}


/**
 * @file grid.h
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief The struct-type grid, along with function(s) for them.
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdbool.h>

#define GRID_LEN 10
#define GRID_WID 10
typedef struct
{
    bool grid[GRID_LEN][GRID_WID];
} grid_t;

size_t refresh_grid(grid_t *);

/**
 * @brief Set the grid to a new state randomly.
 *
 * @param x  Pointer to the grid.
 * @return Pointer to the refreshed grid itself.
 */
inline size_t refresh_grid(grid_t *const x)
{
    bool *const grid = (bool *const)x;
    size_t cnt = 0;
    for (size_t i = 0; i < sizeof(grid_t); ++i)
        cnt += grid[i] = random() & 1;
    return cnt;
}


/**
 * @file revolution.h
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief Implemention of the revolutional algorithm.
 * @version 1.4(兼容C++)
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

int calc_score(obj *const, grid_t *const, size_t);
static void __swap__(unsigned short *, unsigned short *);
static void sort(const size_t, unsigned short *, const double *);
double revolution(size_t, obj *);

/**
 * @brief Get the stragegy object
 *
 * 按照“东西南北中，空0 罐1 墙2”的规则编解码并取策略。
 */
#define get_stragegy(x, y, o, g) \
    o->strategy[(size_t)(x == 9 ? 2 : g->grid[x + 1][y]) + (x ? g->grid[x - 1][y] : 2) * 3 + (y == 9 ? 2 : g->grid[x][y + 1]) * 9 + (y ? g->grid[x][y - 1] : 2) * 27 + g->grid[x][y] * 81]

const unsigned STEP_NUM = 200, GRID_NUM = 100, COPU_NUM = 95, RESERVED = 10;
/**
 * @brief 计算 %obj在 %grid上获得的分数。
 *
 * @param o Ptr to the obj.
 * @param g Ptr to thr grid.
 * @return int (可能为负)
 * 
 * 预留cnt参数，目前还没有用
 */
inline int calc_score(obj *const o, grid_t *const g, size_t cnt)
{
    int score = 0;
    for (size_t i = 0, x = 0, y = 0, cmd; i < STEP_NUM; ++i)
    {
        (cmd = get_stragegy(x, y, o, g)) || (cmd = random() % MOVE_MAX + 1);
        switch (cmd)
        {
        case up:
            y ? --y : (score -= 5);
            break;
        case down:
            y != 9 ? ++y : (score -= 5);
            break;
        case left:
            x ? --x : (score -= 5);
            break;
        case right:
            x != 9 ? ++x : (score -= 5);
            break;
        case pick:
            if (g->grid[x][y])
            {
                g->grid[x][y] = false;
                score += 10;
                /*if (!--cnt)
                    break ; // break的语义存疑。*/
            }
            else
                score -= 2;
            break;
        default:
            break;
        }
    }
    return score;
}
#undef get_stragegy

/// @brief C-version of swap.
static inline void __swap__(unsigned short *const lhs, unsigned short *const rhs)
{
    unsigned short t = *lhs;
    *lhs = *rhs;
    *rhs = t;
}
#define swap(a, b) __swap__(&(a), &(b)) //使得sort可读性更强
/**
 * @brief 三路快排
 *
 * @param len 数组长度
 * @param arr 待排序数组
 * @param cmp 比较的依据
 */
static void sort(const size_t len, unsigned short arr[/*len*/], const double cmp[/*len*/])
{
    if (len <= 1)
        return;
    const unsigned short pivot = arr[random() % len];
    unsigned short i = 0, j = 0, k = len;
    while (i < k)
    {
        if (cmp[arr[i]] < cmp[pivot])
            swap(arr[i++], arr[j++]);
        else if (cmp[pivot] < cmp[arr[i]])
            swap(arr[i], arr[--k]);
        else
            ++i;
    }
    sort(j, arr, cmp);
    sort(len - k, arr + k, cmp);
}
#undef swap

#define ridx (rank[probability(num)])
/**
 * @brief 物竞天择，适者生存。
 *
 * @param num
 * @param o
 * @return 这一代的均分。
 */
inline double revolution(size_t num, obj o[/*num*/])
{
    //生存斗争 - struggle
    grid_t g;                              //构造和析构成本太大，放在循环外面
#if __cplusplus                            //降低性能的。只是为了兼容。
    double *const score = new double[num]; // std::vector<double> score(num);
    unsigned short *const rank = new unsigned short[num];  // std::vector<size_t> rank(num);
    obj *const child = new obj[num];       // std::vector<obj> child(num);
#else
    double score[num];
    unsigned short rank[num];
    obj child[num]; // use VLA instead of malloc, faster and safer.
#endif
    double avr = 0;
    for (size_t i = 0; i < num; ++i)
    {
        score[i] = 0;
        for (int j = 0; j++ < GRID_NUM;)
            score[i] += calc_score(o + i, &g, refresh_grid(&g));
        avr += score[i] /= GRID_NUM;
    }

    for (size_t i = 0; i < num; ++i)
        rank[i] = i;
    sort(num, rank, score);

    //自然选择 - existence

    for (size_t i = 0; i < COPU_NUM; ++i)
        copulation(o + ridx, o + ridx, child + i * 2, child + (i * 2 | 1));
    memcpy(o, child, COPU_NUM * 2 * sizeof(obj)); //最好的10个自动保留
#if __cplusplus
    delete[] score;
    delete[] rank;
    delete[] child;
#endif
    return avr / num;
}
#undef ridx
