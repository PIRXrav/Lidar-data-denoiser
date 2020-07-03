#include "HL_LP.h"

unsigned char NB_MAT;
unsigned char couleur;


//Initialise la couleur
unsigned char HL_init_couleur(unsigned char c) {
	couleur = c;
	return c;
}


// Index Circulaire 
// Rend un tableau circulaire
INLINE size_t IC(signed int i) {

	if (i < 0)
		return i + NB_POINT;
	if (i >= NB_POINT)
		return i - NB_POINT;
	return i;
}

#define IC2(i)	((i)<0 ? (i)+NB_POINT : (((i)>=NB_POINT)? (i)-NB_POINT: (i)))

//Copie LP source -> LP dest
INLINE char lp_cpy(LIDAR_POINT * source, LIDAR_POINT * dest) {
	dest->d = source->d;
	dest->theta = source->theta;
	dest->etat = source->etat;
	dest->x = source->x;
	dest->y = source->y;
	dest->id = source->id;
	return 0;
}

//Copie LP source [] -> LP dest []
char HL_cpy(LIDAR_POINT * source, LIDAR_POINT * dest) {
	for (size_t j = 0; j < NB_POINT; j++)
		lp_cpy(&source[j], &dest[j]);
	return 0;
}


//Retourne la distance entre deux points
unsigned int lp_distance(LIDAR_POINT * A, LIDAR_POINT *B) {
	return (unsigned int)sqrt((A->x - B->x)*(A->x - B->x) + (A->y - B->y)*(A->y - B->y));
}