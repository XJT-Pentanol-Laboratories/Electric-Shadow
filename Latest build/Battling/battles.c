#include "battles.h"

bool rng_initialised = false;

double random01() {
	if (!rng_initialised) {
		srand((unsigned int)time(NULL));
		rng_initialised = true;
	}
	return (double)rand() / ((double)RAND_MAX + 1.0);
}

bool chance(double c) {
	return random01() < c;
}

double random(double a, double b) {
	return a + random01() * (b - a);
}

int random_int(int a, int b) {
	return floor(random(a, b));
}

void init_battle(battle* b, uint16_t id) {

	uniquify_enemies(b);
}

enemy* get_enemy(battle* b, int8_t position) {
	return (enemy*)(b->enemies[position].battler);
}

player* get_player(battle* b, int8_t position) {
	return (player*)(b->players[position].battler);
}

void sort_speeds(battle* b) {
	// Insertion sort, but ehh i guess its only 20 elements so we should be fine
	int8_t sorted_length; // why do i keep using int8?????
	for (sorted_length = 0; sorted_length < 20; sorted_length++) { // i know, what an atrocity is this
		if (sorted_length < 5) {
			b->order[sorted_length] = &(b->players[sorted_length]);
		} else {
			b->order[sorted_length] = &(b->enemies[sorted_length - 5]);
		}
		int8_t i;
		for (i = sorted_length; i > 0; i--) {
			if (((player*)(b->order[i]->battler))->spd > ((player*)(b->order[i - 1]->battler))->spd) { // uhh... yay bigger atrocity?
				void *temp = b->order[i - 1];
				b->order[i - 1] = b->order[i];
				b->order[i] = temp;
			} else { break; }
		}
	}
}

battler* find_next_b(battle* b) {
	int8_t i;
	for (i = 0; i < 20; i++) {
		if (b->order[i]->type == BattlerTypeEnemy) {
			if (((enemy*)(b->order[i]->battler))->id == 0) {
				continue;
			}
		}
		if (!(b->order[i]->moved)) {
			return b->order[i];
		}
	}
	return NULL;
}

void uniquify_enemy(battle* b, int8_t position) {
	enemy* this_enemy = get_enemy(b, position);
	int8_t uid = -1;
	int8_t i;
	for (i = 0; i < 15; i++) {
		enemy* that_enemy = get_enemy(b, i);
		if (that_enemy != this_enemy) {
			if (that_enemy->id == this_enemy->id && that_enemy->uid > uid) {
				uid = that_enemy->uid;
			}
		}
	}
	uid++;
	this_enemy->uid = uid;
	if (uid > -1) {
		snprintf(this_enemy->d_name, sizeof(this_enemy->d_name), "%s %c", this_enemy->name, (uid + 65));
	} else if (this_enemy->distinguished) {
		strncpy(this_enemy->d_name, this_enemy->name, sizeof(this_enemy->d_name));
	} else {
		snprintf(this_enemy->d_name, sizeof(this_enemy->d_name), "The %s", this_enemy->name);
	}
}

void uniquify_enemies(battle* b) {
	int i;
	for (i = 0; i < 15; i++) {
		uniquify_enemy(b, i);
	}
}

void replace_enemy(battle* b, int8_t position, uint16_t id) {
	init_enemy(get_enemy(b, position), id);
	uniquify_enemies(b);
}

void max_hp(battler* br) {
	if (br->type == BattlerTypeEnemy) {
		((enemy*)(br->battler))->hp = ((enemy*)(br->battler))->max_hp;
		printf("%s's HP is maxed out!\n", ((enemy*)(br->battler))->d_name);
	} else {
		((player*)(br->battler))->hp = ((player*)(br->battler))->max_hp;
		printf("%s's HP is maxed out!\n", ((player*)(br->battler))->name);
	}
}

void full_heal(battler* br) {
	if (br->type == BattlerTypeEnemy) {
		if (br->dead) {
			printf("But %s was already gone.\n", ((enemy*)(br->battler))->d_name);
			return;
		}
		max_hp(br);
	} else {
		max_hp(br);
		if (br->dead) {
			printf("But %s has still not woken up...\n", ((player*)(br->battler))->name);
		}
	}
}

void heal(battler* br, int16_t hp) {
	if (br->type == BattlerTypeEnemy) {
		if (br->dead) {
			printf("But %s was already gone.\n", ((enemy*)(br->battler))->d_name);
			return;
		}
		((enemy*)(br->battler))->hp += hp;
		if (((enemy*)(br->battler))->hp >= ((enemy*)(br->battler))->max_hp) {
			full_heal(br);
		} else {
			printf("%s recovered %d HP!\n", ((enemy*)(br->battler))->d_name, hp);
		}
	} else {
		((player*)(br->battler))->hp += hp;
		if (((player*)(br->battler))->hp >= ((player*)(br->battler))->max_hp) {
			full_heal(br);
		} else {
			printf("%s recovered %d HP!\n", ((player*)(br->battler))->name, hp);
			if (br->dead) {
				printf("But %s has still not woken up...\n", ((player*)(br->battler))->name);
			}
		}
	}
}

void damage(battler* br, int16_t hp) {
	if (br->type == BattlerTypeEnemy) {
		if (br->dead) {
			printf("But %s was already gone.\n", ((enemy*)(br->battler))->d_name);
			return;
		}
		((enemy*)(br->battler))->hp -= hp;
		if (((enemy*)(br->battler))->hp > 0) {
			printf("%d HP of damage to %s!\n", hp, ((enemy*)(br->battler))->d_name);
		} else {
			die(br);
		}
	} else {
		if (br->dead) {
			printf("But %s was already gone.\n", ((player*)(br->battler))->name);
			return;
		}
		((player*)(br->battler))->hp -= hp;
		if (((player*)(br->battler))->hp > 0) {
			printf("%d HP of damage to %s!\n", hp, ((player*)(br->battler))->name);
		} else {
			die(br);
		}
	}
}

void die(battler* br) {
	if (br->type == BattlerTypeEnemy) {
		if (br->dead) {
			printf("But %s was already gone.\n", ((enemy*)(br->battler))->d_name);
			return;
		}
		((enemy*)(br->battler))->hp = 0;
		br->dead = true;
		printf("You defeated %s!\n", ((enemy*)(br->battler))->d_name);
	} else {
		if (br->dead) {
			printf("But %s was already gone.\n", ((player*)(br->battler))->name);
			return;
		}
		((player*)(br->battler))->hp -= 0;
		br->dead = true;
		printf("%s got hurt and fell...\n", ((player*)(br->battler))->name);
	}
}


