#include <iostream>
#include <fstream>
#include <assert.h>
#include <assert.h>
#include "lire_fichier_data.h"

static MODE internal_mode;
static LIDAR_POINT **internal_LP;
static int internal_xRobotTab[NB_TOUR];
static int internal_yRobotTab[NB_TOUR];


static char barre_de_chargement(int i, int imax) {
	float progress = (float)i / imax;
	int barWidth = 50;
	std::cout << "[";
	int pos = (int)(barWidth * progress);
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << "[" << int(i) << "/" << int(imax) << "]  " << int(progress * 100.0) << " %\r";
	std::cout.flush();
	return 0;
}

static LIDAR_POINT ** lp_alloc(void) {
	LIDAR_POINT ** tab_test = (LIDAR_POINT **)malloc(NB_TOUR * sizeof(LIDAR_POINT **));
	if (tab_test != NULL)
		for (size_t j = 0; j < NB_TOUR; j++)
			assert(tab_test[j] = (LIDAR_POINT*)malloc(NB_POINT * sizeof(LIDAR_POINT)));
	return tab_test;
}


void lireFichierGen(MODE mode) {
	internal_LP = lp_alloc();
	int poubelle, poubelle2;
	std::ifstream fichier(namefile[(size_t)mode], std::ios::in);
	assert(fichier);
	for (int i = 0; i < NB_TOUR; i++) {
		for (int j = 0; j < NB_POINT; j++) {
			switch (mode) {
				case V1:
					fichier >> poubelle >> internal_LP[i][j].d >> internal_LP[i][j].theta;
					internal_LP[i][j].theta = (internal_LP[i][j].theta * 2 * PI) / 360.0f;
					
				case V2:
				case BOUTBOUT:
				case RECALAGE_A:
				case RECALAGE_B:
				case PR:
					fichier >> poubelle >> poubelle2 >> internal_LP[i][j].d >> internal_LP[i][j].theta;
					internal_LP[i][j].theta = internal_LP[i][j].theta;
				break;
				default:
					exit(1);
				break;
				internal_LP[i][j].etat = 1;
				internal_LP[i][j].id = j;
			}
		}
		barre_de_chargement(i, NB_TOUR);
	}
	fichier.close();

}

/*Interface*/

LIDAR_POINT * get_scan(size_t index) {
	return internal_LP[index];
}
