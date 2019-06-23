#include "Map.h"

//Constructor
Map::Map(sf::RenderWindow * window_P) : window(window_P) {}

//Private
void Map::drawLine(float x_P, float localRayAngle_P, float rayLength_P, std::string blockSide_P) {
	float distance = rayLength_P * std::cos(std::abs(localRayAngle_P) * PI / 180);
	if (distance < 1) {
		distance = 1;
	}
	float height = float(SCREEN_HEIGHT) / (distance / 8.0f);

	sf::RectangleShape line(sf::Vector2f(1, height));
	line.setPosition(x_P, float(SCREEN_HEIGHT) / 2.0f - height / 2.0f);
	
	const int SHADE_MULTIPLIER = 100;
	if (distance < SHADE_MULTIPLIER) {
		distance = SHADE_MULTIPLIER;
	}
	if (blockSide_P == "VERTICAL") {
		line.setFillColor(sf::Color(0, 0, sf::Uint8(110.0f / (distance / float(SHADE_MULTIPLIER))), 255));
	}
	else if (blockSide_P == "HORIZONTAL") {
		line.setFillColor(sf::Color(0, 0, sf::Uint8(180.0f / (distance / float(SHADE_MULTIPLIER))), 255));
	}

	window->draw(line);
}
float Map::rayXOffset_F(float rayX_P, int xOffsetMultiplier_P) {
	float xOffset = rayX_P - (BLOCK_SIZE * std::floor(rayX_P / BLOCK_SIZE));

	if (xOffsetMultiplier_P == 1) {
		xOffset = BLOCK_SIZE - xOffset;
	}
	else if (xOffsetMultiplier_P == -1) {
		xOffset = -xOffset;
	}
	if (fmod(xOffset, BLOCK_SIZE) == 0) {
		xOffset = float(BLOCK_SIZE * xOffsetMultiplier_P);
	}

	return xOffset;
}
float Map::rayYOffset_F(float rayY_P, int yOffsetMultiplier_P) {
	float yOffset = rayY_P - (BLOCK_SIZE * std::floor(rayY_P / BLOCK_SIZE));

	if (yOffsetMultiplier_P == 1) {
		yOffset = BLOCK_SIZE - yOffset;
	}
	else if (yOffsetMultiplier_P == -1) {
		yOffset = -yOffset;
	}
	if (std::fmod(yOffset, BLOCK_SIZE) == 0) {
		yOffset = float(BLOCK_SIZE * yOffsetMultiplier_P);
	}

	return yOffset;
}
float Map::xOffsetDistance_F(float xOffset_P, float rayAngle_P) {
	return std::abs(xOffset_P / std::cos(rayAngle_P * PI / 180));
}
float Map::yOffsetDistance_F(float yOffset_P, float rayAngle_P) {
	return std::abs(yOffset_P / std::sin(rayAngle_P * PI / 180));
}

//Public
void Map::move() {
	clock.setCurrent();
	float fps = clock.getFPS();

	bool moveForward = false;
	bool moveBackward = false;
	bool rotateLeft = false;
	bool rotateRight = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveForward = true;
	}
	else {
		moveForward = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveBackward = true;
	}
	else {
		moveBackward = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		rotateRight = true;
	}
	else {
		rotateRight = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		rotateLeft = true;
	}
	else {
		rotateLeft = false;
	}

	if (angle >= 360) {
		angle -= 360;
	}
	else if (angle < 0) {
		angle += 360;
	}

	float moveSpeed = 1.2f * 50.0f / fps;
	float angleSpeed = 2.0f * 50.0f / fps;
	int newX;
	int newY;
	int newArrayX;
	int newArrayY;
	if (moveForward) {
		newX = int(x + moveSpeed * std::cos(angle * PI / 180));
		newY = int(y - moveSpeed * std::sin(angle * PI / 180));
		newArrayX = int(std::floor(newX / BLOCK_SIZE));
		newArrayY = int(std::floor(newY / BLOCK_SIZE));

		if (!mapLayout[newArrayY][newArrayX]) {
			x += moveSpeed * std::cos(angle * PI / 180);
			y -= moveSpeed * std::sin(angle * PI / 180);
		}
	}
	if (moveBackward) {
		newX = int(x - moveSpeed * std::cos(angle * PI / 180));
		newY = int(y + moveSpeed * std::sin(angle * PI / 180));
		newArrayX = int(std::floor(newX / BLOCK_SIZE));
		newArrayY = int(std::floor(newY / BLOCK_SIZE));

		if (!mapLayout[newArrayY][newArrayX]) {
			x -= moveSpeed * std::cos(angle * PI / 180);
			y += moveSpeed * std::sin(angle * PI / 180);
		}
	}
	if (rotateRight) {
		angle -= angleSpeed;
	}
	if (rotateLeft) {
		angle += angleSpeed;
	}
	clock.setPrevious();
}
void Map::drawBackground() {
	sf::RectangleShape line(sf::Vector2f(float(SCREEN_WIDTH), 1.0f));
	for (int i = 0; i < SCREEN_HEIGHT / 2; i++) {
		line.setFillColor(sf::Color(i / 8, i / 8, i / 8, 255));

		line.setPosition(0.0f, float(SCREEN_HEIGHT / 2 + i));
		window->draw(line);

		line.setPosition(0.0f, float(SCREEN_HEIGHT / 2 - i));
		window->draw(line);
	}
}
void Map::drawWalls() {
	float startingRayAngle = angle + FIELD_OF_VIEW / 2;

	bool canSee;
	int iterations;
	float rayAngle;
	float localRayAngle;
	float rayLength;
	float rayX;
	float rayY;
	int rayArrayX;
	int rayArrayY;

	int xOffsetMultiplier;
	int yOffsetMultiplier;

	float rayXOffset;
	float rayYOffset;
	float xOffsetDistance;
	float yOffsetDistance;

	std::string hitDirection;

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		canSee = true;
		iterations = 0;

		rayAngle = startingRayAngle - (FIELD_OF_VIEW / float(SCREEN_WIDTH)) * i;
		if (rayAngle >= 360) {
			rayAngle -= 360;
		}
		else if (rayAngle < 0) {
			rayAngle += 360;
		}
		localRayAngle = (FIELD_OF_VIEW / 2) - (FIELD_OF_VIEW / float(SCREEN_WIDTH)) * i;
		rayLength = 0;

		rayX = x;
		rayY = y;

		if (rayAngle >= 0 && rayAngle < 90) {
			xOffsetMultiplier = 1;
			yOffsetMultiplier = -1;
		}
		else if (rayAngle >= 90 && rayAngle < 180) {
			xOffsetMultiplier = -1;
			yOffsetMultiplier = -1;
		}
		else if (rayAngle >= 180 && rayAngle < 270) {
			xOffsetMultiplier = -1;
			yOffsetMultiplier = 1;
		}
		else if (rayAngle >= 270) {
			xOffsetMultiplier = 1;
			yOffsetMultiplier = 1;
		}

		while (true) {
			rayXOffset = rayXOffset_F(rayX, xOffsetMultiplier);
			rayYOffset = rayYOffset_F(rayY, yOffsetMultiplier);
			xOffsetDistance = xOffsetDistance_F(rayXOffset, rayAngle);
			yOffsetDistance = yOffsetDistance_F(rayYOffset, rayAngle);

			//Extend Rays
			if (xOffsetDistance < yOffsetDistance) {
				rayX += rayXOffset;
				rayY -= std::tan(rayAngle * PI / 180) * rayXOffset;
				rayLength += xOffsetDistance;
				hitDirection = "VERTICAL";

				if (std::fmod(rayX, BLOCK_SIZE) != 0) {
					rayX = std::round(rayX);
				}
			}
			else {
				rayX -= rayYOffset / std::tan(rayAngle * PI / 180);
				rayY += rayYOffset;
				rayLength += yOffsetDistance;
				hitDirection = "HORIZONTAL";

				if (std::fmod(rayY, BLOCK_SIZE) != 0) {
					rayY = std::round(rayY);
				}
			}

			rayArrayX = int(std::floor(rayX / BLOCK_SIZE));
			rayArrayY = int(std::floor(rayY / BLOCK_SIZE));
			if (xOffsetMultiplier == -1 && (hitDirection == "VERTICAL" || std::fmod(rayX, BLOCK_SIZE) == 0)) {
				rayArrayX--;
			}
			if (yOffsetMultiplier == -1 && (hitDirection == "HORIZONTAL" || std::fmod(rayY, BLOCK_SIZE) == 0)) {
				rayArrayY--;
			}
			if (rayArrayX < MAP_WIDTH && rayArrayY < MAP_HEIGHT && mapLayout[rayArrayY][rayArrayX]) {
				break;
			}

			//ERROR: Break Loop
			iterations++;
			if (iterations > 50) {
				std::cout << "ERROR: unending ray. Casting was terminated." << std::endl;
				canSee = false;
				break;
			}
		}
		if (canSee) {
			drawLine(float(i), localRayAngle, rayLength, hitDirection);
		}
	}
}
void Map::drawMinimap() {
	//Background
	const int MINIMAP_SIZE = 150;
	sf::RectangleShape background(sf::Vector2f(MINIMAP_SIZE, MINIMAP_SIZE));
	background.setFillColor(sf::Color(100, 100, 100, 170));
	window->draw(background);

	const float SCALE = float(MINIMAP_SIZE) / (BLOCK_SIZE * MAP_WIDTH);

	//Blocks
	const float MAP_BLOCK_SIZE = BLOCK_SIZE * SCALE;
	sf::RectangleShape block(sf::Vector2f(MAP_BLOCK_SIZE, MAP_BLOCK_SIZE));
	block.setFillColor(sf::Color(20, 20, 20, 255));
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (mapLayout[i][j]) {
				block.setPosition(j * MAP_BLOCK_SIZE, i * MAP_BLOCK_SIZE);
				window->draw(block);
			}
		}
	}

	//Player
	const int PLAYER_WIDTH = 4;
	float playerX = x * SCALE;
	float playerY = y * SCALE;
	sf::CircleShape player(PLAYER_WIDTH);
	player.setFillColor(sf::Color(40, 40, 40, 255));
	player.setPosition(playerX - PLAYER_WIDTH, playerY - PLAYER_WIDTH);
	window->draw(player);

	//Angle Guide Line
	sf::RectangleShape angleLine(sf::Vector2f(8, 2));
	angleLine.setFillColor(sf::Color(40, 40, 40, 255));
	angleLine.setOrigin(0, 1);
	angleLine.setPosition(playerX, playerY);
	angleLine.setRotation(-angle);
	window->draw(angleLine);
}