/*
Problems:
Enlarge segfaults at any value higher than 2, and padding isnt actual padding...
Rotate only actually rotates clockwise
	(I'm converting negative angles to their positive representation, so -90 results in 270 rotation)
*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "piclib.h"

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale, 
			PIXEL** new, int* newrows, int* newcols) 
{
	int row, col, scale_r, scale_c;//row, col, scaled rows, scaled cols
	*newrows = rows * scale;
	*newcols = cols * scale;
	if(scale < 1)//dont scale 0 or negative
		return -1;
	if( (rows <= 0) || (cols <= 0))
		return -1;//size isn't correct, stop

	*new = (PIXEL*) malloc((rows * scale) * (cols * scale) * sizeof(PIXEL));//malloc scaled size

	for(row = 0; row < rows; row++){
		for(col = 0; col < cols; col++){
			PIXEL* source = original + row*cols + col;//grab the original pixel	
			for(scale_r = 0; scale_r < scale; scale_r++){//go through the additional scaled row pixels
				for(scale_c = 0; scale_c < scale; scale_c++){//scaled col pixels
					/*
					skips every other column, essentially padding black spaces between each pixel..
					but this isnt real padding anyway.. 
					*/
					PIXEL* dest = (*new) + (scale_c * scale * col) + ((scale_r * scale * row) * (*newcols));
					*dest = *source;
				}
			}
		}	
	}
	return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read 
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 * 
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation, 
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
		 PIXEL** new, int* newrows, int* newcols)
{
	int row, col;
	if( (rows <= 0) || (cols <= 0))
		return -1;//size isn't correct, stop

	*new = (PIXEL*) malloc(rows*cols*sizeof(PIXEL));
	/*positive rotation only... ran out of time for proper counter-clockwise rotation
		main converts all rotations in to a single positive digit (1, 2, or 3)
		calls rotation up to that many times
		worst case: 3 rotations occur (270 and -90) 
	*/
	*newcols = rows;
	*newrows = cols;

	for(row = 0; row < rows; row++){
		for(col = 0; col < cols; col++){
			PIXEL* source = original + row*cols + col;
			//since rows and cols swapped...  new rows are previous cols.. new cols are previous rows
			//essentially same as above, but swapping row with (cols-1-col) and col with row..
			PIXEL* dest = (*new) + ((cols-1-col) * rows + row);
			*dest = *source;
		}
	}		
	return 0;

}

/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile(). 
 * 
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols) 
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

void errScale(){
	fprintf(stderr, "Incorrect usage: -s [scale amount, non-negative]\n");
	exit(-1);
}

void errRot(){
	fprintf(stderr, "Incorrect usage: -r [rotate amount]\n");
	exit(-1);
}

void genericErr(){
	fprintf(stderr, "Incorrect usage: -s [scale amount] -r [rotate amount] -f -o[output file] [input file] \n");
	exit(-1);
}

int main(int argc, char** argv)
{
	int r, c, opt, scale = 0, f = 0;
	int rot = 0;
	int newr, newc;
	int oldr, oldc;
	PIXEL *b, *nb, *tmp;

	char* out = NULL;//out defauling to stdout (actually happens in provided writefile() function)

	
	while( (opt = getopt(argc, argv, "s:r:fo:")) != -1){
		switch(opt){
			case 's':
				scale = atoi(optarg);
				if(scale < 0)
					errScale();
				break;
			case 'r':
				rot = atoi(optarg);
				if(rot % 90 != 0)
					errRot();
				break;
			case 'f':
				f = 1;
				break;
			case 'o':
				 out = optarg;//outputfile provided
				 break;
			default:
				genericErr();
				break;
		}
	}
	if(optind < argc)//inputfile name is final argument
    	readFile(argv[optind], &r, &c, &b);//adjust in to point to that file path in argv, and open it
    else
    	readFile(NULL,&r,&c,&b);//using stdin
	/*
	printf("%i", optind);
	printf("scale:  %i \n", scale); */
	/*
	call order.. scale > rotate > flip
	*/
	//readFile("example.bmp", &r, &c, &b);
	newr = oldr = r;
	newc = oldc = c;
	tmp = b;
	if (scale > 0) {
		enlarge(b, oldr, oldr, scale, &nb, &newr, &newc);
	} else if (rot != 0){
		while( (rot % 360) == 0){
			rot = rot / 360;//reduce to lowest amount of rotations
		}
		
		rot/=90;//convert to single digits

		if(rot < 0){//negative.. -1, -2, -3
			// -1: -90 = 270
			// -2: -180 = 180
			// -3: -270 = 90
			rot -= 4;//subtract from "4" (which is 360)
		}
		if(rot > 0){
			rotate(tmp, oldr, oldc, rot, &nb, &newr, &newc);//rotate once
			rot--;
			while(rot > 0){//if theres multiple rotations...
				rotate(nb, oldr, oldc, rot, &nb, &newr, &newc);//keep rotating until rot == 0;
				rot--;
			}
		}
	} else if (f == 1){
		flip(b, &nb, newr, newc);
	}
	writeFile(out, newr, newc, nb);
		
	free(b);
	free(nb);
	return 0;
}
