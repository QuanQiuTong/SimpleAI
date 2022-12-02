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
#ifndef OBJECT_H
#define OBJECT_H

#include <string.h>

#include "library.h"

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
inline void random_generate(const size_t num, obj x[num])
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
    memcpy(son->strategy, fa->strategy, 60);
    memcpy(dau->strategy, mo->strategy, 60);
    memcpy(dau->strategy + 60, fa->strategy + 60, 60);
    memcpy(son->strategy + 60, mo->strategy + 60, 60);
    memcpy(son->strategy + 120, fa->strategy + 120, 60);
    memcpy(dau->strategy + 120, mo->strategy + 120, 60);
    memcpy(dau->strategy + 180, fa->strategy + 180, 63);
    memcpy(son->strategy + 180, mo->strategy + 180, 63);

    //变异 - mutation
    static const unsigned MU = 600;
    for (size_t i = 0; i < 243; ++i)
    {
        if (random() % MU == 0)
            son->strategy[i] = random() % (MOVE_MAX + 1);
        if (random() % MU == 0)
            dau->strategy[i] = random() % (MOVE_MAX + 1);
    }
    #ifdef DEBUG_COPULATION
    FILE *fp = fopen("copulation", "w+");
    fprintf(fp, "fa:\n");
    for (size_t k = 0; k < STRATEGY_LEN; ++k)
        fprintf(fp, "%d ", fa->strategy[k]);
    fputc('\n', fp);
    fprintf(fp, "mo:\n");
    for (size_t k = 0; k < STRATEGY_LEN; ++k)
        fprintf(fp, "%d ", mo->strategy[k]);
    fputc('\n', fp);
    fprintf(fp, "son:\n");
    for (size_t k = 0; k < STRATEGY_LEN; ++k)
        fprintf(fp, "%d ", son->strategy[k]);
    fputc('\n', fp);
    fprintf(fp, "dau:\n");
    for (size_t k = 0; k < STRATEGY_LEN; ++k)
        fprintf(fp, "%d ", dau->strategy[k]);
    fputc('\n', fp);
    fclose(fp);
    #endif
}

#endif // OBJECT_H