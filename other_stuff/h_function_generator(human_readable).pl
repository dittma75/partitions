#!usr/bin/perl
#Used for SMALL inputs only.
use strict;
use warnings;

my @working_queue;      #the queue of function calls that aren't h(1,#)
my @pos_list;           #the list of function calls that are h(1,#)
my @neg_list;           #the list of function calls that are -h(1,#)
my @final_list;         #the final list to be saved to a text file.

if ($#ARGV != 1)
{
    print ("Usage:  h_function_generator.pl m n");
    exit(1);
}

if ($ARGV[0] == 0)
{
    push(@final_list, "+".$ARGV[1]);
}
else
{
    #Holds the current function call being examined.
    my $current_call;

    #Push the initial function call onto the working queue.
    push(@working_queue, "+,".$ARGV[0].",".$ARGV[1]);

    #Keep going until the working stack is empty.
    while ($#working_queue != -1)
    {
        #Pop the queue.
        $current_call = shift(@working_queue);
        my @string = split(/,/, $current_call);
        my $sign = $string[0];
        my $m = $string[1];
        my $n = $string[2];
        #This is not the function call we're looking for, so decompose it.
        if ($m != 1)
        {
            #Decomposes into h(m-1,n-1) - h(m-1, n-m).
            push(@working_queue, $sign.",".($m-1).",".($n-1));
            #Sign must be flipped.
            if ($sign eq "+")
            {
                push(@working_queue, "-,".($m-1).",".($n-$m));
            }
            else
            {
                push(@working_queue, "+,".($m-1).",".($n-$m));
            }
        }
        #This is the function call we're looking for, so send it to the
        #result list.  Filter out the bad function calls that evaluate to 0.
        elsif ($m <= $n)
        {
            if ($sign eq "+")
            {
                push(@pos_list, "+".$n);
            }
            else
            {
                push(@neg_list, "-".$n);
            }
        }
    }
    #Reference info from:
    #http://perldoc.perl.org/perlreftut.html#Making-References
    my $long_list = \@pos_list;
    my $short_list = \@neg_list;
    #if ($#pos_list < $#neg_list)
    #{
    #   $short_list = \@pos_list;
    #   $long_list = \@neg_list
    #}
    #for (my $i = 0; $i <= $#{$short_list}; $i++)
    #{
    #   if ($i == 0 or ${$short_list}[$i] ne ${$short_list}[$i - 1])
    #   {
    #       INNER:{
    #           for (my $j = 0; $j <= $#{$long_list}; $j++)
    #           {
    #               if (substr(${$short_list}[$i]." ", 1, -1) eq 
    #                   substr(${$long_list}[$j]." ", 1, -1,))
    #               {
    #                   splice(@{$short_list}, $i, 1);
    #                   splice(@{$long_list}, $j, 1);
    #                   #We just reduced the array length by 1, so i shouldn't
    #                   #change from this loop iteration to the next.
    #                   $i -= 1;
    #                   last INNER; #Go back to the start of the INNER loop.
    #               }
    #           }
    #       }
    #   }
    #}
    @final_list = (@{$short_list}, @{$long_list});
}
open (FILE, '>function_results.txt');
   print FILE join(";", @final_list).";";
close (FILE);
