#include <stdlib.h>
#include <stdio.h>
#include <x86intrin.h>
#include <sys/mman.h>
#include <signal.h>

#define NUM_PAGES 1
#define THRESHOLD 110

#include "util.h"

struct sigaction sa;

FILE* outfile;

long int avg, times;
int started;
void callback(int signo){
	sigaction(SIGALRM, &sa, NULL);
	if(times==0){
		printf("Times is zero\n");
	}
	int tempavg = avg/times;
	avg = 0;
	times = 0;
	if(tempavg < THRESHOLD){
		//printf("0 (%d)\n",tempavg);
		if(started){
			printf("0\n");
			fprintf(outfile, "0\n");
		}
	} else {
		//printf("1 (%d)\n",tempavg);
		printf("1\n");
		fprintf(outfile, "1\n");
		if(!started){
			started = 1;
		}
	}
	fflush(outfile);
}

int main(int argc, char** argv){
	if (argc < 4){
		printf("Usage: %s set_no slice_no delay\n", argv[0]);
		exit(0);
	}

	sa.sa_handler = callback;
	if(sigaction(SIGALRM, &sa, NULL) != 0){
		printf("Cannot set signal handler\n");
	}

	int set = atoi(argv[1]);
	int slice = atoi(argv[2]);
	long int delay = atoi(argv[3]);

	char filename[20];
	sprintf(filename, "results/%lu-recv", delay);
	outfile = fopen(filename, "w");
	if(outfile == NULL){
		printf("Cannot open file\n");
		exit(0);
	}

	char* buffer = (char*) mmap (NULL, NUM_PAGES * HUGE_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB , -1, 0);
	if(buffer == (void *) -1){
		printf("Cannot allocate hugepage. Check /sys/kernel/mm/hugepage for number of hugepages available");
	}

	int* offsets = create_eviction_set(buffer, set, slice, NUM_PAGES);

	int temp;
	int start, end;

	for(int j=0; j<NUM_PAGES; j++){
		temp = *(buffer + offsets[j]);
	}

	avg = 0;
	set_timer(seconds(delay), nanoseconds(delay), seconds(delay), nanoseconds(delay));
	while(1){
		_mm_mfence();
		start = __rdtscp(&temp);
		for(int j=0; j<NUM_PAGES; j++){
			temp = *(buffer + offsets[j]);
		}
		_mm_lfence();
		end = __rdtscp(&temp);
		avg += end-start;
		times++;
	}
}
