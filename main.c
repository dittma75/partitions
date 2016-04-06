/* 
 * File:   main.c
 * Author: Kevin Dittmar
 *
 * Created on July 8, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "least_part_m.h"
#include "unrestricted_partition.h"
mpz_t cache[1000000];
int main(int argc, char** argv) 
{
    if (argc != 3)
    {
        printf("Usage: partitions.exe m n");
        exit(EXIT_FAILURE);
    }
    printf("m= %s; n = %s\n", argv[1], argv[2]);
    mpz_t result;
    mpz_init(result);
    generate();
    h(result, atoi(argv[1]) , atoi(argv[2]));
    gmp_printf("%Zd\n", result);
    return (EXIT_SUCCESS);
}

