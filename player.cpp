//
//  player.cpp
//  StandAlone
//
//  Created by Gian Salay on 12/2/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#include "player.h"


player::player()
{

	rect.setSize(sf::Vector2f(30, 16));
	rect.setFillColor(sf::Color::Blue);
	rect.setOrigin(15, 8);
	rect.setPosition(400, 200);
}

void player::update()
{
	sprite.setOrigin(15, 8);
	sprite.setPosition(rect.getPosition());

}
void player::updateMove()
{
	sprite.setTextureRect(sf::IntRect(0, 0, 30, 16));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))

	{
		if (rect.getPosition().y >0 + player_height)
			rect.move(0, -movementSpeed);

		sprite.setTextureRect(sf::IntRect(counterWalking * 30, 0, 30, 16));

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))

	{
		if (rect.getPosition().y <SCREEN_HEIGHT - player_height - 85)
			rect.move(0, movementSpeed);
		sprite.setTextureRect(sf::IntRect(counterWalking * 30, 0, 30, 16));

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))

	{
		if (rect.getPosition().x <SCREEN_WIDTH - player_width / 2)
			rect.move(movementSpeed, 0);
		sprite.setTextureRect(sf::IntRect(counterWalking * 30, 0, 30, 16));

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))

	{
		if (rect.getPosition().x >0 + player_width / 2)
			rect.move(-movementSpeed, 0);
		sprite.setTextureRect(sf::IntRect(counterWalking * 30, 0, 30, 16));
	}

	if (sprite_timer.getElapsedTime().asSeconds() > 0.2)
	{
		counterWalking++;
		sprite_timer.restart();
	}

	if (counterWalking == 3)
	{
		counterWalking = 1;
	}
}

void player::updateRotation(sf::Vector2i mouse)
{
	mcPos = sprite.getPosition();

	a = mouse.x - mcPos.x;

	b = mouse.y - mcPos.y;

	angle = -atan2(a, b) * 180 / 3.14;

	sprite.setRotation(angle);
	rect.setRotation(angle);

}