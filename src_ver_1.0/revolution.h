/**
 * @file revolution.h
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief Implemention of the revolutional algorithm.
 * @version 0.1(uncompleted)
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef REVOLUTION_H
#define REVOLUTION_H

#include "object.h"
#include "grid.h"

int calc_score(obj *const, grid_t *const, size_t);
void __swap__(size_t *, size_t *);
void sort(const size_t, size_t *, const double *);
double revolution(size_t, obj *);

/**
 * @brief Get the stragegy object
 *
 * @param x
 * @param y 2 coordinates.
 * @param o
 * @param g
 * @return move_t strategy.
 *
 * 按照东西南北中编解码并取策略。
 * 空0 罐1 墙2
 */
#define get_stragegy(x, y, o, g) \
    o->strategy[(size_t)(x == 9 ? 2 : g->grid[x + 1][y]) + (x ? g->grid[x - 1][y] : 2) * 3 + (y == 9 ? 2 : g->grid[x][y + 1]) * 9 + (y ? g->grid[x][y - 1] : 2) * 27 + g->grid[x][y] * 81]

const size_t STEP_NUM = 200, GRID_NUM = 100, COPU_NUM = 95, RESERVED = 10;
/**
 * @brief 计算 %obj在 %grid上获得的分数。
 *
 * @param o Ptr to the obj.
 * @param g Ptr to thr grid.
 * @return int (可能为负)
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
inline void __swap__(size_t *lhs, size_t *rhs)
{
    size_t t = *lhs;
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
void sort(const size_t len, size_t arr[len], const double cmp[len])
{
    if (len <= 1)
        return;
    const size_t pivot = arr[random() % len];
    size_t i = 0, j = 0, k = len;
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
inline double revolution(size_t num, obj o[num])
{
    //生存斗争 - struggle
    grid_t g; //构造和析构成本太大，放在循环外面
    double score[num], avr = 0;
    for (size_t i = 0; i < num; ++i)
    {
        score[i] = 0;
        for (int j = 0; j++ < GRID_NUM;)
            score[i] += calc_score(o + i, &g, refresh_grid(&g));
        avr += score[i] /= GRID_NUM;
    }
    size_t rank[num];
    for (size_t i = 0; i < num; ++i)
        rank[i] = i;
    sort(num, rank, score);

    //自然选择 - existence
    obj child[num]; // use VLA instead of malloc, faster and safer.
    for (size_t i = 0; i < COPU_NUM; ++i)
        copulation(o + ridx, o + ridx, child + i * 2, child + (i * 2 | 1));
    memcpy(o, child, COPU_NUM * 2 * sizeof(obj)); //最好的10个自动保留
    return avr / num;
}
#undef ridx

#endif // REVOLUTION_H