//
//  zombie.h
//  StandAlone
//
//  Created by Gian Salay on 12/2/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#ifndef __StandAlone__zombie__
#define __StandAlone__zombie__

#include "entity.h"

class zombie : public entity
{
public:
	sf::Clock sprite_timer;
	int movementSpeed = 1;
	int damage = 5;
	int hp = 10;
	bool isAlive = true;
	float angle, movementAngle, Vx, Vy;
	double a, b;
	sf::Vector2f zombiePos;

	float z_width = 30;
	float z_height = 17;
	int SCREEN_HEIGHT = 800;
	int SCREEN_WIDTH = 1024;


	zombie();
	void update();
	void updateMove(sf::Vector2f mcPos);
	void updateHit();
	// void updateRotation();
};

#endif /* defined(__StandAlone__zombie__) */