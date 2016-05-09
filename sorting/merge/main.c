/*
Christopher Naranjo
COP4338 - HW 3 mergesort

There are some bugs in this, I started working on this too late and it's increidbly sloppy.
fails for ./merge 10

sometimes segfaults on ./merge 2, and ./merge 5, couldnt figure that out
othertimes it runs just fine
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
Test cases:
	./merge 2
	./merge 5
	./merge 10
	list is always 50 elements long
*/
int* mergeSegments(int*, int*, int, int, int*);

typedef struct thread_container{
	pthread_t id;//thread id from pthread
	int threadnum;//for my own use in merge
	int base_size;//current size of the segment
	int merged;
	int* segment;//the segment of the original array//flag for later on during merging process
	int next_sz;
	int* next;//neighbor's segment
} threads;

/*
So i can pass multiple arguments into mergesegment() with pthread_create
*/
void merge(void* thread_arg){
	int* tmp;
	int* newsize;
	threads *arg;//cast the pthread call back into thread struct
	arg = (threads*) thread_arg;
	tmp = mergeSegments(arg->segment, arg->next, arg->base_size, arg->next_sz, newsize);
	arg->base_size = *newsize;
	arg->segment = tmp;
}
/*
Simple merge portion of mergesort for two arrays

*/
int* mergeSegments(int* left, int* right, int l_sz, int r_sz, int* newsize){
	int i, j, k;
	int dup;
	int* out;
	out = malloc(sizeof(int) * (l_sz + r_sz));
	for(i = 0, j = 0, k = 0, dup = 0; i < l_sz && j < r_sz; k++){
		if(*(left+i) <= *(right+j)){//mergesort puts smaller numbers from left first
			if(*(left+i) == *(right+j))
				dup++;//duplicate counter

			*(out+k) = *(left+i)
;			i++;
		} else {//put item from right
			*(out+k) = *(right+j);
			j++;
		}
	}
	while(i < l_sz){//while there is stuff left in the left array
		*(out+k) = *(left+i);//append it to output
		k++;
		i++;
	}
	while(j < r_sz){//while there is stuff left in the right array
		*(out+k) = *(right+j);//append to output
		j++;
		k++;
	}
	*newsize = l_sz + r_sz;

	printf("Merged %d elements with %d elements. %d duplicates.\n", l_sz, r_sz, dup);
	return out;
}

/*
For qsort, it requires a compare function
 = 0 : same value
 < 0 : a < b
 > 0 : a > b
*/
int compare(const void* a, const void* b){
	return(*(int*)a - *(int*)b);
}
/*
So I can pass multiple arguments into qsort from each thread
*/
void sort(void* thread_arg){
	threads *arg;//cast the pthread call back into thread struct
	arg = (threads*) thread_arg;
	qsort(arg->segment, arg->base_size, sizeof(int), compare);//do that qsort
	printf("Sorted %d elements.\n", arg->base_size);
}


/*
Populate array with random numbers;
*/
void generate(int* arr){
	int i;
	srand(time(NULL));

	for(i = 0; i < 50; i++)
		*(arr++) = (rand() % 50 + 1);
}
/*
void freeAll(int num_th, threads** old){
	int i;
	for(i = 0; i < num_th; i++){
		free(old[i]->segment);
	}
} */

int main(int argc, char** argv){
	int num_th;
	/* first lets check if input is even correct */
	if(argc != 2){
		printf("error: addtional argument required, ./merge [value]\n");
		return -1;
	} else {
		num_th = atoi(argv[1]);
	}

	/* i was told we're just going to always have the array be size 50 */
	int seg_size = 50 / num_th;//size of the segments
	int i, j;//for loops counters
	int arr[50];
	int* arr_ptr = &arr[0];//because i prefer pointer arithmatic

	generate(arr);

	threads** container = malloc(num_th * sizeof(struct threads*));//allocate space for array of thread structs

	for(i = 0; i < num_th; i++){
		container[i] = malloc(sizeof(threads));//allocate space for the actual struct
		container[i]-> segment = malloc(sizeof(int) * seg_size);//allocate space for the array segment, within struct
		container[i]->merged = 0;
		container[i]-> threadnum = i;
		container[i]-> base_size = seg_size;//for qsort and later stuff

		for(j = 0;j< seg_size; j++, arr_ptr++){
			*(container[i]-> segment+j) = *arr_ptr;//populate segment array for this thread struct
		}

		if(pthread_create(&(container[i]->id), NULL, (void*) &sort, container[i])){//start the thread working on sorting it
			printf("thread creation dun did bad, exiting\n");//i should use a better error message
			return -1;
		}
	}

	for(i = 0; i < num_th; i++)
		pthread_join(container[i]->id, NULL);//wait for all the threads to finish sorting their segments

	/*
	I realized later on i should have just used a linked list of structs, instead of arrays
	Would have made this much easier, and less clunky

	*/
	if(num_th > 1){//for 2 threads
		for(i = 0; i < num_th; i+=2){//(merge every other thread with its neighbor)
			if(i+1 >= num_th)
				break;
			container[i]->next = container[i+1]->segment;//copy neighboring pointer into other struct
			container[i]->next_sz = container[i+1]->base_size;//get the size too
			container[i+1]->merged = 1;//set the neighbor to 'merged'
			if(pthread_create(&(container[i]->id), NULL, (void*) &merge, container[i])){//start the thread working on merging the segments
				printf("thread creation dun did bad, exiting\n");//i should use a better error message
				return -1;
			}
		}
	}
	for(i = 0; i < num_th; i++)
		pthread_join(container[i]->id, NULL);

	if(num_th >= 5){

		for(i = 0; i < num_th; i+=4){
			if(i+1 >= num_th)
				break;
			container[i]->next = container[i+2]->segment;//copy neighboring pointer into other struct
			container[i]->next_sz = container[i+2]->base_size;//get the size too
			container[i+2]->merged = 1;
			if(pthread_create(&(container[i]->id), NULL, (void*) &merge, container[i])){//start the thread working on merging the segments
				printf("thread creation dun did bad, exiting\n");//i should use a better error message
				return -1;
			}
		}
		for(i = 0; i < num_th; i++)
			pthread_join(container[i]->id, NULL);

		i=0;
		container[i]->next = container[i+4]->segment;//copy neighboring pointer into other struct
		container[i]->next_sz = container[i+4]->base_size;//get the size too
		container[i+4]->merged = 1;
		if(pthread_create(&(container[i]->id), NULL, (void*) &merge, container[i])){//start the thread working on merging the segments
			printf("thread creation dun did bad, exiting\n");//i should use a better error message
			return -1;
		}

	}

	for(i = 0; i < num_th; i++)
		pthread_join(container[i]->id, NULL);
	/*
	if(num_th == 10){

		for(i = 0; i < num_th; i+=4){
			if(i+1 >= num_th)
				break;
			container[0]->next = container[i+4]->segment;//copy neighboring pointer into other struct
			container[0]->next_sz = container[i+4]->base_size;//get the size too
			container[i+4]->merged = 1;
			if(pthread_create(&(container[i]->id), NULL, (void*) &merge, container[i])){//start the thread working on merging the segments
				printf("thread creation dun did bad, exiting\n");//i should use a better error message
				return -1;
			}
		}	

	}
	/*
	if(num_th == 5){
		for(i = 0; i < num_th; i += )
	}*/
	/*
	if(num_th > 1){
		i = 0;
		j = 1;
		while((i < num_th) && (j < num_th)){

			container[i]->next = container[j]-> segment;
			container[i]->next_sz = container[j]->base_size;//get the size too
			container[j]->merged = 1;//set the neighbor to 'merged'
			if(pthread_create(&(container[i]->id), NULL, (void*) &merge, container[i])){//start the thread working on merging the segments
				printf("thread creation dun did bad, exiting\n");//i should use a better error message
				return -1;
			}
			while(i < (num_th-1) && container[i]->merged){//find the next non-merged neighbor
				i++;
			}
			while(j < (num_th -1) && container[j]->merged){
				j++;
			}
		}

	}*/


	for(i = 0; i < num_th; i++)
		pthread_join(container[i]->id, NULL);//wait for all the threads to finish sorting their segments

	for(i = 0; i < container[0]->base_size; i++){
		printf("%d ", *(container[0]->segment+i));
	}
	free(container);
	printf("\n");
	return 0;
}