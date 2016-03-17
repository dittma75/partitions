#!usr/bin/perl
use strict;
use warnings;

my @working_queue;      #the queue of function calls that aren't h(1,#)
my @pos_list;           #the list of function calls that are h(1,#)
my @neg_list;           #the list of function calls that are -h(1,#)
my @final_list;         #the final list to be saved to a text file.
my $counter = 0;
if ($#ARGV != 1)
{
    print ("Usage:  h_function_generator.pl m n");
    exit(1);
}
#All we have to do is print the first sign.
if ($ARGV[0] == 0)
{
    open (FILE, '>function_signs.txt');
    print FILE "+"."\n";
    close (FILE);
}
else
{
    #Make sure the file is empty.
    open (FILE, '>function_signs.txt');
    print FILE "";
    close (FILE);
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
        #This is the function call we're looking for, so send its sign to the
        #result text file.
        else
        {
            open (FILE, '>>function_signs.txt');
            print FILE $sign.";";
            close (FILE);
        }
    }
}
