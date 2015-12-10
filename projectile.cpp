//
//  Projectile.cpp
//  StandAlone
//
//  Created by Gian Salay on 12/2/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#include "Projectile.h"

#include "math.h"



projectile::projectile()
{

	rect.setSize(sf::Vector2f(10, 5));
	rect.setFillColor(sf::Color::Yellow);
	rect.setOrigin(5, 2.5);

}
void projectile::update()
{
	sprite.setPosition(rect.getPosition());
}
void projectile::updateMove()
{
	rect.move(velX*movementSpeed, velY*movementSpeed);

}
void projectile::updateRotation()
{
	if (rotated == false)
	{
		mcPos = sprite.getPosition();


		a = mouse.x - mcPos.x;

		b = mouse.y - mcPos.y;

		angle = -atan2(a, b) * 180 / 3.14;

		sprite.setRotation(angle);
		rect.setRotation(angle);
		rotated = true;
	}
}
