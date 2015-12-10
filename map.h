#ifndef __StandAlone__map__
#define __StandAlone__map__

#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<cctype>
#include <string>
#include <vector>
#include <sstream>


sf::Texture tileTexture;
sf::Sprite tiles;

std::vector<std::vector<sf::Vector2i>> map;
std::vector<std::vector<int>> colMap;




#endif