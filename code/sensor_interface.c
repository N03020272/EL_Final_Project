#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

	wiringPiSetup();
	pinMode(0, OUTPUT);
	//printf("hi\n");
	digitalWrite(0, LOW);
	delayMicroseconds(2000);
	digitalWrite(0, LOW);
	delayMicroseconds(40);
	//printf("what?\n");
	pinMode(0, INPUT);
	while(digitalRead(0));
	//The sensor activation is done

	int useconds = 0;
	int preamble_1 = 0;
	int preamble_2 = 0;
	int outbit[40];
	int preamble_finished = 0;
	int i;
	int oneFlag = 0;
	int RH_value[16];
	int T_value[16];
	int CheckSum[8];
	
	if(!digitalRead(0))
	{
		while(useconds < 70)		
		{
			delayMicroseconds(1);
			useconds++;
			preamble_1 = 1;
		}

		useconds = 0;

		//wait until the bus goes high to sense
		//next cycle of preamble
		while(!digitalRead(0));
		//{
		//	printf("2 do we get stuck in here?");
		//}

		//wait at least 70us for bus to be high
		while(useconds < 70)
		{
			delayMicroseconds(1);
			useconds++;
			preamble_2 = 1;
		}
		
		useconds = 0;

		//wait until the bus goes low to indicate
		//the start of the data transmission
		while(digitalRead(0));
		//{
		//	printf("3 do we get stuck in here?");
		//}

		if(preamble_1 & preamble_2)	
			preamble_finished = 1;

		if(preamble_finished)
		{
			for(i = 0; i < 40; i++)
			{
				//bus is low for at least 40us
				
				//wait until the bus goes high to sense
				//next cycle of transmission
				while(!digitalRead(0));
			
				while(digitalRead(0))
				{
					//printf("heeeeh");
					useconds++;
					delayMicroseconds(1);
					//printf("do we get stuck in here?");
					if(useconds > 50)
					{
						outbit[i] = 1;
						oneFlag = 1;
						if(i == 39)
						{
							//printf("yoyoyoyoyo");
							break;
						}
					}
				}
				if(!oneFlag)
				{
					outbit[i] = 0;
				}
				oneFlag = 0;
				useconds = 0;
				//if we had a 1 condition, we need to wait 
				//until the bus goes low again
				//while(digitalRead(0) && outbit[i] == 1);	
			}
		}
	}
	
	i = 0;
	int RH_index = 0;
	int T_index = 0;
	int Chk_index = 0;
	while(i < 40)
	{
		if(i < 16)
		{
			RH_value[RH_index] = outbit[i];
			RH_index++;
		}
		if(i >= 16 && i < 32)
		{
			T_value[T_index] = outbit[i];
			T_index++;
		}
		if(i >= 32)
		{
			CheckSum[Chk_index] = outbit[i];
			Chk_index++;
		}
		//printf("%d", outbit[i]);
		i++;
	}
	int j;
	int RH_decimal = 0;
	int Temp_decimal = 0;
	for(i = 0, j = 15; i < 16; i++, j--)
	{
		RH_decimal = RH_decimal + (RH_value[i] << j);
		printf("%d", RH_value[i]);
	}
	printf("\n%d", RH_decimal/10);
	printf("\n");
	for(i = 0, j = 15; i < 16; i++, j--)
	{
		Temp_decimal = Temp_decimal + (T_value[i] << j);
		printf("%d", T_value[i]);
	}
	printf("\n%d", Temp_decimal/10);
	printf("\n");
	for(i = 0; i < 8; i++)
		printf("%d", CheckSum[i]);
	printf("\n");
	
	for(i = 0; i < 40; i++)
		printf("%d", outbit[i]);
	
	FILE *fp;
	//int temp[2];
	//sprintf(temp, "%d", Temp_decimal);
	//int rh[2];
	//sprintf(rh, "%d", RH_decimal);
	fp=fopen("readings.txt","w");
	printf("\nEnter data to be stored in to the file:");
	//while((ch=getchar())!=EOF)
	fprintf(fp, "%d", Temp_decimal/10);
	fprintf(fp, "\n");
	fprintf(fp, "%d", RH_decimal/10);
	fclose(fp);

	return 0;
}
