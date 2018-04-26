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
	      (les lignes sont mises les unes à la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes à la suite des autres)


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

int abs_val(int value)
{
  if (value < 0)
    return -1 * value;
  return value;
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

/*
struct pixel* init_pixels(guchar *pucImaRes, guint NbLine, guint NbCol)
{
  struct pixel* pixels = malloc(NbLine * NbCol * sizeof(struct pixel));
  iNbPixelsTotal=NbCol*NbLine;
  for(iNumPix=0;
      iNumPix<iNbPixelsTotal*iNbChannels;
      iNumPix=iNumPix+iNbChannels){
    
  }
}
*/
