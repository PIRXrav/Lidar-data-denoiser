
#include "HL/HL_LP.h"									/* Donne acces au LP */
#include "HL/HL_traitement/HL_traitement.h"				/* Utilitaire */
#include "HL/HL_filter/HL_filter.h"						/* Filtrage numérique */
#include "LIB_PC/lire_fichier_data/lire_fichier_data.h"	/* Prend ses données depuis un fichier */


int main(void){

	//Initialisation du programme
	HL_init_couleur(VIOLET);
	static size_t i_tour = 0;
	
	lireFichierGen(PR);

	while ((fenetre.get_fenetre()).isOpen()){
		// reception données (LIDAR ethernet) -> lp_tour_actuel[3600]
		static LIDAR_POINT lp_tour_actuel[NB_POINT];
		i_tour = (i_tour +1) % NB_TOUR;
		HL_cpy(get_scan(i_tour), lp_tour_actuel);

		// reception x/y (CORE uart)
		// Fic 6 PR
		int px_actuel = 1265 - 30;//-50;
		int py_actuel = 160 - 52;//+50;
		float phi_actuel = 0;
		
		//Traitement du tour 
		if (HL_mise_0_etat_mesures_fausse(lp_tour_actuel) == -1) {
			std::cout << "[HL] ERROR SCAN : PAS DE POINTS" << std::endl;
			return -1;
		}

		if (HL_check_theta(lp_tour_actuel) == -1) {
			std::cout << "[HL] ERROR SCAN : THETA NON VALIDE" << std::endl;
			return -1;
		}

		// Theta, Phi -> X, Y pour affichge
		HL_CALCXY(lp_tour_actuel, px_actuel, py_actuel, phi_actuel);
		HL_draw(lp_tour_actuel, blue);

		// Premier filtrage
		if (HL_filtragenum(lp_tour_actuel) == -1) {
			std::cout << "[HL] ERROR FILTRAGE : PAS DE POINTS" << std::endl;
			return -1;
		}
		HL_CALCXY(lp_tour_actuel, px_actuel, py_actuel, phi_actuel);
		HL_draw(lp_tour_actuel, red);

		// Segond filtrage : erreures de mesures : variations rapides
		HL_suppr_bord(lp_tour_actuel);
		HL_draw(lp_tour_actuel, orange);


		fenetre.draw_circleREDU(sf::Vector2f(px_actuel, py_actuel), 30, red);
		fenetre.draw_terrain();

		//Boucle d'evenements & double buffering
		fenetre.pool_event();
		fenetre.actualiser();
		
	}
	return 0;
}

