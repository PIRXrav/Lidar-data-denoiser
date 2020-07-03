#include "TABLE_COSIN.h"
#include "HL_traitement.h"

#define LIDAR_POINT_ERROR_D 1048575
#define PT_MIN_SCAN 1000

void pol_to_XY(LIDAR_POINT* point, int px, int py, float phi);


//Il faut supprimer les erreures de mesure induqué par le LIDAR : LIDAR_POINT_ERROR_D == d
char HL_mise_0_etat_mesures_fausse(LIDAR_POINT * tab) {
	signed int cpt = 0;
	for (int i = 0; i < NB_POINT; i++)
		if (LIDAR_POINT_ERROR_D == tab[i].d) 
			tab[i].etat = 0;
		else 
			cpt++;

	if (cpt <= PT_MIN_SCAN) 
		return -1;

	return 0;
}

char HL_check_theta(LIDAR_POINT * tab) {
	for (size_t i = 0; i < NB_POINT;i++)
		if ((tab[i].theta - ((float)i * 2 * PI / NB_POINT) > 0.1))//Erreur
			return -1;
	return 0;
}


void pol_to_XY(LIDAR_POINT *point, int px, int py, float phi) {
	if (couleur == JAUNE) {
		point->x = (int)(-cos(point->theta - phi)*point->d) + px; 
		point->y = (int)(sin(point->theta - phi)*point->d) + py;

	}
	else if (couleur == VIOLET) {
		point->x = (int)(cos((point->theta) + phi)*point->d) + px; 
		point->y = (int)(sin((point->theta) + phi)*point->d) + py;
	}
}

#define ANGLE_MAX_CST NB_POINT
#define ICXY(angle)	((angle)<0 ? (angle)+ANGLE_MAX_CST : (((angle)>=ANGLE_MAX_CST)? (angle)-ANGLE_MAX_CST: (angle)))

/*
*	Calcul les cordonnées cartesienne du point :
*		px : signed int (mm)
*		py : signed int (mm)	
*		phi : signed int °/10 : [-1800;1800]
*/
/*
INLINE void pol_to_XY_TABLES(LIDAR_POINT *point, int px, int py, signed int phi) {
	point->x = (signed int)(COS_TABLE[ICXY(point->id + phi)] * (float)point->d) + px;
	point->y = (signed int)(SIN_TABLE[ICXY(point->id + phi)] * (float)point->d) + py;
}
*/
//public

void HL_CALCXY(LIDAR_POINT * tab, int px, int py, float phi) {
	size_t i;
	for (i = 0; i < NB_POINT; i++)
		pol_to_XY(&tab[i], px, py, phi);
}


void HL_draw(LIDAR_POINT * tab, COULEUR c) {
	size_t i;
	for (i = 0; i < NB_POINT; i++) {
		if (tab[i].etat)
			fenetre.draw_pt(tab[i].x, tab[i].y, c);
	}
}


#include <time.h> 
#include "HL_traitement.h"
