#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Need to specify physical address as argument"
	exit
fi

MSR_TOOLS_PATH=/path
SRC_PATH=/path
# SRC_PATH is the path to the src folder
OUTPUT_FILE=/path
# The output values will be appended to the OUTPUT_FILE

# Values for Sandy Bridge architecture
MSR_UNC_PERF_GLOBAL_CTRL="0x391"
MSR_UNC_CBO_0_PERFEVTSEL0="0x700"
MSR_UNC_CBO_1_PERFEVTSEL0="0x710"
MSR_UNC_CBO_2_PERFEVTSEL0="0x720"
MSR_UNC_CBO_3_PERFEVTSEL0="0x730"
MSR_UNC_CBO_0_PERFCTR0="0x706"
MSR_UNC_CBO_1_PERFCTR0="0x716"
MSR_UNC_CBO_2_PERFCTR0="0x726"
MSR_UNC_CBO_3_PERFCTR0="0x736"

cd $MSR_TOOLS_PATH
# Stop counting
sudo ./wrmsr $MSR_UNC_PERF_GLOBAL_CTRL 0x0

# Read counter values
slice0=`sudo ./rdmsr -u $MSR_UNC_CBO_0_PERFCTR0`
slice1=`sudo ./rdmsr -u $MSR_UNC_CBO_1_PERFCTR0`
slice2=`sudo ./rdmsr -u $MSR_UNC_CBO_2_PERFCTR0`
slice3=`sudo ./rdmsr -u $MSR_UNC_CBO_3_PERFCTR0`

# Add counter values to output files
echo $1","$slice0","$slice1","$slice2","$slice3 >> $OUTPUT_FILE

# Clear the event select MSRs
sudo ./wrmsr $MSR_UNC_CBO_0_PERFEVTSEL0 0x0
sudo ./wrmsr $MSR_UNC_CBO_1_PERFEVTSEL0 0x0
sudo ./wrmsr $MSR_UNC_CBO_2_PERFEVTSEL0 0x0
sudo ./wrmsr $MSR_UNC_CBO_3_PERFEVTSEL0 0x0

cd $SRC_PATH
