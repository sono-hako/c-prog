/*
Christopher Naranjo : PID 2410292
Offhand HW #3
COP4338 - Programming 3

Questions 2, 3
*/

#include <stdio.h>
#include <stdlib.h>

/*2) Create a struct of make_complex, with two arg*/
typedef struct {
	double real;
	double imaginary;
}Complex;

Complex* make_complex(double r, double i){
	Complex* a;
	a = malloc(sizeof(Complex));//allocate memory for a

	if(!a){//check if memory was allocated correctly
		printf("unable to allocate memory\n");
		return NULL;
	}

	(*a).real = r;
	(*a).imaginary = i;

	return a;
}

Complex* add_complex(Complex* fst, Complex* snd){
	Complex* result;

	result = malloc(sizeof(Complex)); //allocate memory for result

	if(!result){//check if memory was allocated correctly
		printf("unable to allocate memory\n");
		return NULL;
	}

	(*result).real = (*fst).real + (*snd).real;
	(*result).imaginary = (*fst).imaginary + (*snd).imaginary;

	return result;
}

/*3) Create array of size 'n', initilized every position to initial_value*/
int* create_array(int n, int initial_value){
	
	if(!n){//check for correct input
		printf("cannot allocate space of size: 0\n");
		return NULL;
	}

	int* tmp;
	tmp = (int*) malloc(n * sizeof(int));

	if(!tmp){//check if memory was allocated correctly
		printf("unable to allocate memory\n");
		return NULL;
	}
	n--;//we need n elements in total, so subtract 1, since first position is at 0
		//eg: n = 4, locations to store at: 0, 1, 2, 3 (total 4)
	while(n >= 0){
		*(tmp+n) = initial_value;
		n--;
	}

	return tmp;
}


int main(int argc, char* argv[]){
	/* I need user input functions now */

	double a;
	double b;
	int* arr;
	Complex* c1;
	Complex* c2;
	Complex* c3;
	/*2) complex numbers */
	printf("1) Enter two doubles for a complex number\ne.g. [1.0 2.0]\n");
	scanf("%lf %lf", &a, &b);
	c1 = make_complex(a, b);

	printf("Enter two doubles to ADD to complex number\ne.g. [1.0 2.0]\n");
	scanf("%lf %lf", &a, &b);
	c2 = make_complex(a, b);

	if(!c1 || !c2)//memory allocation failed, exit
		return -1;

	c3 = add_complex(c1, c2);

	if(!c3)//memory allocation failed somehow, exit
		return -1;
	printf("Results: \n");
	printf("C1: %f %f\n", c1->real, c1->imaginary);
	printf("C2: %f %f\n", c2->real, c2->imaginary);
	printf("Sum: %f %f\n", c3->real, c3->imaginary);

	/* 3) dynamic array creation and allocation*/
	printf("3) Enter size of array to create, and initial value\n[size initial_value]\n");
	int sz, init;
	scanf("%d %d", &sz, &init);

	arr = create_array(sz, init);//add function for recieving user input

	if(!arr)
		return -1;
	int i = 0;

	printf("Array size: %d\n", sz);
	printf("Contents: ");
	while(i < 5){
		printf("%d ", *(arr+i));
		i++;
	}
	printf("\n");

	/*free all allocated memory*/
	free(c1);
	free(c2);
	free(c3);
	free(arr);
	return 0;
}

/*
------------------Disclamer------------------
I understand that this homework assignment cannot be joint work with another student in the class or whoever previously took this class. I hereby certify that I worked on this homework assignment by myself. I did not share any part of my code with my classmates and I did not copy any part of the code from anyone else or from Internet.

During the assignment, I only consulted the following book or online sources: (Please list your sources here.)

By putting my name below, I understand that the course adopts a zero tolerance policy for cheating. I cannot give code to or receive code from other students. I cannot pull code from the internet. I understand any ethical violation will be reported to the university academic affairs and will result in an F grade for this course.
 
Name: Christopher Naranjo

Date: 2/14/2015
*/