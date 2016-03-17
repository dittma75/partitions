/* 
 * File:   distinct.c
 * Author: Kevin Dittmar
 *
 * Created on July 8, 2014, 9:26 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "distinct.h"
/**
 * Get number of partitions of n where all parts are distinct.
 * @param n number to partition
 * @return number of partitions made up of distinct parts.
 */

int q(int n)
{
    return (1 + next_q(n, 0, n - 1));
    //printf("The number of partitions is %d.", 1 + next_q(n, 0, n - 1));
}

int next_q(int n, int amount, int next)
{
    //This partition is valid.
    if (amount == n)
    {
        return 1;
    }
    //The sum of the parts is too large.
    if (amount > n)
    {
        return 0;
    }
    //We're out of numbers to use.
    if (next < 1)
    {
        return 0;
    }
    //We're not finished yet.
    //Is it possible to finish the partition with the numbers left?
    if (gauss(next) >= (n - amount))
    {
        return next_q(n, amount + next, next - 1) +
               next_q(n, amount, next - 1);
    }
    //There is no way to make a valid partition from the rest.
    return 0;
}

float gauss(int n)
{
    return (n * (n + 1) / 2);
}