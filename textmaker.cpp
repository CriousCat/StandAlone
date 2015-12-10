//
//  textmaker.cpp
//  StandAlone
//
//  Created by Gian Salay on 12/5/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#include "textmaker.h"

textmaker::textmaker()
{

	text.setColor(sf::Color::White);
	text.setCharacterSize(20);
	// text.setPosition(0,0);
	text.setString(notice);
}

void textmaker::update()
{
	//sf::Clock text_timer;
	text.move(0, -movementSpeed);
	counter++;
	if (text_timer.getElapsedTime().asSeconds() >= 0.4)
	{
		erase = true;
		text_timer.restart();
	}
}
