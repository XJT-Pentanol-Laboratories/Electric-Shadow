#ifndef ENEMIES_H
#define ENEMIES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Enemy {
	uint16_t id;
	char name[32];
	bool distinguished;
	int8_t uid;
	char d_name[36];
	char desc[128];
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
} enemy;

void init_enemy(enemy* e, const uint16_t id);



#endif