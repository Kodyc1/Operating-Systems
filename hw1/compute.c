#include <stdio.h>
#include <stdlib.h>

int main()
{
	// initializing variables for finding
	// average, min, and max
	float total = 0;
        int count = 0;
        
	float max = 0;
	float min = 99999999;
	
	float input;
	
	// while stdin is not empty and is taking
	// in the referenced input
	while(scanf("%f",&input) == 1)
	{
	 	//printf("%f\n",input);	
		count += 1;
		total += input;

		if (input > max){
			max = input;
		}
		if (input < min){
			min = input;
		}	
	}

	if (count == 0){
		printf("No input found\n");
		exit(1);
	}

	float average = total/count;

	printf("max: %f\nmin: %f\naverage: %f\n",max,min,average);



	return 0;
}
