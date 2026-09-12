#ifndef BATTLES_H
#define BATTLES_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "players.h"
#include "enemies.h"


// Type definitions

typedef enum BattlerType {
	BattlerTypePlayer,
	BattlerTypeEnemy
} battler_type;

typedef struct Battler {
	battler_type type;
	bool moved;
	bool dead;
	void* battler;
} battler;

typedef struct Battle {
	battler players[5];
	battler enemies[15];
	battler* order[20];
	bool over;
	bool won;
} battle;


// Math stuff

extern bool rng_initialised;

double random01();

bool chance(double c);

double random(double a, double b);

int random_int(int a, int b);


// General stuff

void init_battle(battle* b, uint16_t id);

enemy* get_enemy(battle* b, int8_t position);

player* get_player(battle* b, int8_t position);

void sort_speeds(battle* b);

battler* find_next_b(battle* b);


// Enemy functions

void uniquify_enemy(battle* b, int8_t position);

void uniquify_enemies(battle* b);

void replace_enemy(battle* b, int8_t position, uint16_t id);


// Script actions

void max_hp(battler* br);

void full_heal(battler* br);

void heal(battler* br, int16_t hp);

void damage(battler* br, int16_t hp);

void die(battler* br);


// Turn actions





#endif