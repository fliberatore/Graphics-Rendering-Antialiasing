/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include	<cmath>

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */

  if(u < 0.0f || v < 0.0f || u > 1.0f || v > 1.0f)
	  return GZ_FAILURE;
  float posU = u*(xs-1); int lowU =(int) floor(posU); int highU =(int) ceil(posU); float deltaU = posU - lowU;
  float posV = v*(ys-1); int lowV =(int) floor(posV); int highV =(int) ceil(posV); float deltaV = posV - lowV;

  GzColor corner[4];
  memcpy(corner[0], image[lowV * xs + lowU], sizeof(GzColor));
  memcpy(corner[1], image[lowV * xs + highU], sizeof(GzColor));
  memcpy(corner[2], image[highV * xs + lowU], sizeof(GzColor));
  memcpy(corner[3], image[highV * xs + highU], sizeof(GzColor));

  color[RED] = (1-deltaU)*(1-deltaV)*corner[0][RED] + deltaU*(1-deltaV)*corner[1][RED] + (1-deltaU)*deltaV*corner[2][RED] + deltaU*deltaV*corner[3][RED];
  color[GREEN] =(1-deltaU)*(1-deltaV)*corner[0][GREEN] + deltaU*(1-deltaV)*corner[1][GREEN] + (1-deltaU)*deltaV*corner[2][GREEN] + deltaU*deltaV*corner[3][GREEN];
  color[BLUE] = (1-deltaU)*(1-deltaV)*corner[0][BLUE] + deltaU*(1-deltaV)*corner[1][BLUE] + (1-deltaU)*deltaV*corner[2][BLUE] + deltaU*deltaV*corner[3][BLUE];
	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	if( ((int)floor(u*8))%2 == ((int)floor(v*8))%2 )
		color[RED] = color[GREEN] = color[BLUE] = 0;
	else color[RED] = color[GREEN] = color[BLUE] = 1;
	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

