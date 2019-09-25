#include <stdlib.h>
#include <math.h>                   /* for lrint */
#include "quad.h"
#include "pgmio.h"

/* Reconstruct image from quadtree.  Write into the Image record that im points
 * to.  Assume that the square region described by the quadtree fits into the
 * Image.
 *
 * Recall that each Quad node already contains position information; respect it,
 * and you will benefit from it too (a secret for successful recursions).
 */
void saveQuad(Image *im, Quad *q)
{
    if (im == NULL || q == NULL) return;

    int i,j,k;

    if (q -> children[0] == NULL)
    {
        for (j=0;j<q->width;j++)
        {
            for (k=0;k<q->width;k++)
            {
                *(im->data + ((q -> ty) + k)*(im -> sy) + ((q -> tx) + j)) = q -> grayscale;
            }
        }
    }
    else
    {
        for(i=0;i<4;i++)
        {
            saveQuad(im, q->children[i]);
        }
    }
}

/* Build quadtree from image.  Splitting stops when maxDepth hits 0 or maximum
 * difference in grayscale is <= threshold.  Do this to only the square region
 * at top-left corner (tx, ty) of width w.  Use heap allocation for all Quad
 * nodes you produce.
 */
Quad *formQuadtree(Image *im, int maxDepth, int threshold, int tx, int ty, int w)
{
    int max=0, min=255, i, j, k, s, avg=0, pixel;

    Quad *HQuad = (Quad *)malloc(sizeof(Quad));
    HQuad -> children[0] = NULL;
    HQuad -> children[1] = NULL;
    HQuad -> children[2] = NULL;
    HQuad -> children[3] = NULL;
    HQuad -> tx = tx;
    HQuad -> ty = ty;
    HQuad -> width = w;

    for (s=0;s<w;s++)
    {
        for (k=0;k<w;k++)
        {
            pixel = *(im->data + (ty + k)*(im -> sy) + tx + s);
            avg = avg + pixel;
            if (pixel < min) min = pixel;
            if (pixel > max) max = pixel;
        }
    }

    avg = lrint(avg / (w*w));

    if (w == 1 || maxDepth == 0 || max - min <= threshold)
    {
        HQuad -> grayscale = avg;
        return HQuad;
    }

    for (i=0;i<2;i++)
    {
        for (j=0;j<2;j++)
        {
            HQuad -> children[2*i + j] = formQuadtree(im, maxDepth - 1, threshold, tx + (j*w/2), ty + (i*w/2), w/2);
        }
    }

    return HQuad;
}

/* Deallocate all Quad nodes of the given quadtree. Assume that they all came
 * from the heap.
 */
void freeQuadtree(Quad *q)
{
    if(q!=NULL)
    {
        for(int i=0;i<4;i++)
        {
            freeQuadtree(q->children[i]);
        }

    free(q);
    }
}
