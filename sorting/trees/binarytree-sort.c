#include <stdio.h>
#include <stdlib.h>
#include <string.h>//other non-strcmp functions (strcpy and strlen)
#include <unistd.h>//getopt and other stuff related to it


typedef struct curNode{//tree struct
	struct curNode* left;
	struct curNode* right;
	char* str;
	int multi;
}node;

node *root = NULL;//i dont think this is good practice? probably not

/*
Takes a pointer to a curNode.. pointer
char*
and function pointer(*cmp), which is either:
	mystrcmp(not case sensitive)
	mystrcasecmp(case sensitive)
*/
void insert(node ** curNode, char* data, int (*cmp)(char*, char*)){
	node *temp = NULL;
	if( !(*curNode) ){//if there is no node at this location
		temp = (node *)malloc(sizeof(node)); //create a new one
		temp -> str = data;
		temp -> left = NULL;
		temp -> right = NULL;//since this is a root(correct term?), both left and right subtree default to null
		temp -> multi = 1;
		(*curNode) = temp; //set as root
	} else if ( (*cmp)( (*curNode)->str, data ) == 0 ){
		(*curNode)->multi++;//already exists, increment multi
	} else if ( (*cmp)( (*curNode)->str, data ) == -1 ){
		insert(&(*curNode)->right, data, cmp);//to the right(higher) side...
	} else if ( (*cmp)( (*curNode)->str, data ) == 1 ){
		insert(&(*curNode) -> left, data, cmp);//to the left(lower) side..
	}
}

void inOrder(node * curNode, FILE* out){
	if(curNode){//if the node exists..
		inOrder(curNode -> left, out);//go down the left subtree
		while((curNode -> multi) > 0){//while there are duplicates
			fprintf(out, "%s\n", curNode->str);//print it
			curNode->multi--;
		}
		inOrder(curNode -> right, out);//go down the right subtree
	}
}
/*
free() the tree, with post-order traversal
*/
void freeTree(node * tree){//be free, tree
	if(tree){
		freeTree(tree->left);//go down left subtree until null is hit
		freeTree(tree->right);//then go down right subtree until null is hit
		free(tree);
	}
}

/*
case insensitive string compare
*/
int mystrcmp(char* s1, char* s2){
	//copying both strings into something else so i dont edit the original strings..
	char* str1 = malloc(strlen(s1) * sizeof(char));
	char* str2 = malloc(strlen(s2) * sizeof(char));
	strcpy(str1, s1);//copy strings into new location, so we dont edit the original values
	strcpy(str2, s2);
	while(*str1 != '\0'){//since case insensitive... increase anything lower than  'a' by +32
		//i also realize, this is modifying the actual strings..
		if( (*str1) % 'A' < 32)
			*str1 = *str1 + 32;
		if( (*str2) % 'A' < 32)
			*str2 = *str2 + 32;
		/*
		messy.. ternary statements instead?
		*/
		if(*str1 == '\0'){
			return -1;
		} //s1 smaller than s2(not at least the same length..)
		else if(*str1 > *str2){
			return 1; //s1 is bigger than s2
		}
		else if(*str1 < *str2){
			return -1; //s1 is smaller than s2
		}
		str1++;
		str2++;
	}
	free(str1);//at this point we dont need str1 anymore
	if(*s2 != '\0'){
		free(str2);//bye bye str2
		return -1;//s1 is shorter than s2 (and equal).. but str2 has extra words
		//eg; 'same' and 'same word'.. this catches this case..
	}
	return 0;//strings are r=equal
}
/*
case sensitive string compare
*/
int mystrcasecmp(char* s1, char* s2){
	while(*s1 != '\0'){//since case sensitive, leave it alone..
		if(*s1 == '\0')
			return -1;//s1 smaller than s2(not at least the same length..)
		else if(*s1 > *s2)
			return 1;//s1 is bigger than s2
		else if(*s1 < *s2)
			return -1;//s1 is smaller than s2
		s1++;
		s2++;
	}
	if(*s2 != '\0')
		return -1;//s1 is shorter than s2 (and equal, for that portion).. but str2 has extra words

	return 0;//equal
}
/*
error message 
*/
void usage(){
    fprintf(stderr, "incorrect usage: bstsort [-c] [-o output_file_name] [input_file_name]\n");
    exit(1);
}

int main(int argc, char** argv) {
    int c;
    int (*compare)(char*, char*);//function pointer for which compare to use based off caps sensitivity
    char* incBuff = NULL;//getline takes care of malloc/realloc for this?
    char* newline = NULL;
    size_t length = 0;
    ssize_t read;//for getLine: [-1, largenumber)
    FILE *in = stdin, *out = stdout;//default stdin, stdout

    compare = &mystrcmp;//point to insensitive function

    while(( c = getopt(argc, argv, "co:")) != -1){
		switch (c){
			case 'c':
				compare = &mystrcasecmp;//point to sensitive function
				break;
			case 'o':
				out = fopen(optarg, "w");//change out to output file
				break;
			default:
				usage();
				break;
		}
    }
    if(optind < argc)//inputfile name is final argument
    	in = fopen(argv[optind], "r");//adjust in to point to that file path in argv, and open it

   	while( (read = getline(&incBuff, &length,in) ) > 0){//as long as lines keep coming in
		if(read == 1){
			incBuff = '\0';//i dont care if the string length is 1.. means it's just a \n char
			//this would probably be a problem on windows systems with their \r\n newline thing
		}
		else {
			incBuff[read-1] = '\0';//remove newline char.. I guess the string would be 'ab\0\0'..
	    	newline = malloc((read-1) * sizeof(char));//allocate proper space for the string
	    	strcpy(newline, incBuff);//copy it over from incBuff buffer to newline
	    	insert(&root, newline, compare);//to the tree!
    	}
    }

    inOrder(root, out);//print in-order traversal to desired output destination
    free(incBuff);//free the buffer memory from getline..
    freeTree(root);//be free, tree
    fclose(in);//close the files
    fclose(out);
    return (0);
}
