# Obtaining the hash function

Using the C-Box counters we obtain a file which has the line address to slice mapping. In this case it is the `4-may-slice-info` file.

Code here `xor.c` and `function.c` will try to obtain the hash function by exploiting the fact that two addresses which differ in only one bit if mapped to two different slices, the bit in which they differ should be present in the hash function.

