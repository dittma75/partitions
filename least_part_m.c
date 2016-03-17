/* 
 * File:   least_part_m.c
 * Author: Kevin Dittmar
 *
 * Created on July 9, 2014, 8:23 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "least_part_m.h"

mpz_t cache[50][20000];
int is_loaded[50];
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
        cache_lookup(result, n + 1);
    }
    //Base case partition.
    else if (m == 1)
    {
        cache_lookup(result, n);
    }
    //Sum of base case partitions after decomposition.
    else
    {
        //Can be decomposed into parts through the recurrence relation, or
        //is already in the form h(1,n).
        FILE *function_file;
        FILE *sign_file;
        //Get the function tree that was generated earlier.
        function_file = fopen("function_tree.txt", "r");
        if (function_file == NULL)
        {
            printf("Error:  Cannot find function_tree.txt");
            exit(1);
        }
        //Get the sign tree that was generated earlier.
        sign_file = fopen("function_signs.txt", "r");
        if (sign_file == NULL)
        {
            printf("Error:  Cannot find function_signs.txt");
            exit(1);
        }

        mpz_t value;
        mpz_init(value);
        char sign;
        int read_n;
        //Pair off until we run out of numbers - there will always be plenty of signs.
        while (fscanf(function_file, "%d;", &read_n) > 0 &&
               fscanf(sign_file, "%c", &sign) > 0)
        {
            if (read_n != 0)
            {
                cache_lookup(value, read_n);
                if (sign == '+')
                {
                    mpz_add(result, result, value);
                }
                else
                {
                    mpz_sub(result, result, value);
                }
            }
        }
    }
}

//Check if cache segment is initialized and return result.
void cache_lookup(mpz_t result, int n)
{
    //The cache stores the numbers from 1 to 1,000,000, but indices
    //start at 0.
    n -= 1;
    if (!is_loaded[n / 20000])
    {
        initialize_segment(n / 20000);
    }
    mpz_set(result, cache[n / 20000][n % 20000]);
}

//Load a needed segment of the cache into memory.
void initialize_segment(int index)
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