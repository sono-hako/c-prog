/*
Christopher Naranjo
COP4338
PID: 2410292

Find the two largest integers entered.

*/

#include <stdio.h>
#include <stdlib.h>

double find_two_largest(const int* a, int n,
						int* largest,
						int* second_largest){
	int pos = 0;
	while(pos < n){
		if(*(a+pos) > *largest){//new largest
			*second_largest = *largest;//move largest down to second now
			*largest = *(a+pos);//set new largest
		}
		pos++;
	}
	//Wouldn't this be better as a void function?
	return 0.0;
}

int main(int argc, char* argv[]){

	if(argc == 1){
		printf("No elements entered\n");
		printf("usage ./largest [list of numbers: 1 2 3 4]\n");
		return 0;
	}
	int size = argc - 1;
	int fst = 0;
	int snd = 0;
	int i = 1;
	//allocateing size of argc into *in, init everything to zero with calloc
	int* in = (int*) calloc((size), sizeof(int));
	/*
	I really should error check for values that will break atoi()...
	*/
	while(i < argc){
		*(in + (i - 1)) = atoi(*(argv+i));//convert argv elements into ints
		i++;
	}

	find_two_largest(in, size, &fst, &snd);
	printf("Two largest elements entered: %d, %d\n", fst, snd);
	free(in);
	return 0;
}