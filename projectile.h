//
//  Projectile.h
//  StandAlone
//
//  Created by Gian Salay on 12/2/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#ifndef __StandAlone__Projectile__
#define __StandAlone__Projectile__

#include "entity.h"

class projectile : public entity
{
public:
	sf::Clock sprite_timer;
	int movementSpeed = 1;
	int attack = 5;
	bool isAlive = true;
	float angle;
	double a, b;
	int SCREEN_WIDTH = 800;
	int SCREEN_LENGTH = 1024;
	bool rifleshot = false;
	bool rotated = false;
	sf::Vector2f mcPos;
	sf::Vector2i mouse;


	float velX, velY;

	int counterWalking = 0;

	projectile();
	void update();
	void updateMove();
	void updateRotation();
};


#endif /* defined(__StandAlone__Projectile__) */
