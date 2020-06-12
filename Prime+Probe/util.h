#include <inttypes.h>

#define LLC_SET_INDEX_BITS 13
#define LLC_SETS 8192
#define LLC_ASSOCIATIVITY 12
#define LLC_BLOCK_OFFSET_BITS 6
#define HUGE_PAGE_SIZE 2097152

uint64_t get_ppn(uint64_t vaddr);
int get_slice(uint64_t paddr);
int get_set(uint64_t paddr);
int* create_eviction_set(char*, int, int, int);
void set_timer(int, int, int, int);
int seconds(int);
long int nanoseconds(int);
