#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

uint64_t b0 = 0b0101011111010101110101010001;
uint64_t b1 = 0b0010110101111110101010100010;

/* Program to verify the calculated hash function */
int main(int argc, char* argv[]){
	if (argc<2) {
		printf("Usage: %s cacheline_address_in_decimal\n", argv[0]);
		exit(0);
	}

	uint64_t address = atoi(argv[1]);

	int val0 = 0;
	int val1 = 0;

	uint64_t tempaddr, tempb;

	//Computing bit 0
	tempaddr = address;
	tempb = b0;
	while(tempb>0){
		if(tempb & 1){
			val0 ^= (tempaddr & 1);
		}
		tempb = tempb >> 1;
		tempaddr = tempaddr >> 1;
	}

	//Computing bit 1
	tempaddr = address;
	tempb = b1;
	while(tempb>0){
		if(tempb&1){
			val1 ^= (tempaddr & 1);
		}
		tempb = tempb >> 1;
		tempaddr = tempaddr >> 1;
	}

	printf("%d\n", (val1<<1 | val0));
}
