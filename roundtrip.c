#include <stdio.h>
#include <stdlib.h>
#include "pgmio.h"
#include "quad.h"

/* Command line arguments:
 * input file, output file, threshold, max depth
 */
int main(int argc, char *argv[])
{
  if (argc < 5) {
    printf("Parameters: inputfile outputfile threshold maxdepth\n");
    return 1;
  }

  struct image *im = readPGMimage(argv[1]);
  
  printf("PRE");
  Quad *q = formQuadtree(im, atoi(argv[4]), atoi(argv[3]), 0, 0, im->sx);
  printf("LOL FUNCTION 2 WORKS");
  
  for (int j=0;j<512;j++)
        {
            for (int k=0;k<521;k++)
            {
                *(im->data + ((q -> ty) + j)*(im -> sy) + ((q -> tx) + k)) = 255;
            }
        }
  
  saveQuad(im, q);
  writePGMimage(im, argv[2]);

  freeImage(im);
  freeQuadtree(q);

  return 0;
}
