#include <dirent.h>
#include "compute.h"


void print_result(char *filename, guchar *img, int NbTotal)
{
  int nb_clouds = 0;
  for (int i = 0; i < NbTotal; i++)
    if (*(img + i * 3) == 255)
      nb_clouds++;
  printf("File:%s, number of clouds: %d/%d\n", filename, nb_clouds, NbTotal);
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
  print_result(filename, pucImaRes, NbCol * NbLine);
}

char *update_name(char *filename)
{
  char *res = malloc(256 * sizeof(char));
  for (int i = 0; i < 256; i++)
    res[i] = 0;
  strcat(res, "./");
  strcat(res, filename);
  return res;
}

int process_images()
{
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(".")) == NULL)
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
    process_image(new_name);
  }
  closedir(dir);
  return 0;
}

int main(int argc, char **argv)
{
  int res = process_images();
  return res;
}
