//
//  lootdrops.h
//  StandAlone
//
//  Created by Gian Salay on 12/8/15.
//  Copyright (c) 2015 NadaNazis. All rights reserved.
//

#ifndef __StandAlone__lootdrops__
#define __StandAlone__lootdrops__

#include "entity.h"

class lootdrops : public entity
{
public:
	//If RNGod is kind this will be true
	bool drop = true;

	//WEAPON BOOLEANS
	//These will turn true if player steps on appropriate loot drop
	//e.g. player steps on shotgun sprite, shotgun = true
	bool hmg = false;
	bool shotgun = false;
	bool rifle = false;


	bool erase = false;

	lootdrops();
	void update();
	void updateMove();

};


#endif /* defined(__StandAlone__lootdrops__) */
