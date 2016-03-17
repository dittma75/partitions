/* 
 * File:   parts_less_than_equal_to_m.c
 * Author: Kevin Dittmar
 *
 * Created on July 8, 2014, 8:52 AM
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Get number of partitions of n where all parts are less than m.
 * @param m maximum value for any part of the partition.
 * @param n number to be partitioned.
 * @return the number of partitions of n where
 * all parts are less than n.
 */
int p(int m, int n)
{
    int partitions = 0;
    if (n % m == 0)
    {
        partitions++;
    }
    else
    {
        partitions += p(m, n % m);
    }
    partitions += p(m - 1, n);
}
