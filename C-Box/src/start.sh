#!/bin/bash

MSR_TOOLS_PATH=/path
SRC_PATH=/path
# SRC_PATH is the path to the src folder

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

# Clear the counter values
sudo ./wrmsr $MSR_UNC_CBO_0_PERFCTR0 0x0
sudo ./wrmsr $MSR_UNC_CBO_1_PERFCTR0 0x0
sudo ./wrmsr $MSR_UNC_CBO_2_PERFCTR0 0x0
sudo ./wrmsr $MSR_UNC_CBO_3_PERFCTR0 0x0

# Select LLC_References event (0x34) in all slices
sudo ./wrmsr $MSR_UNC_CBO_0_PERFEVTSEL0 0x508f34
sudo ./wrmsr $MSR_UNC_CBO_1_PERFEVTSEL0 0x508f34
sudo ./wrmsr $MSR_UNC_CBO_2_PERFEVTSEL0 0x508f34
sudo ./wrmsr $MSR_UNC_CBO_3_PERFEVTSEL0 0x508f34

# Start counting
sudo ./wrmsr $MSR_UNC_PERF_GLOBAL_CTRL 0x2000000f
cd $SRC_PATH
