//
//  zombie.cpp
//  StandAlone
//
//  Created by Gian Salay on 12/2/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#include "zombie.h"

int chance(int x, int y)
{
	random_device random;
	uniform_int_distribution<int> distribution(x, y);
	int r = distribution(random);
	return r;
}
zombie::zombie()
{

	rect.setSize(sf::Vector2f(30, 17));
	rect.setFillColor(sf::Color::Blue);
	rect.setOrigin(15, 8.5);
	int location = chance(0, 3);
	switch (location)
	{
	case 0:
	{
		rect.setPosition(chance(200, 800), -50);
		break;
	}
	case 1:
	{
		rect.setPosition(SCREEN_WIDTH + 50, chance(200, 600));
		break;
	}
	case 2:
	{
		rect.setPosition(chance(200, 800), SCREEN_HEIGHT + 50);
		break;
	}
	case 3:
	{
		rect.setPosition(-50, chance(200, 600));
		break;
	}
	}

}

void zombie::update()
{
	sprite.setOrigin(15, 8.5);
	sprite.setPosition(rect.getPosition());

}


void zombie::updateMove(sf::Vector2f mcPos)
{
	zombiePos = sprite.getPosition();

	a = mcPos.x - zombiePos.x;

	b = mcPos.y - zombiePos.y;

	angle = -atan2(a, b) * 180 / 3.14;
	movementAngle = atan2(b, a);

	sprite.setRotation(angle);
	rect.setRotation(angle);

	Vx = cos(movementAngle);
	Vy = sin(movementAngle);

	rect.move(Vx*movementSpeed, Vy*movementSpeed);

}
void zombie::updateHit()
{
	sprite_timer.restart();
	sprite.setTextureRect(sf::IntRect(30, 0, 30, 17));


}