#pragma once
#include "../../HL/HL_LP.h"

#define NB_TOUR 500

#define CONFIG	C(V1)				\
				C(V2)				\
				C(RECALAGE_A)		\
				C(BOUTBOUT)			\
				C(RECALAGE_B)		\
				C(PR)				

/*Abstraction*/
#define C(x) x,
typedef enum MODE { CONFIG TOP }MODE;
#undef C
#define C(x) #x##".txt",
const char * const namefile[] = { CONFIG };

void lireFichierGen(MODE mode);
LIDAR_POINT * get_scan(size_t index);
