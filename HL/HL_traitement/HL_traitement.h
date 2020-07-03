#pragma once
#include "../HL_LP.h"

void HL_draw(LIDAR_POINT * tab, COULEUR c);

char HL_mise_0_etat_mesures_fausse(LIDAR_POINT * tab);

char HL_check_theta(LIDAR_POINT * tab);

void HL_CALCXY(LIDAR_POINT * tab, int px, int py, float phi);
