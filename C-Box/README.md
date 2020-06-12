# Reversing LLC

The objective of this project is to reverse enginneer the hash function used for selecting the slice in the LLC of Intel CPUs.

Clone the [Intel msr-tools](https://github.com/intel/msr-tools) and compile it.

Configure the Path variables and MSR values for your architecture in start.sh and stop.sh files. Refer Intel Software Developer Manual Volume 4 for details on MSR numbers.

The scripts are for a 4 slice processor. For other slice numbers the start.sh and stop.sh files have to be modified.

Make sure the MSR module is loaded by executing
```sh
sudo modprobe msr
```

Compile and run the executable with sudo
```sh
cd src
gcc main.c -o reverse
sudo ./reverse
```

This generates a csv file with the following format:

physicalAddress,slice0CounterValue,slice1CounterValue,slice2CounterValue,slice3CounterValue

The convert.sh script converts the output file from the previous format to the following format:

physicalCacheLineAddress,sliceNumber

Note: The last 6 bits (Cache line offset are omitted) and the sliceNumber is calculated based on which slice has the highest number of LLC references

