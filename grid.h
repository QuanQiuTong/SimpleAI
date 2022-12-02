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
#ifndef GRID_H
#define GRID_H

#include <stdbool.h>

#include "library.h"

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
#endif // GRID_H