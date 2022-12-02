/**
 * @file main.c
 * @author 傅全通 (fu.quantong@foxmail.com)
 * @brief Looply get the %obj revoluted, and show this progress in the terminal.
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma GCC optimize(3)
//#define DEBUG_COPULATION
//#define NDEBUG
#include "revolution.h"

int main(int argc, char *argv[])
{
    obj yellow_chan[200];
    random_generate(200, yellow_chan);

    for (size_t i = 0; i < 2000; ++i)
    {
        printf("Generation %d\'s average is %.3lf.\n",
               i, revolution(200, yellow_chan));
    }
#if 1
    FILE *fp = openfile(write_strategy);
    fputs("# Use \'#\' to write a line of comment.\n",fp);
    fprintf(fp, "# Generation %d\n# number %d\n", 1999, 198);
    for (size_t k = 0; k < STRATEGY_LEN; ++k)
        fprintf(fp, "%d\t", yellow_chan[198].strategy[k]);
    fputc('\n', fp);
    fclose(fp);
#endif
    return 0;
}