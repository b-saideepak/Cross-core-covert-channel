#include<stdio.h>
#include<stdlib.h>

void main()
{
	FILE *fp;
	int B0[34] = {0},B1[34]={0},lines=0,temp,slice,t;/*The max no.of bits in the addresses are 28 + 6 bits are block offset. Hence array size 34*/
	char ch;
	fp = fopen("new_power.txt","r");
	/*This file contains a,b,c in each line: a-to which hash function this bit belongs(0,1,2-both):igonre b: c is the bit number*/
	while((ch=fgetc(fp))!=EOF)
	{
		if(ch=='\n')
			lines++;
	}

	fclose(fp);
	fp = fopen("new_power.txt","r");
	
	while(lines--)
	{
		fscanf(fp,"%d %d %d\n",&t,&temp,&slice);
		if(t==0)
			B0[slice + 6] = 1;
		else if(t==1)
			B1[slice + 6] = 1;
		else if(t==2){
			B0[slice + 6] = 1;
			B1[slice + 6] = 1;
		}

	}
	for(int i=33;i>-1;i--){
		printf("%d ",B0[i]);
	}
	printf("\n");
	for(int i=33;i>-1;i--){
	printf("%d ",B1[i]);
	}
	printf("\n");
}

