
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
