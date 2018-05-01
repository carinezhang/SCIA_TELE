#include "compute.h"

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto: 

  
  But: 

  Entrees: 
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes � la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes � la suite des autres)


  Sortie:

  Rem: 

  Voir aussi:

  ---------------------------------------*/
void ComputeImage(guchar *pucImaOrig, 
		  guint NbLine,
		  guint NbCol, 
		  guchar *pucImaRes)
{
  int iNbPixelsTotal, iNumPix;
  int iNumChannel, iNbChannels=3; /* on travaille sur des images couleurs*/
  guchar ucMeanPix;

  printf("Segmentation de l'image.... A vous!\n");
  
  iNbPixelsTotal=NbCol*NbLine;
  for(iNumPix=0;
      iNumPix<iNbPixelsTotal*iNbChannels;
      iNumPix=iNumPix+iNbChannels){
    /*moyenne sur les composantes RVB */
    ucMeanPix=(unsigned char)
	((
	  *(pucImaOrig+iNumPix) +
	  *(pucImaOrig+iNumPix+1) +
	  *(pucImaOrig+iNumPix+2))/3);
    /* sauvegarde du resultat */
    for(iNumChannel=0;
	iNumChannel<iNbChannels;
	iNumChannel++)
      *(pucImaRes+iNumPix+iNumChannel)= ucMeanPix;
  }

  int* centers = init_centers();
  
}

/**
 * Init centers
 *
 * Divide 0 - 255 (spectre) into NB_CLASS parts, find the center of each part,
 * then create NB_CLASS vectors of SIZE_VECTOR and assign the center
 * to each component of the vector of the n class. 
 *
 * Input:
 *
 * Output : vector of int 
 */
int* init_centers() 
{
  int *centers = malloc(NB_CLASS * SIZE_VECTOR * sizeof(int));
  int upper = 256 / NB_CLASS;
  int start = upper / 2;
  for (int i = 0; i < NB_CLASS; i++)
  {
    for (int j = 0; j < SIZE_VECTOR; j++)
    {
      centers[i * SIZE_VECTOR + j] = start + (i * upper);
    }
  }
  return centers;
}

int dist(int* a, int* b)
{
  int res = 0;
  for (int i = 0; i < SIZE_VECTOR; i++)
  {
    if (a[i] == -1 || b[i] == -1)
      continue;
    res += (a[i] - b[i])  * (a[i] - b[i]);
  }
  return res;
}

int search_center(int* centers, struct pixel p) 
{
  int index = 0;
  int dist_min = dist(centers, p.v);
  for (int i = 1; i < NB_CLASS; i++)
  {
    int dist_t = dist(centers + i * SIZE_VECTOR, p.v);
    if (dist_t < dist_min)
    {
      index = i;
      dist_min = dist_t;
    }
  }
  return index;
}

int cmpfunc (const void * a, const void * b) {
     return ( *(int*)a > *(int*)b );
}

int* find_neighbours(guchar *pucImaRes, int x, int y, int NbLine, int NbCol)
{
  int* res = malloc(SIZE_VECTOR * sizeof(int));
  for (int i = 0; i < SIZE_VECTOR; i++)
    res[i] = -1;
  res[0] = *(pucImaRes + x * NbCol + y);
  if (x > 0)
    res[1] = *(pucImaRes + (x - 1) * NbCol + y);
  if (x < NbLine - 1)
    res[2] = *(pucImaRes + (x + 1) * NbCol + y);
  if (y < 0)
    res[3] = *(pucImaRes + x * NbCol + (y - 1));
  if (y > NbCol - 1)
    res[4] = *(pucImaRes + x * NbCol + (y + 1));
  qsort(res, SIZE_VECTOR, sizeof(int), cmpfunc);
  return res;
}

struct pixel* init_pixels(guchar *pucImaRes, guint NbLine, guint NbCol, int* centers)
{
  struct pixel* pixels = malloc(NbLine * NbCol * sizeof(struct pixel));

  for (int i = 0; i < NbCol; i++)
  {
    for (int j = 0; j < NbLine; j++)
    {
      struct pixel p = pixels[i * NbCol + j];
      p.cl = search_center(centers, p);
      p.v = find_neighbours(pucImaRes, i, j, NbLine, NbCol);
    }
  }
  return pixels;
}

void update_class(int* centers, struct pixels* pixels, int NbTotalPix)
{
  for (int i = 0; i < NbTotalPix; i++)
    pixels[i].cl = search_center(centers, pixels[i]);
}


int evaluate_center(int center, int* centers, struct pixel* pixels, int NbCol, int NbLine)
{
  int *classPixels = malloc(sizeof(int) * NbCol * NbLine);
  int nbVec = 0;
  for (int i = 0; i < NbCol; i++) {
    for (int j = 0; j < NbLine; j++) {
      if (pixels[i * NbCol + j].cl == center) {
        classPixels[nbVec] = pixels[i * NbCol + j].v[SIZE_VECTOR/2]
        nbVec++;
      }
    }
  }
  qsort(classPixels, nbVec, sizeof(int), cmpfunc);
  // calcule du nouveau centre c

  if (nbVec != 0) {
    int c = classPixels[nbVec/2];

    for (int i = 0; i < SIZE_VECTOR; i++)
      centers[center * SIZE_VECTOR + i] = c; 
  } 

  free(classPixels);
}

//Division par zero
// free