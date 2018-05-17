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
 
  struct pixel* pixels = init_pixels(pucImaRes, NbLine,NbCol, centers);
  for (int i = 0; i < NB_CLASS; i++)
    printf(" %d ", centers[i* SIZE_VECTOR]);
  printf("\n");
  // LOOP
  main_loop(centers, pixels, NbLine, NbCol);
  draw_clouds(pucImaRes, pixels, NbLine * NbCol);
}

void array_copy(int* src, int* dest)
{
  memcpy(dest, src, NB_CLASS * SIZE_VECTOR * sizeof(int));
}

int check_array(int* a, int* b)
{
  for (int i = 0; i < SIZE_VECTOR * NB_CLASS; i++)
    if (a[i] != b[i])
      return 1;
  return 0;
}

void main_loop(int* centers, struct pixel* pixels, int NbLine, int NbCol)
{
  int REPEAT = 500;
  static int test = 0;
  int *old_centers = malloc(SIZE_VECTOR * NB_CLASS * sizeof(int));
  for (int r = 0; r < REPEAT; r++)
  {
    array_copy(centers, old_centers);
    for (int i = NB_CLASS - 1; i >= 0; i--)
    {
      evaluate_center(i, centers, pixels, NbCol, NbLine);
      printf("update_class\n");
      update_class(centers, pixels, NbLine * NbCol);
      for (int i = 0; i < NB_CLASS; i++)
        printf(" %d ", centers[i* SIZE_VECTOR]);
      printf("\n");
    }
    if (check_array(old_centers, centers))
    {
      if (test == 0)
        break;
      test++;
    }
  }
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

/*
 * Compute the square distance between 2 vectors.
 *
 * Input: 2 vectors of SIZE_VECTOR length
 *
 * Output: the square distance
 */
int dist(int* a, int* b)
{
  int res = 0;
  for (int i = 0; i < SIZE_VECTOR; i++)
  {
    if (a[i] == -1 || b[i] == -1)
      continue;
    res += (b[i] - a[i])  * (b[i] - a[i]);
  }
  return res;
}

int search_center(int* centers, struct pixel p) 
{
  int index = 0;
  int dist_min = dist(centers, p.v);
  for (int i = 1; i < NB_CLASS; i++)
  {
    int dist_t = dist(centers + (i * SIZE_VECTOR), p.v);
    if (dist_t < dist_min)
    {
      index = i;
      dist_min = dist_t;
    }
  }
  return index;
}

int cmpfunc (const void * a, const void * b) {
     return ( *(int*)a < *(int*)b );
}

int* find_neighbours(guchar *pucImaRes, int x, int y, int NbLine, int NbCol)
{
  int* res = malloc(SIZE_VECTOR * sizeof(int));
  for (int i = 0; i < SIZE_VECTOR; i++)
    res[i] = -1;
  res[0] = *(pucImaRes + x * NbCol * 3 + y * 3);
  if (x > 0)
    res[1] = *(pucImaRes + (x - 1) * NbCol * 3 + y * 3);
  if (x < NbLine - 1)
    res[2] = *(pucImaRes + (x + 1) * NbCol * 3 + y * 3);
  if (y > 0)
    res[3] = *(pucImaRes + x * NbCol * 3 + (y - 1) * 3);
  if (y < NbCol - 1)
    res[4] = *(pucImaRes + x * NbCol * 3 + (y + 1) * 3);
  qsort(res, SIZE_VECTOR, sizeof(int), cmpfunc);
  return res;
}

struct pixel* init_pixels(guchar *pucImaRes, guint NbLine, guint NbCol, int* centers)
{
  guint i = 0;
  guint j = 0;
  struct pixel* pixels = malloc(NbLine * NbCol * sizeof(struct pixel));
  for (i = 0; i < NbLine; i++)
  {
    for (j = 0; j < NbCol; j++)
    {
      struct pixel *p = &pixels[i * NbCol + j];
      p->v = find_neighbours(pucImaRes, i, j, NbLine, NbCol);
      p->cl = search_center(centers, *p);
    }
  }
  return pixels;
}

void update_class(int* centers, struct pixel* pixels, int NbTotalPix)
{
  for (int i = 0; i < NbTotalPix; i++)
    pixels[i].cl = search_center(centers, pixels[i]);
}


void evaluate_center(int center, int* centers, struct pixel* pixels, int NbCol, int NbLine)
{
  int *sum = malloc(sizeof(int) * SIZE_VECTOR);
  for (int i = 0; i < SIZE_VECTOR; i++)
    sum[i] = 0;
  int n = 0;
  for (int i = 0; i < NbLine * NbCol; i++)
  {
    if (pixels[i].cl != center)
      continue;
    for (int j = 0; j < SIZE_VECTOR; j++)
      if (pixels[i].v[j] != -1)
        sum[j] += pixels[i].v[j];
    n++;
  }
  if (n == 0)
    return;

  for (int i = 0; i < SIZE_VECTOR; i++)
    sum[i] = sum[i] / n;
  qsort(sum, SIZE_VECTOR, sizeof(int), cmpfunc);
  for (int i = 0; i < SIZE_VECTOR; i++)
    centers[center * SIZE_VECTOR + i] = sum[i];

  // If it's the cloud vector assign the mean to each components
  /*
  if (center == NB_CLASS - 1)
  {
    int nb = 0;
    for (int i = 0; i < SIZE_VECTOR; i++)
      if (centers[center * SIZE_VECTOR + i] != -1)
        nb++;
    int mean = centers[center * SIZE_VECTOR + nb / 2];
    for (int i = 0; i < SIZE_VECTOR; i++)
      centers[center * SIZE_VECTOR + i] = mean;
  }
  */
}

void draw_clouds(guchar *resIma, struct pixel *pixels, int nbPix)
{
  for (int i = 0; i < nbPix; i++)
  {
    if (pixels[i].cl == NB_CLASS - 1)
      for (int j = 0; j < 3; j++)
        *(resIma + i * 3 + j) = 255;
    else
      for (int j = 0; j < 3; j++)
        *(resIma + i * 3 + j) = 0;
  }
}

//Division par zero
// free
