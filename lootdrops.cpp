//
//  lootdrops.cpp
//  StandAlone
//
//  Created by Gian Salay on 12/8/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#include "lootdrops.h"


lootdrops::lootdrops()
{

	rect.setSize(sf::Vector2f(30, 16));
	rect.setFillColor(sf::Color::Blue);
	rect.setOrigin(15, 8);
	rect.setPosition(400, 200);
}

void lootdrops::update()
{
	sprite.setOrigin(15, 8);
	sprite.setPosition(rect.getPosition());

}
