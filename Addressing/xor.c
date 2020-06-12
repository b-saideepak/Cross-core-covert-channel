#include<stdio.h>
#include<stdlib.h>

int power(int n)
{
	int flag=1,count=0;
	while(n)
	{
		if(n==1)
			break;
		else if(n%2==1){
			flag=-1;
			break;
		}
		n/=2;
		count++;
	}
	if(flag==1)
		return count;
	return flag;

}


void main()
{
	FILE *ip,*op,*up;
	int lines=0,i,j=0,temp,*data,slice,out,*sl;
	char ch;

	ip = fopen("4_may_slice_info","r");
	op = fopen("new_hash.txt","w");
	up = fopen("new_power.txt","w");

	while((ch=fgetc(ip))!=EOF){
	if(ch=='\n')
		lines++;
	}
	i=0;
	fclose(ip);

	ip = fopen("4_may_slice_info","r");
	data = (int*)malloc(sizeof(int)*lines);
	sl = (int*)malloc(sizeof(int)*lines);

	/*Taking the addresses into data array and slice no in slice array*/
	while(i<lines){
	fscanf(ip,"%d%c%d\n",&temp,&ch,&slice);
	data[i] = temp;
	sl[i] = slice;
	i++;
	}
	printf("Hello\n");	
	/*Comparing every two addresses and find those two addresses which differ in only one bit and the slice no.s are different*/
	i=0;
	while(i<lines)
	{
		j = i + 1;
		while(j<lines)
		{
			temp = data[i]^data[j];			
			out = power(temp);/*Function returns the bit no. if they differ in only one bit else -1 is returned*/
			if(out!=-1&&sl[i]!=sl[j]){
				if((sl[i]==0&&sl[j]==1)||(sl[i]==1&&sl[j]==0)){/*This means that bit involves in B0 hash function*/
				fprintf(op,"%d %d %d\n",data[i],data[i],out); 
				fprintf(up,"0 %d %d\n",temp,out);
				}
				if((sl[i]==0&&sl[j]==2)||(sl[i]==2&&sl[j]==0)){/*This means that bit involves in B1 hash function*/
				fprintf(op,"%d %d %d\n",data[i],data[i],out); 
				fprintf(up,"1 %d %d\n",temp,out);
				}
				if((sl[i]==0&&sl[j]==3)||(sl[i]==3&&sl[j]==0)){/*This means that bit involves in B0 and B1 hash function*/
				fprintf(op,"%d %d %d\n",data[i],data[i],out); 
				fprintf(up,"2 %d %d\n",temp,out);
				}
				if((sl[i]==1&&sl[j]==2)||(sl[i]==2&&sl[j]==1)){/*This means that bit involves in B0 and B1 hash function*/
				fprintf(op,"%d %d %d\n",data[i],data[i],out); 
				fprintf(up,"2 %d %d\n",temp,out);
				}
				if((sl[i]==1&&sl[j]==3)||(sl[i]==3&&sl[j]==1)){/*This means that bit involves in B1 hash function*/
				fprintf(op,"%d %d %d\n",data[i],data[i],out); 
				fprintf(up,"1 %d %d\n",temp,out);
				}
				if((sl[i]==2&&sl[j]==3)||(sl[i]==3&&sl[j]==2)){/*This means that bit involves in B0 hash function*/
				fprintf(op,"%d %d %d\n",data[i],data[i],out); 
				fprintf(up,"0 %d %d\n",temp,out);
				}
			}	
			j++;
		}
		i++;
		printf("'");

	}
	//printf("%d\n",data[0]);
	fclose(ip);
	fclose(op);
	fclose(up);
}
