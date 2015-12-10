//
//  player.h
//  StandAlone
//
//  Created by Gian Salay on 12/2/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#ifndef __StandAlone__player__
#define __StandAlone__player__

#include "entity.h"

class player : public entity
{
public:
	sf::Clock sprite_timer;
	int player_width = 30;
	int player_height = 16;
	int SCREEN_HEIGHT = 800;
	int SCREEN_WIDTH = 1024;
	int movementSpeed = 2;
	int hp = 100;
	int score = 0;
	float angle;

	float gun_knockback = 20;
	double a, b;
	sf::Vector2f mcPos;

	/* WEAPON LOG
	0 - HANDGUN
	1 - HEAVY MACHINE GUN
	2 - SHOTGUN
	3 - RIFLE
	*/

	int weapon_held = 0;

	//WEAPON PARAMETERS
	int ammo = 0;
	float FIRE_RATE = 0.5;

	//SPRITE COUNTER
	int counterWalking = 0;

	player();
	void update();
	void updateMove();
	void updateRotation(sf::Vector2i mouse);
};

#endif /* defined(__StandAlone__player__) */
