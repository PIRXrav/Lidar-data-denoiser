#pragma once

//================================= IFDEFS ===============================================
#ifndef _WIN32
	#define _LIN 
#endif

#ifdef _WIN32
	//SFML uniquement sur WIN
	#define SFLM 1
	#define INLINE extern __forceinline
#else
	//#define INLINE __attribute__((always_inline)) 
	#define INLINE
#endif


//============================================= PING PONG =============================================
// PING PONG
#include <chrono>  // for high_resolution_clock
// Record start time
#define PING \
	auto start = std::chrono::high_resolution_clock::now();
//Record end time
#define PONG \
	auto finish = std::chrono::high_resolution_clock::now();\
	std::chrono::duration<double> elapsed = finish - start; \
	std::cout << "Elapsed time: " << elapsed.count()*1e3 << " ms\n";


#include "../LIB_PC/fenetre_SFML/c_fenetre.h"
#include <iostream>
#include <cstdint>
#include <string>
#include <thread>
#include <cmath>
#include <fstream>
#include <math.h>


//=== CARAC LIDAR === 
#define NB_POINT 3600
#define NB_POINT_MAX 84000
#define FREQ (NB_POINT_MAX / NB_POINT)


//=== CARAC TERRAIN ===
/*
  YMAX  +----------------------------------------+
		|                                        |
		|                                        |
		|                                        |
		|                                        |
		+------------+							 |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |	A (J)
		|            |                           +--+
		|            |                           |
		|            |                           |
		|            |                           |
		|Rampes      |                           +--+
		|            |                           |	A (V)
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		|            |                           |
		+------------+                           |
		|                                        |
		|                                        |
		|                                        |
		|                                        |
  YMIN  +----------------------------------------+
       XMIN                                    XMAX
*/

#define XMAX 2000
#define YMAX 3000
#define XMIN 0
#define YMIN 0

//Pilone
#define AXC (42)			//cote X pylone
#define AYC (128)			//coté Y pylone
#define AY (YMAX/2-AYC/2)
#define AX (XMAX + 12)

//Rampes
#define RAMPEXMIN 0
#define	RAMPEXMAX 457
#define RAMPEYMIN 450
#define RAMPEYMAX 2550

//Balnces
#define BXC 40
#define BYC 40
#define BX ((149 + 40 +44 +40))
#define BY (YMAX /2 - BYC/2 )

///trigonometrie
#define PI 3.14159265



// COULEUR 
#define JAUNE 1
#define VIOLET 2
extern unsigned char couleur;
unsigned char HL_init_couleur(unsigned char c);


typedef struct {
	signed int d;			//La distance
	float theta;			//L'angle du point repere lidar

	signed int x;			//x
	signed int y;			//y

	char etat;		//la valididité de ce point

	size_t id;		//L'index de ce point -> table sinus / cosinus
}LIDAR_POINT; // LP


INLINE size_t IC(signed int i);
#define IC2(i)	((i)<0 ? (i)+NB_POINT : (((i)>=NB_POINT)? (i)-NB_POINT: (i)))

INLINE char lp_cpy(LIDAR_POINT * source, LIDAR_POINT * dest);
char HL_cpy(LIDAR_POINT * source, LIDAR_POINT * dest);
unsigned int lp_distance(LIDAR_POINT * A, LIDAR_POINT *B);
