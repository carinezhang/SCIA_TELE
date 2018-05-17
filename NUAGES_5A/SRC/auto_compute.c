#include <dirent.h>
#include "compute.h"


void print_result(char *filename, guchar *img)
{
}


void process_image(char *filename)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  if (!pixbuf)
    printf("PIXBUF IS NULL !!!\n");
  GdkPixbuf *pixbufRes = gdk_pixbuf_copy(pixbuf);
  int NbCol = gdk_pixbuf_get_width(pixbuf);
  int NbLine = gdk_pixbuf_get_height(pixbuf);
  guchar *pucImaOri = gdk_pixbuf_get_pixels(pixbuf);
  guchar *pucImaRes = gdk_pixbuf_get_pixels(pixbufRes);
  ComputeImage(pucImaOri, NbLine, NbCol, pucImaRes);

  // COUNT % pixels
}

char *update_name(char *filename)
{
  char *res = malloc(256 * sizeof(char));
  for (int i = 0; i < 256; i++)
    res[i] = 0;
  strcat(res, "../EXE/");
  strcat(res, filename);
  return res;
}

int process_images()
{
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir("../EXE/")) == NULL)
    return 1;
  while ((ent = readdir(dir)) != NULL)
  {
    // Check si le fichier est un .bmp
    char* name = ent->d_name;
    size_t len = strlen(name);
    if (strcmp(name, "..") == 0 || strcmp(name, ".") == 0)
      continue;
    if (strcmp(name + (len - 4), ".bmp") != 0)
      continue;
    char *new_name = update_name(name);

    printf("%s\n", new_name);

    process_image(new_name);
  }
  closedir(dir);
  return 0;
}

int main(int argc, char **argv)
{
  process_images();
  return 0;
}


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

}
