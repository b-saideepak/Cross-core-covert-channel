#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<signal.h>
#include<time.h>

#include "util.h"

#define PAS 34
#define PAGEMAP_LENGTH 8

uint64_t get_ppn(uint64_t add) {
   // Open the pagemap file for the current process
   FILE *pagemap = fopen("/proc/self/pagemap", "rb");

   // Seek to the page that the buffer is on
   unsigned long offset = add / getpagesize() * PAGEMAP_LENGTH;
   if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0) {
      fprintf(stderr, "Failed to seek pagemap to proper location\n");
      exit(1);
   }

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
   unsigned long page_frame_number = 0;
   fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);

   return page_frame_number;
}
void getbinary(unsigned long int addr,int* binary)
{
	int index=0;
	while(addr)
	{
		binary[index++] = addr%2;
		addr/=2;
	}
}
void gethash(int* Bit0,int* Bit1)
{
	FILE *fp;
	char ch;
	int bit,id1 = PAS-1,id2 = PAS-1;
	fp = fopen("hash_function","r");
	while((ch=fgetc(fp))!='\n'&&id1>-1)
	{
		fscanf(fp,"%d",&bit);
		Bit0[id1--] = bit;
	}
	while((ch=fgetc(fp))!=EOF&&id2>-1)
        {
                fscanf(fp,"%d",&bit);
                Bit1[id2--] = bit;
        }
	fclose(fp);

}
int getslice(int* bin, int* hash)
{
	int slice=0;
	for(int i=PAS-1;i>-1;i--)
	{
		if(hash[i]==1)
		slice = (slice^bin[i]);	
	}
	return slice;
}

int getset(unsigned long int addr)
{
	FILE *set = fopen("/sys/devices/system/cpu/cpu0/cache/index3/number_of_sets","r");
	unsigned long int sets,setno;
	fscanf(set,"%lu",&sets);
	printf("sets %lu\n",sets);
	sets-=1;
	printf("%p %p\n",(void*)addr,(void*)sets);
	addr>>=6;
	setno = sets&addr;
	return setno;
}

int get_slice(uint64_t paddr){
	int Bit0[PAS]={0}, Bit1[PAS]={0}, bin[PAS] = {0};
	getbinary(paddr, bin);
	gethash(Bit0, Bit1);
	return 2 * getslice(bin, Bit1) + getslice(bin, Bit0);
}

int get_set(uint64_t paddr){
	paddr = paddr >> LLC_BLOCK_OFFSET_BITS;
	return paddr & ((1<<LLC_SET_INDEX_BITS) - 1);
}

int* create_eviction_set(char* buffer, int set, int slice, int size){
	int* offsets = (int*) malloc (sizeof(int)*size);
	int current_page_start = 0, current;
	for(int page = 0; page < size; page++){
		*(buffer + current_page_start) = 'a';
		uint64_t paddr = get_ppn((uint64_t)(buffer + current_page_start));
		paddr = paddr << 12;

		current = set << LLC_BLOCK_OFFSET_BITS;
		for(int i = 0; i < 4; i++){
			if(slice == get_slice(paddr + current)){
				offsets[page] = current_page_start + current;
			}
			current += (1<<(LLC_SET_INDEX_BITS + LLC_BLOCK_OFFSET_BITS));
		}
		current_page_start += HUGE_PAGE_SIZE;
	}
	return offsets;
}

void set_timer(int sstart, int nstart, int sinterval, int ninterval){
	timer_t timer;
	struct sigevent sigev;
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGALRM;
	if(timer_create(CLOCK_REALTIME, &sigev, &timer)!=0){
		printf("Cannot create timer\n");
		exit(0);
	}
	struct itimerspec tspec;
	tspec.it_interval.tv_sec = sstart;
	tspec.it_interval.tv_nsec = nstart;
	tspec.it_value.tv_sec = sinterval;
	tspec.it_value.tv_nsec = ninterval;
	if(timer_settime(timer, 0, &tspec, NULL)!=0){
		printf("Cannot set timer\n");
		exit(0);
	}
}

int seconds(int microseconds){
	return microseconds / 1000000;
}

long int nanoseconds(int microseconds){
	return (microseconds % 1000000) * 1000;
}
