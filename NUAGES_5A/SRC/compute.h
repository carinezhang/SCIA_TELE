
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#define LargeurImage 360

#define NB_CLASS 8
#define SIZE_VECTOR 5

struct pixel {
  int cl;
  int* v;             // The neighbours sorted
};

void ComputeImage(guchar *pucImaOrig, 
		  guint uiNbCol, 
		  guint uiNbLine,
		  guchar *pucImaRes);

int* init_centers();
int dist(int* a, int* b);
int search_center(int* centers, struct pixel p);
int* find_neighbours(guchar *pucImaRes, int x, int y, int NbLine, int NbCol);
struct pixel* init_pixels(guchar *pucImaRes, guint NbLine, guint NbCol, int* centers);
void update_class(int* centers, struct pixel* pixels, int NbTotalPix);
void evaluate_center(int center, int* centers, struct pixel* pixels, int NbCol, int NbLine);
void draw_clouds(guchar *resIma, struct pixel *pixels, int nbPix);
void main_loop(int* centers, struct pixel* pixels, int NbLine, int NbCol);
