The contents of this directory are all that is necessary to
run the Least Part Partitions project.

cachedir contains the cache of unrestricted partition counts
that the program uses to speed up computation.  This directory
will start empty, and the program will generate the cache on its
first run.  On a relatively modern computer (quad core i7 with 12 GB of RAM),
it takes about 3 minutes to generate the cache.  It will take longer on older
systems.

function_tree.txt is a helper file used by the program to keep track of
the leaves of the h(m,n) decomposition tree.  The current file is a sample.
The program will generate this file with the correct value at runtime.

partitions.exe is the executable of the program that calculates h(m,n) for
given values.

Usage:  partitions.exe m n

where m is the least part to use for the partition and n is the number to
partition.
It is highly suggested that no m larger than 15 is used.  For
non-trivial computations, this calculation takes about 30 minutes on the
previously mentioned machine, and every increase to m doubles the amount
of time taken.

Do not remove any of the DLLs.  They are necessary for the program to run.
