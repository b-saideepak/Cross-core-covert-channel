#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <x86intrin.h>
#include <signal.h>

#include "util.h"

#define NUM_PAGES 16
#define DATA_LEN 8

struct sigaction sa;

FILE* outfile;

int data[DATA_LEN];
int cur_data;

void callback(int signo){
	sigaction(SIGALRM, &sa, NULL);
	printf("%d\n", data[cur_data]);
	fprintf(outfile, "%d\n", data[cur_data]);
	fflush(outfile);
	cur_data++;
	if(cur_data >= DATA_LEN){
		long int random_num = rand();
		for(int i=0;i<DATA_LEN;i++){
			data[i] = random_num % 2;
			random_num /= 2;
		}
		cur_data = 0;
	}
}

int main(int argc, char** argv){
	if (argc < 4){
		printf("Usage: %s set_no slice_no delay\n", argv[0]);
		exit(0);
	}

	srand(123);

	sa.sa_handler = callback;
	if(sigaction(SIGALRM, &sa, NULL) != 0){
		printf("Cannot set signal handler\n");
	}

	int set = atoi(argv[1]);
	int slice = atoi(argv[2]);
	long int delay = atoi(argv[3]);

	char filename[20];
	sprintf(filename, "results/%lu-send", delay);
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
	for(int j=0; j<NUM_PAGES; j++){
		temp = *(buffer + offsets[j]);
	}

	long int random_num = rand();
	for(int i=0;i<DATA_LEN;i++){
		data[i] = random_num % 2;
		random_num /= 2;
	}
	cur_data = 0;
	set_timer(seconds(delay), nanoseconds(delay), seconds(delay), nanoseconds(delay));
	while(1){
		if(data[cur_data]){
			for(int j=0; j<NUM_PAGES; j++){
				temp = *(buffer + offsets[j]);
			}
		}
	}
}
