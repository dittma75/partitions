/* 
 * File:   least_part_m.c
 * Author: Kevin Dittmar
 *
 * Created on July 9, 2014
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "least_part_m.h"
#include "h_function_generator.h"

mpz_t cache[50][20000];
int is_loaded[50];

/*
 * Calculate h(m,n) and return the result in the out-parameter result
 * mpz_t result is an GMP integer out-parameter to store the number of
 * partitions with least part m for number n
 * int m is the least part of the partition
 * int n is the number to partition
 */
void h(mpz_t result, int m, int n)
{
    mpz_init(result);
    for (int i = 0; i < 50; i++)
    {
        //Initialize is_loaded array.
        is_loaded[i] = 0;
        //Initialize cache array
        for (int j = 0; j < 20000; j++)
        {
            mpz_init(cache[i][j]);
        }
    }    
    //Cases where there is only one partition.
    if ((n == m) ||                     //The smallest part is the only partition.
        (n >= (m * 2) && n < (m * 3)))  //1 partition for 2m <= n < 3m
    {
        mpz_set_ui(result, 1);
    }
    
    //3m == n yields exactly two unique partitions.
    else if ((m * 3) == n)   //The smallest part is one third of the partition.
    {
        mpz_set_ui(result, 2);
    }
    
    //There are no partitions possible with these rules.
    else if ((n < m) ||      //The smallest part can't be bigger than the number.
             (n <= 0) ||     //No negative numbers; 0 has no partitions.
             (m < 0) ||      //Smallest part < 0 makes no sense; 0 is unrestricted.
             ((2 * m) > n))  //Smallest part cannot be more than half of the number.
    {
        mpz_set_ui(result, 0);
    }
    
    //Unrestricted partition.
    else if (m == 0)
    {
        cacheLookup(result, n + 1);
    }
    //Base case partition.
    else if (m == 1)
    {
        cacheLookup(result, n);
    }
    //Sum of base case partitions after decomposition.
    else
    {
        hFunctionGenerator(m, n);
        //Can be decomposed into parts through the recurrence relation, or
        //is already in the form h(1,n).
        FILE *function_file;
        //Get the leaves of the function tree that was generated earlier.
        function_file = fopen("function_tree.txt", "r");
        if (function_file == NULL)
        {
            printf("Error:  Cannot find function_tree.txt");
            exit(1);
        }

        mpz_t value;
        mpz_init(value);
        int read_n;
        // k is the number of leaves of the h(m, n) tree that have been read.
        int k = 0;
        // Evaluate h(m, n) as the sum of all of the h(1, x(k)) leaves
        while (fscanf(function_file, "%d;", &read_n) > 0)
        {
            if (read_n != 0)
            {
                cacheLookup(value, read_n);
                if (e(k) == 1)
                {
                    mpz_add(result, result, value);
                }
                else
                {
                    mpz_sub(result, result, value);
                }
            }
            k++;
        }
    }
}

/* Get h(1, n) from our cache of the first million h(1, n) values
 * int n is the number to find h(1, n) for.
 * mpz_t result is the GMP integer out-parameter for storing the h(1, n)
 * found
 */
void cacheLookup(mpz_t result, int n)
{
    //The cache stores p(n) for 1 <= n <= 1,000,000, but array indices
    //start at 0.
    n -= 1;
    if (!is_loaded[n / 20000])
    {
        initializeSegment(n / 20000);
    }
    mpz_set(result, cache[n / 20000][n % 20000]);
}

/*
 * Load a needed segment of the cache of unrestricted partition numbers 
 * into memory.
 * int index is the number in the filename of the cache file to be loaded.
 */
void initializeSegment(int index)
{
    FILE *file;
    char* name = "cachedir/h_cache_";
    char* extension = ".txt";
    char filename[32];
    int i = 0;
    mpz_t value;
    mpz_init(value);
    sprintf(filename, "%s%d%s", name, index, extension);
    file = fopen(filename, "r");
    while (gmp_fscanf(file, "%Zd\n", value) > 0)
    {
        mpz_set(cache[index][i], value);
        i++;
    }
    fclose(file);
}

/* Corresponds to e(k) in the paper, which is the function that finds
 * the sign of the kth leaf in the tree for some h(m, n).
 */
int e(int k)
{
    if (k == 0)
    {
        return 1;
    }
    else if (k == 1)
    {
        return -1;
    }
    
    return -1 * e(k - pow(2, floor(log2(k))));
}

