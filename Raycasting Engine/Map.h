#pragma once

#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "FPS_Clock.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Map {
private:
	sf::RenderWindow * window;
	FPS_Clock clock;

	const float PI = 3.141592653f;
	const int BLOCK_SIZE = 20;
	const int FIELD_OF_VIEW = 60;
	static const int MAP_WIDTH = 20;
	static const int MAP_HEIGHT = 20;

	int mapLayout[MAP_HEIGHT][MAP_WIDTH] = {
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1 },
		{ 1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1 },
		{ 1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
	};
	float x = 40.f;
	float y = 40.f;
	float angle = 90.f;

	void drawLine(float x_P, float localRayAngle_P, float rayLength_P, std::string blockSide_P);
	float rayXOffset_F(float rayX_P, int xOffsetMultiplier_P);
	float rayYOffset_F(float rayY_P, int yOffsetMultiplier_P);
	float xOffsetDistance_F(float xOffset_P, float rayAngle_P);
	float yOffsetDistance_F(float yOffset_P, float rayAngle_P);

public:
	Map(sf::RenderWindow * window_P);
	void move();
	void drawBackground();
	void drawWalls();
	void drawMinimap();
};