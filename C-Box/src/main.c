#include<emmintrin.h>
#include<sys/mman.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<inttypes.h>
#include<fcntl.h>

#define NUM_PAGES 6
#define NUM_FLUSHES 10000000
#define CACHE_LINE_SIZE 64

uint64_t va_to_pa(uint64_t addr){
	FILE* pagemap = fopen("/proc/self/pagemap", "r");
	if(pagemap == NULL){
		printf("Cannot open file /proc/self/pagemap\n");
		exit(0);
	}
	uint64_t page_size = sysconf(_SC_PAGE_SIZE);
	uint64_t PTE;
	uint64_t VPN = (uint64_t)addr / page_size;
	uint64_t ret, nread = 0;
	while (nread < sizeof(PTE)) {
		ret = pread(fileno(pagemap), &PTE, sizeof(PTE) - nread, VPN * sizeof(PTE) + nread);
		nread += ret;
		if (ret <= 0) {
			printf("Cannot read paddr\n");
			exit(0);
		}
	}
	if(!(PTE>>63 & 1)){
		printf("Present in RAM bit not set\n");
		exit(0);
	}
	return ((PTE & (((uint64_t)1<<55) - 1)) * page_size) + (addr % page_size);
}

int main(){
	int i, p, page_size, cl;
	page_size = sysconf(_SC_PAGE_SIZE);
	char* buffer = (char*) mmap (NULL, NUM_PAGES * page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE , -1, 0);
	char *temp, *stop_cmd;
	for(p=0; p < NUM_PAGES; p++){
		for(cl=0; cl<(page_size/CACHE_LINE_SIZE); cl++){
			temp = buffer + (p*page_size) + (cl*CACHE_LINE_SIZE);
			*(temp) = 'a';
			sprintf(stop_cmd, "bash stop.sh %lu", va_to_pa((uint64_t)temp));
			system("bash start.sh");
			for(i=0; i < NUM_FLUSHES; i++){
				_mm_clflush(temp);
			}
			system(stop_cmd);
		}
	}
}

