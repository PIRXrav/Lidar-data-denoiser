
/*====================================================================================================
				 ______ _____ _   _______ _____            _____ ______
				|  ____|_   _| | |__   __|  __ \     /\   / ____|  ____|
				| |__    | | | |    | |  | |__) |   /  \ | |  __| |__
				|  __|   | | | |    | |  |  _  /   / /\ \| | |_ |  __|
				| |     _| |_| |____| |  | | \ \  / ____ \ |__| | |____
				|_|    |_____|______|_|  |_|  \_\/_/    \_\_____|______|

======================================================================================================*/

/*
	Explication du filtrage :
		- On filtre le signal en supprimant les fronts -> LE PB fonctionne bien
		- A la reconstruction, nous rajontons les fronts


	Details :
		Le traitement est réalisé sur le scan bouclé sur lui même.
		Cela permet de supprimer la discontinuité qui apparait 2pi.

								 A ^ d(A)
								,,,,,,,
							*@@#       *@@*
						  @@*             .@@
						#@                   @#
					   &&                     &&
					  .@                       @.		Représentation polaire
					  %%                       %%
					  %&                       %%
					   @                       @
					   *@                     @#
						*@                   @#
						  #@#             #@#
							 #@@&%,,,%&@@#



		|-------------------------|-------------------------|
		A1						  A2						A3		Représentation cart
					|--------------------------|
					B1						   B2

		Il y a une discontinuité entre d(A1) et d(A2) Lors du filtrage les oscillation de largeur NL aparaissent.
		On traite donc entre A1 et A3 et tronquons entre B1 et B2 de tel manière a ne pas prendre la
		discontinuité entre A1 et A3

		L'algorithme est optimisé pour DELTA (B1,A1) = NL/2
	
		|-------------------------|---------|
		A1						  A2					Représentation cart
			|--------------------------|
			B1						   B2

		//TODO : Réduire la bande passante -> Augmenter l'ordre car fc config = 0"Hz"

*/

#include "HL_filter.h"

char derive(int * gd, size_t size_gd, int * d_dd) {
	for (size_t i = 0; i < size_gd; i++) {
		d_dd[i] = gd[(i + 1) % size_gd] - gd[(i)];
	}
	return 0;
}

//Au dessus de ce seuil, la discontinuité est annulé ! Le seuil le plus important ! 
#define SEUIL_D_DD 100
char extract_PIc(int * gd, size_t size_gd, int * d_dd, int * d_dd2) {
	for (size_t i = 0; i < size_gd; i++) {
		if (abs(d_dd[i]) < SEUIL_D_DD)
			d_dd2[i] = d_dd[i];
		else
			d_dd2[i] = 0;
	}
	return 0;
}

char ecart_PIc(int * gd, size_t size_gd, int * d_dd, int * d_dd2, int * diff_d) {
	for (size_t i = 0; i < size_gd; i++) {
		diff_d[i] = -(d_dd2[i] - d_dd[i]);
	}
	return 0;
}

char cumsum(int * gd, size_t size_gd, int * d_dd2, int *dd_r) {
	dd_r[0] = d_dd2[0];
	for (size_t i = 1; i < size_gd; i++) {
		dd_r[i] = dd_r[i - 1] + d_dd2[i];
	}
	return 0;
}


#define SINC100 0
#define EQUIR29 1
#define EQUIR31 2
#define EQUIR32 3
#define EQUIR33 4

#define CHOIX_FILTRE EQUIR33

#if CHOIX_FILTRE == SINC100
#define PRECISION 16
#define NL 101
const int AN[NL] = {
		0,      0,      1,      2,      2,      0,     -5,    -11,    -13,
	   -7,      9,     27,     37,     29,      0,    -39,    -70,    -71,
	  -32,     36,    104,    133,     97,      0,   -121,   -205,   -200,
	  -87,     96,    269,    336,    241,      0,   -292,   -493,   -478,
	 -209,    231,    651,    827,    606,      0,   -786,  -1398,  -1453,
	 -696,    878,   2985,   5116,   6698,   7282,   6698,   5116,   2985,
	  878,   -696,  -1453,  -1398,   -786,      0,    606,    827,    651,
	  231,   -209,   -478,   -493,   -292,      0,    241,    336,    269,
	   96,    -87,   -200,   -205,   -121,      0,     97,    133,    104,
	   36,    -32,    -71,    -70,    -39,      0,     29,     37,     27,
		9,     -7,    -13,    -11,     -5,      0,      2,      2,      1,
		0,      0
};
// Quelle belle forme de sinus cardinal dans la matrice :)
// Matlab donne une précision de +1
#endif

#if CHOIX_FILTRE == EQUIR29
#define PRECISION 16
const int NL = 30;
const int AN[NL] = {
	 -312,     70,    233,    381,    373,    112,   -376,   -905,  -1170,
	 -856,    208,   1941,   3987,   5813,   6889,   6889,   5813,   3987,
	 1941,    208,   -856,  -1170,   -905,   -376,    112,    373,    381,
	  233,     70,   -312
};

#endif

#if CHOIX_FILTRE == EQUIR31
//Small BP
const int NL = 32;
const int AN[32] = {
	  219,    133,     86,    -39,   -233,   -453,   -628,   -667,   -484,
	  -25,    716,   1681,   2750,   3765,   4561,   4997,   4997,   4561,
	 3765,   2750,   1681,    716,    -25,   -484,   -667,   -628,   -453,
	 -233,    -39,     86,    133,    219
};
#endif

#if CHOIX_FILTRE == EQUIR32
//50 -> 300 Hz
const int NL = 38;
const int AN[38] = {
	  218,    -15,    -66,   -144,   -236,   -324,   -383,   -388,   -313,
	 -138,    146,    537,   1018,   1559,   2119,   2650,   3102,   3431,
	 3605,   3605,   3431,   3102,   2650,   2119,   1559,   1018,    537,
	  146,   -138,   -313,   -388,   -383,   -324,   -236,   -144,    -66,
	  -15,    218
};
#endif


#if CHOIX_FILTRE == EQUIR33
//0 -> 250 Hz ha ha Fc config = 0Hz
const int NL = 38;
const int AN[38] = {
	  218,    -15,    -66,   -144,   -236,   -324,   -383,   -388,   -313,
	 -138,    146,    537,   1018,   1559,   2119,   2650,   3102,   3431,
	 3605,   3605,   3431,   3102,   2650,   2119,   1559,   1018,    537,
	  146,   -138,   -313,   -388,   -383,   -324,   -236,   -144,    -66,
	  -15,    218
};
#endif

char filtrage(size_t size_gd, int *dd_r, int *dd_rf) {

	for (size_t i = 0; i < size_gd; i++) {
		for (size_t n = 0; n < NL; n++) {
			static size_t indice;
			indice = (size_gd + (int)i - (int)n + NL / 2) % size_gd;
			dd_rf[i] += (dd_r[indice])*AN[n]; //hé hé
		}
		dd_rf[i] = dd_rf[i] >> 15;
	}
	return 0;
}


#define MARGE_A1B1 (NL/2)
#define NB_POINT_NEW_OPTIMISATION (NB_POINT + 2*MARGE_A1B1) //*2 pour droite gauche

char HL_filtragenum(LIDAR_POINT * lp_tour_actuel) {

	int good_d[NB_POINT_NEW_OPTIMISATION] = { 0 };
	size_t index_hash[NB_POINT_NEW_OPTIMISATION];
	size_t good_tab_size = 0;


	for (size_t i = 0;i < NB_POINT_NEW_OPTIMISATION;i++) {
		if (lp_tour_actuel[i%NB_POINT].etat) {
			good_d[good_tab_size] = lp_tour_actuel[i%NB_POINT].d;
			index_hash[good_tab_size] = i% NB_POINT;
			good_tab_size++;
		}
	}
	if (good_tab_size < 10) {
		return -1;
	}

	static int d_dd[NB_POINT_NEW_OPTIMISATION];
	//Calcule d_dd
	derive(good_d, good_tab_size, d_dd);

	//Derive sans les PIc
	static int d_dd2[NB_POINT_NEW_OPTIMISATION];
	extract_PIc(good_d, good_tab_size, d_dd, d_dd2);

	//ecart PIc 
	static int diff_d[NB_POINT_NEW_OPTIMISATION];
	ecart_PIc(good_d, good_tab_size, d_dd, d_dd2, diff_d); // Retoune l'oppose du diff_d matlab

	//reconstruction sans les PIc
	static int dd_r[NB_POINT_NEW_OPTIMISATION];
	cumsum(good_d, good_tab_size, d_dd2, dd_r);

	//Filtrage de DD_R -> DD_RF
	static int dd_rf[NB_POINT_NEW_OPTIMISATION];
	filtrage(good_tab_size, dd_r, dd_rf);


	//reconstruction des PIcs
	static int s_diff_d[NB_POINT_NEW_OPTIMISATION];
	cumsum(good_d, good_tab_size, diff_d, s_diff_d);

	//Reconstruction avec décroisement index
	for (size_t i = MARGE_A1B1; i < good_tab_size- MARGE_A1B1; i++) {
		lp_tour_actuel[index_hash[i]].d		//Le tableau d'entrée
			= dd_rf[i]						//La distance filtré sans discontinuité
			+ s_diff_d[i]					//L'offset de distance integré
			+ good_d[0];					//La constance d'integration C		
	}


	return 0;
}


#define DVMAX 20

char HL_suppr_bord(LIDAR_POINT * tab) {

	size_t index_hash[NB_POINT];
	size_t index_hash_size = 0;
	for (size_t i = 0;i < NB_POINT;i++) {
		if (tab[i].etat) {
			index_hash[index_hash_size++] = i;
		}
	}

	//Pour tous les points bon du scan
	for (size_t i = 0;i < index_hash_size;i++) {
		if (abs(tab[index_hash[i]].d - tab[index_hash[(i + 1) % index_hash_size]].d) > DVMAX)
			tab[index_hash[i]].etat = 0;
	}
	return 0;
}
