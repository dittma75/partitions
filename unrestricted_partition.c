/* 
 * File:   unrestricted_partition.c
 * Author: Kevin Dittmar
 *
 * Created on July 8, 2014
 */
#include <stdio.h>
#include <gmp.h>
#include "unrestricted_partition.h"
#include "least_part_m.h"

const int CHUNK_SIZE = 20000;
const int TOTAL_SIZE = 1000000;
mpz_t p_array[1000000];
/*Generate the p(n) cache array from 0-999999.  It just so happens that this
 *corresponds to h(1,n) from 1-1000000.
 */
void generate()
{
    mpz_init(p_array[0]);
    FILE *file;
    for (int i = 0; i < TOTAL_SIZE / CHUNK_SIZE; i++)
    {
        char* name = "cachedir/h_cache_";
        char* extension = ".txt";
        char filename[32];
        sprintf(filename, "%s%d%s", name, i, extension);
        file = fopen(filename, "w");
        if (file != NULL)
        {
            for (int n = (CHUNK_SIZE * i); n < (CHUNK_SIZE * (i + 1)); n++)
            {   
                mpz_init(p_array[n]);
                if (n == 0)
                {
                    mpz_set_ui(p_array[0], 1);
                }
                else
                {
                    unrestricted_partition(p_array[n], n);
                }
                gmp_fprintf(file, "%Zd\n", p_array[n]);
            }
        }
        fclose(file);
    }
}

/*
 * Get the kth pentagonal number.
 * int k is the k we are referring to when we say "kth pentagonal number"
 * return the kth pentagonal number.
 */
int pentagonal(int k)
{
    return k * ((3 * k) - 1) / 2;
}

/*
 * mpz_t out is the GMP integer number of unrestricted partitions for n
 * int n is the number to partition
 * return the number of unrestricted partitions of n.
 */
void unrestricted_partition(mpz_t out, int n)
{
    int sign_counter = 0;
    int k = 0;
    int pent = 0;
    while (n - pent > 0)
    {
        k++;
        //Handle the case of positive k.
        pent = pentagonal(k);
        if (n - pent >= 0)
        {
            if (isPositive(sign_counter))
            {
                mpz_add(p_array[n], p_array[n], p_array[n - pent]);
            }
            else
            {
                mpz_sub(p_array[n], p_array[n], p_array[n - pent]);
            }
        }
        //If necessary, handle the case of negative k.
        if (n - pent > 0)
        {
            pent = pentagonal(-1 * k);
            if (n - pent >= 0)
            {
                if (isPositive(sign_counter))
                {
                    mpz_add(p_array[n], p_array[n], p_array[n - pent]);
                }
                else
                {
                    mpz_sub(p_array[n], p_array[n], p_array[n - pent]);
                }
            }
        }
        sign_counter++;
    }
    mpz_set(out, p_array[n]);
}

/* Based on the recurrence relation found by Drs. Hassen and Osler, the
 * following rules apply to the sign of the term.
 * If (counter / 2) is even, the sign is positive.
 * If (counter / 2) is odd, the sign is negative.
 * Hence, if (counter / 2) % 2 is 1 (true), the sign is negative.
 * else, if (counter / 2) % 2 is 0 (false), which is the only other case, the
 * sign is positive.
 * However, since the counter is incremented for every two pentagonal numbers
 * generated, the division by 2 is not necessary.
 * 
 * int counter is the number of the pentagonal number whose sign is to be
 * determined
 */
int isPositive(int counter)
{
    if (counter % 2)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}