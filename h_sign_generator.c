#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "h_sign_generator.h"

void h_sign_generator(int m)
{
    FILE *write;
    FILE *read;
    //correlates to X replace in report
    char plus_replace[17] = "+--+-++--++-+--+";
    //correlates to O replace in report
    char minus_replace[17] = "-++-+--++--+-++-";
    
    double bound = ceil((m - 2) / 4);
    write = fopen("function_signs.txt", "w");
    fprintf(write, "+");
    fclose(write);
    for (int i = 0; i <= bound; i++)
    {
        char next_sign;
        read = fopen("function_signs.txt", "r");
        write = fopen("sign_temp.txt", "w");
        while (fscanf(read, "%c", &next_sign) > 0)
        {
            if (next_sign == '+')
            {
                fprintf(write, "%s", plus_replace);
            }
            else if (next_sign == '-')
            {
                fprintf(write, "%s", minus_replace);
            }
        }
        fclose(read);
        fclose(write);
        system("mv sign_temp.txt function_signs.txt");
    }
}
