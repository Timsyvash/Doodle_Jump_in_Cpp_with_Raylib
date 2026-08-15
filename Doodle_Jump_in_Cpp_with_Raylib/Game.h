#pragma once
#include <raylib.h>
#include "Platforms.h"
class Player;

class Game {
private:
	Texture background_texture;
	Color background_color;
	float bgY;
	const float speed = 4.0f;
	Platforms platforms;
	bool gameOver;
public:
	Game();
	~Game();
	void Draw(const Vector2& playerPos);
	// Scroll world by dy (move background and platforms)
	void Scroll(float dy);
	void collisionPlayerWithPlatforms(Player &player);
	void game_over(Player& player);
	// Game over handling
	bool isGameOver = false;
	void CheckGameOver(const Player &player);
	void Reset(Player &player);
};