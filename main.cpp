
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

#include <iostream>
#include "player.h"
#include "entity.h"
#include "textmaker.h"
#include "projectile.h"
#include "zombie.h"
#include "lootdrops.h"
#include "map.h"

#define PI  3.141592
using namespace std;

int random(int x, int y)
{
	random_device random;
	uniform_int_distribution<int> distribution(x, y);
	int r = distribution(random);
	return r;
}

// Loads tile images and reads a text file which indicates the tile placement
void LoadMap(const char*filename)
{
	std::ifstream openfile(filename);
	std::vector<sf::Vector2i> tempMap;
	::map.clear();

	if (openfile.is_open() && !openfile.eof())
	{
		std::string str;
		std::getline(openfile, str);
		tileTexture.loadFromFile(str);
		tiles.setTexture(tileTexture);
		while (!openfile.eof())
		{
			std::getline(openfile, str);
			std::stringstream stream(str);
			std::string value;

			while (std::getline(stream, value, ' '))
			{
				if (value.length() > 0)
				{
					std::string xx = value.substr(0, value.find(','));
					std::string yy = value.substr(value.find(',') + 1);

					int x, y;
					x = stoi(xx.c_str());
					y = stoi(yy.c_str());

					tempMap.push_back(sf::Vector2i(x, y));

				}
			}
			if (tempMap.size() > 0)
			{
				::map.push_back(tempMap);
				tempMap.clear();
			}
		}
	}
}

// Reads a text file which indicates the areas that are passable by the player, enemies, and ammunition
void LoadColMap(const char*filename)
{
	std::ifstream openfile(filename);
	std::vector<int> tempMap;
	colMap.clear();

	if (openfile.is_open() && !openfile.eof())
	{
		std::string str;
		while (!openfile.eof())
		{
			std::getline(openfile, str);
			std::stringstream stream(str);
			std::string value;

			while (std::getline(stream, value, ' '))
			{
				if (value.length() > 0)
				{
					int a = atoi(value.c_str());
					tempMap.push_back(a);
				}
			}
			colMap.push_back(tempMap);
			tempMap.clear();
		}
	}
}

int main(int, char const**)
{
	// ======================================================
	//
	//                VARIABLE DECLARATIONS
	//
	// ======================================================

	enum State
	{
		STATE_PAUSED,
		STATE_RUNNING,
		STATE_GAMEOVER,
		STATE_WIN

	};
	State state_ = STATE_RUNNING;

	float playerSpeed = 2;
	int counter = 0;
	int counter2 = 0;

	int counterWalking = 1;

	sf::Vector2i mouse;


	float time_elapsed;

	// ======================================================
	//
	//                     SFML AND RESOURCE SETUP
	//
	// ======================================================

	//Load map tiles and tile locations
	LoadMap("map.txt");
	//Load collision map
	LoadColMap("col.txt");

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1024, 800), "Stand Alone");
	window.setPosition(sf::Vector2i(150, 0));

	// Set the Icon
	sf::Image icon;
	if (!icon.loadFromFile("pistol.png")) {
		return EXIT_FAILURE;
	}
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// ======================================================
	//
	//                     TEXTURES
	//
	// ======================================================
	sf::Texture texPlayer;
	if (!texPlayer.loadFromFile("mc.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture texZombie;
	if (!texZombie.loadFromFile("zombie.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture texBullet;
	if (!texBullet.loadFromFile("bullet.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture texLoot;
	if (!texLoot.loadFromFile("hmg.png")) {
		return EXIT_FAILURE;
	}
	sf::Sprite player(texPlayer);
	player.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	player.setTextureRect(sf::IntRect(0, 0, 30, 16));


	//Gun textures

	sf::Texture hmg_texture;
	if (!hmg_texture.loadFromFile("hmg.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture shotgun_texture;
	if (!shotgun_texture.loadFromFile("shotgun.png")) {
		return EXIT_FAILURE;
	}

	sf::Texture rifle_texture;
	if (!rifle_texture.loadFromFile("rifle.png")) {
		return EXIT_FAILURE;
	}
	// Create a graphical text to display
	sf::Font font;
	if (!font.loadFromFile("sansation.ttf")) {
		return EXIT_FAILURE;
	}
	sf::Text text("Hello SFML", font, 50);
	text.setColor(sf::Color::Black);

	// ======================================================
	//
	//                     SOUNDS
	//
	// ======================================================
	sf::Music music;
	if (!music.openFromFile("nice_music.ogg")) {
		return EXIT_FAILURE;
	}

	sf::SoundBuffer hitbuffer;
	if (!hitbuffer.loadFromFile("hit.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound hit;
	hit.setBuffer(hitbuffer);

	sf::SoundBuffer pistolbuffer;
	if (!pistolbuffer.loadFromFile("pistol.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound pistol;
	pistol.setBuffer(pistolbuffer);

	sf::SoundBuffer shotgunbuffer;
	if (!shotgunbuffer.loadFromFile("shotgun.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound shotgun;
	shotgun.setBuffer(shotgunbuffer);

	sf::SoundBuffer riflebuffer;
	if (!riflebuffer.loadFromFile("sniper.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound rifle;
	rifle.setBuffer(riflebuffer);

	sf::SoundBuffer reloadbuffer;
	if (!reloadbuffer.loadFromFile("reload.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound reload;
	reload.setBuffer(reloadbuffer);

	// ======================================================
	//
	//                     UI COMPONENTS
	//
	// ======================================================

	sf::RectangleShape healthbar;
	healthbar.setSize(sf::Vector2f(200, 30));
	healthbar.setPosition(10, 10);
	healthbar.setFillColor(sf::Color::Red);
	sf::RectangleShape inlinehealth;
	inlinehealth.setSize(sf::Vector2f(200, 30));
	inlinehealth.setPosition(10, 10);
	inlinehealth.setFillColor(sf::Color::Green);

	sf::RectangleShape outlinehealth;
	outlinehealth.setSize(sf::Vector2f(210, 40));
	outlinehealth.setPosition(5, 5);
	outlinehealth.setFillColor(sf::Color::White);


	// ======================================================
	//
	//                     OBJECTS
	//
	// ======================================================

	//Player object
	class player p1;
	p1.sprite.setTexture(texPlayer);
	p1.sprite.setTextureRect(sf::IntRect(0, 0, 30, 16));
	sf::Clock damage_clock;

	//PROJECTILES
	vector<projectile>::const_iterator iter;
	vector<projectile> bulletArray;
	class projectile bullet;
	bullet.sprite.setTexture(texBullet);
	sf::Clock shoot_clock;

	//ZOMBIES

	vector<zombie>::const_iterator iter2;
	vector<zombie>::const_iterator iter12;

	vector<zombie> zombieArray;
	/*
	class zombie zomby;
	zomby.sprite.setTexture(texZombie);
	zomby.sprite.setTextureRect(sf::IntRect(0,0,30,17));
	zombieArray.push_back(zomby);
	*/
	//TEXT
	vector<textmaker>::const_iterator iter3;
	vector<textmaker> textArray;
	class textmaker textdisplayer;
	textdisplayer.text.setPosition(300, 300);
	textdisplayer.text.setFont(font);


	//DROPS

	vector<lootdrops>::const_iterator iter4;
	vector<lootdrops> lootArray;

	// class lootdrops loot;
	// loot.sprite.setTexture(texLoot);
	// loot.sprite.setPosition(500, 500);
	// lootArray.push_back(loot);



	// ======================================================
	//
	//                      FRAME LIMIT
	//
	// ======================================================
	//FRAME LIMIT
	window.setFramerateLimit(60);


	/* sf::Vertex line[] =
	{
	sf::Vertex(sf::Vector2f(10, 10)),
	sf::Vertex(sf::Vector2f(150, 150))
	};
	*/
	sf::RectangleShape rifleshot(sf::Vector2f(1000, 2));

	// rifleshot.setOrigin(0, 0);

	int alpha = 0;
	//rifleshot.setFillColor(sf::Color(100,200,150,alpha));

	// Start the game loop
	while (window.isOpen())
	{

		// ======================================================
		//
		//                  WINDOW EVENTS
		//
		// ======================================================

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// Escape pressed: exit
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
			if (state_ != STATE_GAMEOVER)
			{
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
				{
					if (state_ == STATE_RUNNING)
						state_ = STATE_PAUSED;
					else
						state_ = STATE_RUNNING;
				}
			}
			else {
				//restart
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
				{
					healthbar.setSize(sf::Vector2f(200, 30));
					healthbar.setPosition(10, 10);
					p1.weapon_held = 0;
					p1.FIRE_RATE = 0.35;
					p1.ammo = -1;
					p1.hp = 100;
					p1.score = 0;
					p1.gun_knockback = 20;
					bullet.movementSpeed = 20;
					p1.rect.setPosition(400, 200);
					lootArray.clear();
					zombieArray.clear();
					textArray.clear();
					bulletArray.clear();
					state_ = STATE_RUNNING;
				}

			}
		}
		if (state_ == STATE_RUNNING)
		{

			// ======================================================
			//
			//                   STATUS UPDATES
			//
			// ======================================================

			if (p1.hp <= 0)
			{
				state_ = STATE_GAMEOVER;
			}
			if (p1.ammo == 0)
			{
				p1.weapon_held = 0;
				p1.FIRE_RATE = 0.35;
				p1.ammo = -1;
				p1.gun_knockback = 20;
				bullet.movementSpeed = 20;
			}


			//temporary win condition since levels don't work yet
			if(p1.score == 7500)
				state_ = STATE_WIN;


			// ======================================================
			//
			//                     ENEMY CONTROL
			//
			// ======================================================

			int levelSize = 5;
			//bool arrayFilled = false;
			if (p1.score >= 500 && p1.score < 1000)
			{
				levelSize += 5;
				//zombieArray.resize(levelSize);
				
			}
			if (p1.score >= 1000 && p1.score < 2000)
			{
				levelSize += 10;
				//zombieArray.resize(levelSize);
			}
			if (p1.score >= 2000 && p1.score < 3000)
			{
				levelSize += 15;
				//zombieArray.resize(levelSize);
			}
			if (p1.score >= 3000 && p1.score < 4000)
			{
				levelSize += 20;
				//zombieArray.resize(levelSize);
			}
			if (p1.score >= 4000 && p1.score < 5000)
			{
				levelSize += 25;
				//zombieArray.resize(levelSize);
			}
			if (p1.score >= 5000 && p1.score < 6000)
			{
				levelSize += 30;
				//zombieArray.resize(levelSize);
			}
			if (p1.score >= 6000 && p1.score < 7000)
			{
				levelSize += 35;
				//zombieArray.resize(levelSize);
			}
			if (p1.score >= 7000 && p1.score < 7500)
			{
				levelSize += 40;
				//zombieArray.resize(levelSize);
			}
			if (zombieArray.size() < levelSize)
			{
				class zombie z;
				if (random(0, 1) == 1)
				{
					z.sprite.setTexture(texZombie);
					z.sprite.setTextureRect(sf::IntRect(0, 0, 30, 17));
					z.movementSpeed = random(1, 2);
					zombieArray.push_back(z);
				}
				else
				{
					z.sprite.setTexture(texZombie);
					z.sprite.setTextureRect(sf::IntRect(0, 0, 30, 17));
					z.movementSpeed = random(1, 2);
					zombieArray.push_back(z);
				}
			}
			
			



			// Clear screen
			window.clear();

			//Place Tiles
			for (int i = 0; i < ::map.size(); i++)
			{
				for (int j = 0; j < ::map[i].size(); j++)
				{
					if (::map[i][j].x != -1 || ::map[i][j].y != -1)
					{
						tiles.setPosition(j * 32, i * 32);
						tiles.setTextureRect(sf::IntRect(::map[i][j].x * 32, ::map[i][j].y * 32, 32, 32));
						window.draw(tiles);
					}
				}
			}

			// ======================================================
			//
			//                         COLLISION
			//
			// ======================================================

			//PROJECTILE - ZOMBIE COLLISION
			counter = 0;
			for (iter = bulletArray.begin(); iter != bulletArray.end(); iter++)
			{
				counter2 = 0;

				for (iter2 = zombieArray.begin(); iter2 != zombieArray.end(); iter2++)
				{
					if (bulletArray[counter].rifleshot)
					{
						if (rifleshot.getGlobalBounds().intersects(zombieArray[counter2].rect.getGlobalBounds()))
						{
							hit.play();
							bulletArray[counter].isAlive = false;
							zombieArray[counter2].hp -= 15;
							//cout<<"hit "+to_string(zombieArray[counter2].hp)<<endl;
							zombieArray[counter2].rect.setPosition(
								zombieArray[counter2].sprite.getPosition().x -
								zombieArray[counter2].Vx*p1.gun_knockback,
								zombieArray[counter2].sprite.getPosition().y -
								zombieArray[counter2].Vy*p1.gun_knockback);
							//Change sprite to "HIT" sprite
							zombieArray[counter2].updateHit();

						}

					}
					else
					{
						if (bulletArray[counter].rect.getGlobalBounds().intersects(zombieArray[counter2].rect.getGlobalBounds()))
						{
							hit.play();
							//cout << "Collision" << endl;
							bulletArray[counter].isAlive = false;
							switch (p1.weapon_held)
							{
							case 0:
							{
								zombieArray[counter2].hp--;

								float offset;
								//KNOCKBACK
								zombieArray[counter2].rect.setPosition(
									zombieArray[counter2].sprite.getPosition().x -
									zombieArray[counter2].Vx*p1.gun_knockback,
									zombieArray[counter2].sprite.getPosition().y -
									zombieArray[counter2].Vy*p1.gun_knockback);
								break;
							}
							case 1:
							{
								zombieArray[counter2].hp -= 2;

								zombieArray[counter2].rect.setPosition(
									zombieArray[counter2].sprite.getPosition().x -
									zombieArray[counter2].Vx*p1.gun_knockback,
									zombieArray[counter2].sprite.getPosition().y -
									zombieArray[counter2].Vy*p1.gun_knockback);

								break;
							}
							case 2:
							{
								//PUT IN VECTOR CLASS PLS
								float magnitude = sqrt(pow(zombieArray[counter].sprite.getPosition().x - p1.sprite.getPosition().x, 2) + pow(zombieArray[counter].sprite.getPosition().y - p1.sprite.getPosition().y, 2));
								// cout<<magnitude<<endl;
								if (magnitude <= 100)
								{
									zombieArray[counter2].hp -= 5;
									// cout<<"5 damage" <<endl;
									zombieArray[counter2].rect.setPosition(zombieArray[counter2].sprite.getPosition().x - zombieArray[counter2].Vx*p1.gun_knockback, zombieArray[counter2].sprite.getPosition().y - zombieArray[counter2].Vy*p1.gun_knockback);
								}
								else
								{
									zombieArray[counter2].hp -= 2;
									// cout<<"2 damage"<<endl;
									zombieArray[counter2].rect.setPosition(zombieArray[counter2].sprite.getPosition().x - zombieArray[counter2].Vx*(p1.gun_knockback / 4), zombieArray[counter2].sprite.getPosition().y - zombieArray[counter2].Vy*(p1.gun_knockback / 4));
								}



								break;
							}
							case 3:
							{
								break;
							}

							}

							//Change sprite to "HIT" sprite
							zombieArray[counter2].updateHit();


						}


					}
					if (zombieArray[counter2].hp <= 0)
					{
						zombieArray[counter2].isAlive = false;
					}

					counter2++;
				}
				bullet.rifleshot = false;
				counter++;
			}

			//PLAYER - ZOMBIE COLLISION
			if (damage_clock.getElapsedTime().asSeconds() > 0.5)
			{
				damage_clock.restart();
				counter = 0;
				for (iter2 = zombieArray.begin(); iter2 != zombieArray.end(); iter2++)
				{
					if (p1.rect.getGlobalBounds().intersects(zombieArray[counter].rect.getGlobalBounds()))
					{
						p1.hp -= zombieArray[counter].damage;
						zombieArray[counter].rect.setPosition(
							zombieArray[counter].sprite.getPosition().x -
							zombieArray[counter].Vx * 30,
							zombieArray[counter].sprite.getPosition().y -
							zombieArray[counter].Vy * 30);
						healthbar.setSize(sf::Vector2f((p1.hp * 2 - (zombieArray[counter].damage) * 2), 30));
					}
					counter++;
				}
			}


			//ZOMBIE - ZOMBIE COLLISION

			counter = 0;
			for (iter2 = zombieArray.begin(); iter2 != zombieArray.end(); iter2++)
			{
				counter2 = 0;
				for (iter12 = zombieArray.begin(); iter12 != zombieArray.end(); iter12++)
				{
					if (iter2 != iter12)
					{
						if (zombieArray[counter].rect.getGlobalBounds().intersects(
							zombieArray[counter2].rect.getGlobalBounds()))
						{

							sf::Vector2f rectV1 = zombieArray[counter].rect.getPosition();

							sf::Vector2f rectV2 = zombieArray[counter2].rect.getPosition();
							//if zombie is to the left of another zombie
							if (rectV1.x <rectV2.x)
							{
								zombieArray[counter].rect.setPosition(
									zombieArray[counter].sprite.getPosition().x - 1, zombieArray[counter].sprite.getPosition().y);
								zombieArray[counter2].rect.setPosition(
									zombieArray[counter2].sprite.getPosition().x + 1, zombieArray[counter2].sprite.getPosition().y);

							}
							else
							{
								zombieArray[counter].rect.setPosition(
									zombieArray[counter].sprite.getPosition().x + 1, zombieArray[counter].sprite.getPosition().y);
								zombieArray[counter2].rect.setPosition(
									zombieArray[counter2].sprite.getPosition().x - 1, zombieArray[counter2].sprite.getPosition().y);

							}

							//if zombie is on top of another zombie
							if (rectV1.y <rectV2.y)
							{
								zombieArray[counter].rect.setPosition(
									zombieArray[counter].rect.getPosition().x, zombieArray[counter].rect.getPosition().y - 1);
								zombieArray[counter2].rect.setPosition(
									zombieArray[counter2].rect.getPosition().x, zombieArray[counter2].rect.getPosition().y + 1);

							}
							else
							{
								zombieArray[counter].rect.setPosition(
									zombieArray[counter].rect.getPosition().x, zombieArray[counter].rect.getPosition().y + 1);
								zombieArray[counter2].rect.setPosition(
									zombieArray[counter2].rect.getPosition().x, zombieArray[counter2].rect.getPosition().y - 1);

							}

						}
					}
					else
					{

					}
					counter2++;
				}
				counter++;
			}



			//PLAYER - LOOT COLLISION

			counter = 0;
			for (iter4 = lootArray.begin(); iter4 != lootArray.end(); iter4++)
			{
				if (p1.rect.getGlobalBounds().intersects(lootArray[counter].rect.getGlobalBounds()))
				{
					reload.play();

					//CHANGE WEAPON
					if (lootArray[counter].hmg == true)
					{
						p1.weapon_held = 1;


					}
					else if (lootArray[counter].shotgun == true)
					{
						p1.weapon_held = 2;

					}
					else if (lootArray[counter].rifle == true)
					{
						p1.weapon_held = 3;

					}

					//CHANGE PLAYER STATUS
					switch (p1.weapon_held)
					{
					case 0:
						p1.FIRE_RATE = 0.35;
						p1.ammo = -1;
						p1.gun_knockback = 20;
						bullet.movementSpeed = 20;
						break;
					case 1:
						p1.FIRE_RATE = 0.10;
						p1.ammo = 100;
						p1.gun_knockback = 20;
						bullet.movementSpeed = 20;
						break;
					case 2:
						p1.FIRE_RATE = 1;
						p1.ammo = 12;
						p1.gun_knockback = 160;
						bullet.movementSpeed = 20;
						break;
					case 3:
						p1.FIRE_RATE = 2;
						p1.ammo = 5;
						p1.gun_knockback = 50;
						bullet.movementSpeed = 35;

						break;
					}
					lootArray[counter].erase = true;
				}
				counter++;
			}

			// TILE MAP COLLISION
			for (int i = 0; i < colMap.size(); i++)
			{
				for (int j = 0; j < colMap[i].size(); j++)
				{
					if (colMap[i][j] == 1 || colMap[i][j] == 2)
					{
						sf::RectangleShape r1;
						r1.setSize(sf::Vector2f(32, 32));
						r1.setPosition(j * 32, i * 32);
						sf::Vector2f rectVm = r1.getPosition();

						// ZOMBIE - MAP COLLISION
						if (colMap[i][j] == 1)
						{
							counter2 = 0;

							for (iter2 = zombieArray.begin(); iter2 != zombieArray.end(); iter2++)
							{
								sf::Vector2f rectVz = zombieArray[counter2].rect.getPosition();
								if (zombieArray[counter2].rect.getGlobalBounds().intersects(r1.getGlobalBounds()))
								{
									if (rectVm.x < rectVz.x)
									{
										zombieArray[counter2].rect.setPosition(
											zombieArray[counter2].sprite.getPosition().x + 1, zombieArray[counter2].sprite.getPosition().y);

									}
									else
									{
										zombieArray[counter2].rect.setPosition(
											zombieArray[counter2].sprite.getPosition().x - 1, zombieArray[counter2].sprite.getPosition().y);
									}

									if (rectVm.y <rectVz.y)
									{
										zombieArray[counter2].rect.setPosition(
											zombieArray[counter2].rect.getPosition().x, zombieArray[counter2].rect.getPosition().y + 1);
									}
									else
									{
										zombieArray[counter2].rect.setPosition(
											zombieArray[counter2].rect.getPosition().x, zombieArray[counter2].rect.getPosition().y - 1);
									}
								}
								else
								{
								}
								counter2++;
							}
						}

						// PLAYER - MAP COLLISION
						if (colMap[i][j] == 1 || colMap[i][j] == 2)
						{

							sf::Vector2f rectVp = p1.rect.getPosition();
							if (p1.rect.getGlobalBounds().intersects(r1.getGlobalBounds()))
							{
								if (rectVm.x < rectVp.x)
								{
									p1.rect.setPosition(
										p1.sprite.getPosition().x + 1, p1.sprite.getPosition().y);

								}
								else
								{
									p1.rect.setPosition(
										p1.sprite.getPosition().x - 1, p1.sprite.getPosition().y);
								}

								if (rectVm.y <rectVp.y)
								{
									p1.rect.setPosition(
										p1.rect.getPosition().x, p1.rect.getPosition().y + 1);
								}
								else
								{
									p1.rect.setPosition(
										p1.rect.getPosition().x, p1.rect.getPosition().y - 1);
								}
							}
							else
							{
							}

						}

							//PROJECTILE/BULLET - MAP COLLISION
							counter = 0;
							for (iter = bulletArray.begin(); iter != bulletArray.end(); iter++)
							{
								if (bulletArray[counter].rect.getGlobalBounds().intersects(r1.getGlobalBounds()))
								{
									bulletArray.erase(iter);
									break;
								}
								counter++;
							}

						}
					}
				}
			

			// ======================================================
			//
			//                  GARBAGE COLLECTION
			//
			// ======================================================

			//ZOMBIE
			counter = 0;
			for (iter2 = zombieArray.begin(); iter2 != zombieArray.end(); iter2++)
			{
				if (zombieArray[counter].isAlive == false)
				{

					//DISPLAYS POINTS ON ZOMBIE DEATH
					textdisplayer.text.setString("100");
					textdisplayer.text.setPosition(zombieArray[counter].rect.getPosition().x - zombieArray[counter].rect.getSize().x / 2, zombieArray[counter].rect.getPosition().y - zombieArray[counter].rect.getSize().y / 2);
					textdisplayer.text_timer.restart();
					textArray.push_back(textdisplayer);

					//RANDOM LOOT WHEN DEAD
					class lootdrops drop;
					if (random(0, 4) == 0) //20% DROP RATE
					{
						int weapon = random(1, 3);
						switch (weapon)
						{
						case 1:
							drop.sprite.setTexture(hmg_texture);
							drop.hmg = true;


							break;
						case 2:
							drop.sprite.setTexture(shotgun_texture);
							drop.shotgun = true;

							break;
						case 3:
							drop.sprite.setTexture(rifle_texture);
							drop.rifle = true;

							break;
						}

						drop.rect.setPosition(zombieArray[counter].rect.getPosition());
						lootArray.push_back(drop);
					}
					p1.score += 100;
					zombieArray.erase(iter2);
					break;
				}
				counter++;
			}

			//PROJECTILE
			counter = 0;
			for (iter = bulletArray.begin(); iter != bulletArray.end(); iter++)
			{
				//check against game bounds

				if (bulletArray[counter].rect.getPosition().x >1000 || bulletArray[counter].rect.getPosition().x < 0 || bulletArray[counter].rect.getPosition().y <0 || bulletArray[counter].rect.getPosition().y > 800)
				{
					bulletArray[counter].isAlive = false;

				}
				if (bulletArray[counter].isAlive == false)
				{
					bulletArray.erase(iter);
					break;
				}
				counter++;
			}

			//TEXT
			counter = 0;
			for (iter3 = textArray.begin(); iter3 != textArray.end(); iter3++)
			{
				if (textArray[counter].erase == true)
				{

					textArray.erase(iter3);
					break;
				}
				counter++;
			}


			//LOOT
			counter = 0;
			for (iter4 = lootArray.begin(); iter4 != lootArray.end(); iter4++)
			{
				if (lootArray[counter].erase == true)
				{

					lootArray.erase(iter4);
					break;
				}
				counter++;
			}



			// ======================================================
			//
			//                       SHOOTING
			//
			// ======================================================

			if (shoot_clock.getElapsedTime().asSeconds() >p1.FIRE_RATE) //SHOT DELAY
			{
				shoot_clock.restart();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{

					//SOUND EFFECTS
					if (p1.weapon_held == 0 || p1.weapon_held == 1)
					{
						pistol.play();
					}
					else if (p1.weapon_held == 2)
					{
						shotgun.play();
					}
					else
					{
						rifle.play();
					}



					//SET BULLET POSITION TO SPRITE
					bullet.rect.setPosition(p1.rect.getPosition());

					//GET ANGLE OF SHOT(MOUSE POINT) WITH RESPECT TO PLAYER POSITION
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					float angleShot = atan2(mousePosition.y - bullet.rect.getPosition().y,
						mousePosition.x - bullet.rect.getPosition().x);

					//FOR ROTATION
					mouse = sf::Mouse::getPosition(window);
					bullet.mouse = mouse;

					// MACHINE GUN ACCURACY OFFSET
					if (p1.weapon_held == 1)
					{
						float offset_angle = (random(0, 10)*PI) / 180;
						if (random(0, 1) == 1)
						{
							bullet.velX = cos(angleShot + offset_angle);
							bullet.velY = sin(angleShot + offset_angle);
						}
						else {
							bullet.velX = cos(angleShot - offset_angle);
							bullet.velY = sin(angleShot - offset_angle);
						}
						bulletArray.push_back(bullet);
					}
					// RIFLE FASTER BULLET + PENETRATION
					else if (p1.weapon_held == 3)
					{
						bullet.velX = cos(angleShot);
						bullet.velY = sin(angleShot);
						rifleshot.setRotation((angleShot * 180) / PI);

						rifleshot.setPosition(p1.rect.getPosition());
						bullet.rifleshot = true;
						alpha = 150;
						bulletArray.push_back(bullet);
					}
					//PISTOL AND SHOTGUN "MAIN BULLET"
					else
					{
						bullet.velX = cos(angleShot);
						bullet.velY = sin(angleShot);
						bulletArray.push_back(bullet);
					}

					//SHOTGUN MULTIPLE BULLETS
					if (p1.weapon_held == 2)
					{
						float offset_angle = 0.174533;// 10 degress
						float offset_angle2 = 0.0872665;// 5 degrees in radians
						bullet.velX = cos(angleShot - offset_angle);
						bullet.velY = sin(angleShot - offset_angle);
						bulletArray.push_back(bullet);
						bullet.velX = cos(angleShot + offset_angle);
						bullet.velY = sin(angleShot + offset_angle);
						bulletArray.push_back(bullet);
						bullet.velX = cos(angleShot - offset_angle2);
						bullet.velY = sin(angleShot - offset_angle2);
						bulletArray.push_back(bullet);
						bullet.velX = cos(angleShot + offset_angle2);
						bullet.velY = sin(angleShot + offset_angle2);
						bulletArray.push_back(bullet);
					}

					// SUBTRACT AMMO
					if (p1.weapon_held != 0)
					{
						p1.ammo--;
					}


				}
			}

			// ======================================================
			//
			//                         DRAWING
			//
			// ======================================================

			//Draw Drops
			counter = 0;
			for (iter4 = lootArray.begin(); iter4 != lootArray.end(); iter4++)
			{

				lootArray[counter].update();
				window.draw(lootArray[counter].sprite);
				counter++;
			}

			//Draw Enemies
			counter = 0;
			for (iter2 = zombieArray.begin(); iter2 != zombieArray.end(); iter2++)
			{


				zombieArray[counter].update();
				zombieArray[counter].updateMove(p1.rect.getPosition());

				//return to normal sprite if 0.1 seconds have passed
				if (zombieArray[counter].sprite_timer.getElapsedTime().asSeconds() > 0.1)
				{
					zombieArray[counter].sprite.setTextureRect(sf::IntRect(0, 0, 30, 17));
				}

				window.draw(zombieArray[counter].sprite);
				//window.draw(zombieArray[counter].rect);

				counter++;
			}

			//Draw Projectiles
			counter = 0;
			for (iter = bulletArray.begin(); iter != bulletArray.end(); iter++)
			{


				bulletArray[counter].update();
				bulletArray[counter].updateMove();
				bulletArray[counter].updateRotation();
				window.draw(bulletArray[counter].sprite);

				counter++;
			}

			//Draw Texts
			counter = 0;
			for (iter3 = textArray.begin(); iter3 != textArray.end(); iter3++)
			{
				textArray[counter].update();
				window.draw(textArray[counter].text);
				counter++;
			}



			// ======================================================
			//
			//                         UPDATE MOVEMENT
			//
			// ======================================================



			//Update player
			p1.update();
			p1.updateMove();
			mouse = sf::Mouse::getPosition(window);
			p1.updateRotation(mouse);
			if ((sf::Mouse::isButtonPressed(sf::Mouse::Left)))
			{

				p1.sprite.setTextureRect(sf::IntRect(90, 0, 30, 25));
			}

			//Debugging Purposes
			// window.draw(p1.rect);

			// Draw the player
			window.draw(p1.sprite);


			// ======================================================
			//
			//                    INTERFACE DRAWING
			//
			// ======================================================


			if (p1.ammo == -1)
			{
				sf::Text ammotxt("Ammo: infinite", font, 30);
				ammotxt.setColor(sf::Color::White);
				ammotxt.setPosition(300, 10);
				window.draw(ammotxt);

			}
			else
			{
				sf::Text ammotxt("Ammo: " + to_string(p1.ammo), font, 30);
				ammotxt.setColor(sf::Color::White);
				ammotxt.setPosition(300, 10);
				window.draw(ammotxt);
			}


			sf::Text lol("Score: " + to_string(p1.score), font, 30);
			lol.setColor(sf::Color::White);
			lol.setPosition(800, 10);
			window.draw(lol);

			if (alpha > 0)
			{
				alpha -= 2;
				rifleshot.setFillColor(sf::Color(255, 255, 255, alpha));
				window.draw(rifleshot);
			}
			window.draw(outlinehealth);
			window.draw(inlinehealth);
			window.draw(healthbar);





			// Update the window
			window.display();
		}
		else if (state_ == STATE_PAUSED)
		{

			sf::Text paused("PAUSED", font, 100);
			paused.setColor(sf::Color::White);
			paused.setPosition(500, 400);
			window.draw(paused);
			window.display();
		}
		else if (state_ == STATE_GAMEOVER)
		{

			window.clear();
			sf::Text over("GAME OVER", font, 100);
			over.setColor(sf::Color::Red);
			over.setPosition(100, 400);
			window.draw(over);
			sf::Text scr("Your Score: " + to_string(p1.score), font, 50);
			scr.setColor(sf::Color::Red);
			scr.setPosition(100, 500);
			window.draw(scr);
			window.display();
		}
		else if (state_ == STATE_WIN)
		{

			window.clear();
			sf::Text win("YOU WIN", font, 100);
			//sf::Text scr("Your Score: " + to_string(p1.score), font, 50);
			win.setColor(sf::Color::Blue);
			//scr.setColor(sf::Color::Blue);
			win.setPosition(100, 400);
			//scr.setPosition(100, 500);
			window.draw(win);
			//window.draw(scr);
			window.display();
		}
	}

	return EXIT_SUCCESS;
}



