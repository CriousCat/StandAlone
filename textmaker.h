//
//  textmaker.h
//  StandAlone
//
//  Created by Gian Salay on 12/5/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#ifndef __StandAlone__textmaker__
#define __StandAlone__textmaker__

#include "entity.h"

class textmaker : public entity
{
public:
	sf::Clock text_timer;
	string notice = "null";
	int movementSpeed = 1;
	int counter = 0;
	int maxCounter = 100;
	bool erase = false;


	textmaker();
	void update();

};

#endif /* defined(__StandAlone__textmaker__) */
