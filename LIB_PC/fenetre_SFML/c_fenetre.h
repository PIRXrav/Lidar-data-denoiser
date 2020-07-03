#pragma once
#include "../../HL/HL_LP.h"
#include <SFML/Graphics.hpp>


///Fenetre
#define XFENETRE (2000*0.3)
//XScreen
#define YFENETRE (3000*0.3)
//Zoom :
#define ZOOM_SPEED  1.3f


//REAL -> Screen
#define X2S(x)((int)((x-XS))/REDU + XFENETRE/2)
#define Y2S(y)((int)((y-YS))/REDU + YFENETRE/2)
//Screen -> Real
#define RX2S(x)((x-XFENETRE/2)*REDU + XS)
#define RY2S(y)((y-YFENETRE/2)*REDU + YS)

#define TITREFENETRE "RCVA"
#define C_TERRAIN black


typedef struct {
	int x, y;
}POINTT;

//Couleurs
#include "couleur.h"
typedef uint32_t COULEUR;
typedef enum {
	no, lunary, solary
}STYLE;
COULEUR couleur_style(STYLE style, int x, int xmax);


class c_fenetre
{	
	//Internal
	private:
		sf::RenderWindow window;
		void MyZoom(sf::Vector2i pixel, float zoom_value);

		
		float REDU; //Reduction de la fenetre virtuelle
		int XS, YS; //Position de la fenetre virtuelle
	public:

		void ForceCamera(sf::Vector2f pos);

		
		//Utilities
		c_fenetre();
		sf::RenderWindow& get_fenetre();//Pour l'usage de fc native (A eviter)
		~c_fenetre();
		void pool_event(void);
		void actualiser();

		//MASTER FC : REAL -> SCREEN
		sf::Vector2f xys(int x, int y);

		//CIRC
		void draw_circle(sf::Vector2f p, int R, COULEUR c);
		void draw_circleREDU(sf::Vector2f p, int R, COULEUR c);
		//RECT
		void draw_rectangle(sf::Vector2f p1, sf::Vector2f p2, COULEUR c);
		void draw_rectanglewREDU(sf::Vector2f p1, sf::Vector2f p2, COULEUR c);

		//Affichage point HL
		void draw_pt(int, int, COULEUR);

		//SNIP
		void draw_sniper(int x, int y, COULEUR c);

		//TERRAIN
		void draw_terrain(void);

};

//BALAIZE CA !
extern c_fenetre fenetre;