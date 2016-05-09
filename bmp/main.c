

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "b-lib.h"

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
  int row, col, scaleCount;
  if ((rows <= 0) || (cols <= 0)) return -1;//make sure we actually have something to work with
  /*
  scale those rows and cols
  */
  *newrows = rows * scale;
  *newcols = cols * scale;

  *new = (PIXEL*) malloc((*newcols) * (*newrows) * sizeof(PIXEL));//allocate space for new scaled array
  /*
  copys everything line by line
        source: ab
                cd

  outer loop 1->aabb

                aabb
  outer loop 2->ccdd
  */
  for(row = 0; row < (*newrows); row++){//for each row, while we havent finished -every- location in the scaled array
    PIXEL* source = original + (row / scale) * cols;//save source position, (dividing row by scale, since loop continues to newrows size)
    PIXEL* dest = *new + (row * (*newcols));//get the starting position to start placing scaled pixels
    for(col = 0; col < cols; col++, source++){
      for(scaleCount = 0; scaleCount < scale; scaleCount++, dest++){//copy that pix over scale times, moving dest position after each copy
        *dest = *source;
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
  if ((rows <= 0) || (cols <= 0)) return -1;//make sure we actually have something to work with

  if(rotation % 90 != 0){//want rotation in 90 degre increments
    fprintf(stderr, "Rotation not a multiple of 90, value entered: %d\n", rotation);
    return -1;
  }
  /*
  Change cols and rows depending on rotation
  (180): nothing changes, new rows = old rows and new cols = old cols
  (90): rows = cols, cols = rows, everything is on its side
  */
  if((rotation % 180) == 0){
    *newrows = rows;
    *newcols = cols;
  } else {
    *newrows = cols;
    *newcols = rows;
  }
  rotation = ((rotation % 360) / 90);//reduce rotation into values [-4..4] (so we dont have to iterate many times)
  //that is, representing everything in the amount of 90deg rotations required

  *new = (PIXEL*)malloc( (cols) * (rows) * sizeof(PIXEL));//allocate new space
  //(rotation % 360) returns negative numbers for negative rotation oddly enough
  if(rotation < 0)
    rotation += 4;//add 4 to fix rotation, e.g. '-3'(representing -270) becomes 1(90deg)

  switch(rotation){
    case 1://90, -270
      for(row = 0; row < rows; row++){
        for(col = 0; col < cols; col++){
          PIXEL* source = original + row*cols + col;//grab source pixel offset
          PIXEL* dest = (*new) + ((cols-col-1) * (*newcols)) + row;//get dest offset (cols became rows, rows become cols)
         *dest = *source; 
        }
      }
      break;
    case 2://180, -180
      for(row = 0; row < rows; row++){
        for(col = 0; col < cols; col++){
          PIXEL* source = original + row*cols + col;//grab source pixel offset
          PIXEL* dest = (*new) + ((rows - row - 1) * (*newcols)) + (cols - col - 1);//get dest offset (start writing from bottom-right corner, and go up)
          *dest = *source;
        }
      }
      break;
    case 3://270, -90
      for(row = 0; row < rows; row++){
        for(col = 0; col < cols; col++){
          PIXEL* source = original + row*cols + col;//grab source pixel offset
          PIXEL* dest = (*new) + (rows * col) + (rows - row - 1);//get dest offset (cols became rows, rows become cols), write in reverse to 90deg
          *dest = *source;
        }
      }
      break;
    default://0, 360.., i wanted to just return the source array and not have to duplicate everything
      //had trouble with that, so just copied everything into output
      for(row = 0; row < rows; row++){
        for(col = 0; col < cols; col++){
          PIXEL* source = original + row*cols + col;
          PIXEL* dest = (*new) + row*cols + col;
         *dest = *source; 
        }
      }
      break;
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

int main(int argc, char** argv)
{
  char flag;//flag for operations
  int r, c, op;//r: rows, c: cols, op: getopt stuff
  int nr, nc;//nr: new rows, nc: new cols
  int scaleSize = 0;//scaleing size
  int rotAmount = 0;//rotation amount (in degrees)
  PIXEL *inPix = NULL, *outPix = NULL;//inpix: input pixels, outpix: output pixels
  char* inFile = NULL;//string for input file name
  char* outFile = NULL;//string for output file name
  
  opterr = 0;//error catch for getopt, but its not like i'm really error checking (yes i know this is bad practice)
  while( (op = getopt(argc, argv, "fs:r:o:")) != -1){
    switch(op){
      case 'f':
        flag = 'f';
        break;
      case 's':
        flag = 's';
        scaleSize = atoi(optarg);
        break;
      case 'r':
        flag = 'r';
        rotAmount = atoi(optarg);
        break;
      case 'o':
        outFile = optarg;
        break;
      case '?':
        if(optopt == 's')
            fprintf(stderr,"Option -%c requires additional arguments.\n", optopt);
        if(optopt == 'r')
            fprintf(stderr,"Option -%c requires additional arguments.\n", optopt);
        return 1;
        break;
      default:
        fprintf(stderr, "Unknown command %c\n", optopt);
        return 1;
        break;
    } 
  }

  if(optind < argc)//final argument is input bmp (optind holds the offset)
    inFile = *(argv+optind);

  readFile(inFile, &r, &c, &inPix);

  switch(flag){
    case 'f':
      flip(inPix, &outPix, r, c);//not checking return value (bad bad)
      break;
    case 's':
      enlarge(inPix, r, c, scaleSize, &outPix, &nr, &nc);//still not checking return value
      break;
    case 'r':
      rotate(inPix, r, c, rotAmount, &outPix, &nr, &nc);//still not checking return value
      break;
  }

  writeFile(outFile, nr, nc, outPix);
  free(inPix);
  free(outPix);
  return 0;
}
