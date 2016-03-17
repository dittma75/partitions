/* 
 * File:   least_part_m.c
 * Author: Kevin Dittmar
 *
 * Created on July 9, 2014, 8:23 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "least_part_m.h"

int h(int m, int n)
{
    FILE *file;
    int** cache_data;

    //Creates a virtual 2D array.
    cache_data = (int**)malloc(m * sizeof(int*));
    for (int i = 0; i < m; i++) {
      cache_data[i] = (int*)malloc(n * sizeof(int));
    }
    
    //Try to open cache file.
    file = fopen("h_cache.txt", "r");
    //Cache file doesn't exist.
    if (file == NULL)
    {
        printf("h_cache.txt does not exist.  Creating on this run.");
    }
    
    //Initialize all values in cache to -1 sentinel value.
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cache_data[i][j] = -1;
        }
    }

    //Put cache values in array where necessary.
    if (file != NULL)
    {
        int m_index = 0;
        int n_index = 0;
        int value = 0;
        while (fscanf(file, "(%d,%d) %d\n", &m_index, &n_index, &value) > 0)
        {
            cache_data[m_index][n_index] = value;
        }
        fclose(file);
    }
    
    h_base(m, n, cache_data);
    //Write to the cache.
    file = fopen("h_cache.txt", "w");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(file, "(%d,%d) %d\n", i, j, cache_data[i][j]);
        }
    }
    fclose(file);
    for (int i = 0; i < m; i++) {
      free(cache_data[i]);
    }
    free(cache_data);
}

int h_base(int m, int n, int** cache)
{
    if (cache[m][n] != -1)
    {
        return cache[m][n];
    }
    
    if ((n == m) ||     //The smallest part is the only partition.
        (m * 2) == n)   //Two of the smallest part is the only partition.
    {
        cache[m][n] = 1;
        return 1;
    }
    if ((n < m) ||           //The smallest part can't be bigger than the number.
        (n <= 0) ||          //No negative numbers; 0 has no partitions.
        (m <= 0) ||          //Smallest part <= 0 makes no sense.
        ((2 * m) > n))       //Smallest part cannot be more than half of the number.
    {
        cache[m][n] = 0;
        return 0;
    }
    
    //Recurrence tries to reference m == 0.
    if (m == 1)
    {
        cache[m][n] = (1 + h_recursive(m, n, m, n - m - 1, cache));
        return cache[m][n];
    }
    
    cache[m][n] = h_base(m - 1, n - 1, cache) - h_base(m - 1, n - m, cache);
    return cache[m][n];
}

int h_recursive(int m, int n, int amount, int next, int** cache)
{
    //This subtree is already cached.
    if (cache[m][n] != -1)
    {
        return cache[m][n];
    }
    
    //This partition is valid.  Subtree complete.
    if (amount == n)
    {
        return 1;
    }
    
    //Subtree dead-end.
    if ((amount > n) ||     //The sum of the parts is too large.
        (next < m) ||       //Numbers left are too small.
        ((n - amount) < m)) //Amount left to make up is too small.
    {
        return 0;
    }
    
    //The current number is too big; go to the next largest number.
    if ((amount + next) > n)
    {
        return h_recursive(m, n, amount, next - 1, cache);
    }
    /*We can either include the current number and keep going with it or
     *start trying the next largest number.
     */
    else
    {
        return h_recursive(m, n, amount + next, next, cache) +
               h_recursive(m, n, amount, next - 1, cache);
    }
}