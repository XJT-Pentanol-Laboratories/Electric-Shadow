#ifndef PLAYERS_H
#define PLAYERS_H

#include <stdint.h>


typedef struct Player {
	char name[16];
	int16_t max_hp;
	int16_t max_mp; // unused for now
	int16_t o_off;
	int16_t o_def;
	int16_t o_spd;
	int16_t o_acc;
	int16_t hp;
	int16_t mp; // also unused for now
	int16_t off;
	int16_t def;
	int16_t spd;
	int16_t acc;
} player;



#endif