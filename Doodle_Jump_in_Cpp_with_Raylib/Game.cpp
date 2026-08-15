#include "Game.h"
#include "Platforms.h"
#include "Player.h"

Game::Game()
{
	Image background_image = LoadImage("images/background.png");
	background_texture = LoadTextureFromImage(background_image);
	UnloadImage(background_image);
	background_color = WHITE;
	bgY = 0.0f;
	platforms.Init((float)GetScreenHeight() - 80.0f);
	gameOver = false;
}

Game::~Game()
{
	UnloadTexture(background_texture);
}

void Game::CheckGameOver(const Player& player)
{
	// Використовуємо GetPos().y замість player.pos.y
	const float FALL_LIMIT = (float)GetScreenHeight() + 300.0f;
	if (player.GetPos().y > FALL_LIMIT) {
		gameOver = true; // Замінено isGameOver на gameOver
	}
}

void Game::Reset(Player& player)
{
	// Скидаємо фон та платформи
	bgY = 0.0f;
	platforms.Init((float)GetScreenHeight() - 80.0f);
	gameOver = false; // Замінено isGameOver на gameOver

	// Скидаємо позицію та швидкість гравця через SetPos
	float startX = (GetScreenWidth() - player.GetWidth()) / 2.0f;
	float startY = (GetScreenHeight() - player.GetHeight()) / 2.0f;
	player.SetPos({ startX, startY });
	player.SetVelY(0.0f);
}

void Game::Draw(const Vector2& playerPos)
{
	if (playerPos.y < GetScreenHeight() / 2) {
		bgY -= speed * GetFrameTime();
	}

	if (bgY < -background_texture.height) bgY = 0.0f;
	if (bgY > 0.0f) bgY = 0.0f;

	DrawTexture(background_texture, 0, (int)bgY, background_color);

	platforms.Update(playerPos);
	platforms.Draw();
}

void Game::Scroll(float dy)
{
	if (!gameOver) {
		bgY -= dy;
		platforms.ShiftAll(dy);
	}
}

void Game::collisionPlayerWithPlatforms(Player& player)
{
	if (!gameOver) {
		float dt = GetFrameTime();
		float playerW = player.GetWidth();
		float playerH = player.GetHeight();
		float bottom = player.GetPos().y + playerH;
		float prevBottom = bottom - player.GetVelY() * dt;

		// Прибрали const, щоб можна було змінювати стан платформи (isBroken)
		for (auto& pl : platforms.GetList()) {
			if (pl.isBroken) continue; // Ігноруємо зламані платформи

			// Перевірка перекриття по горизонталі
			bool hx = (player.GetPos().x + playerW > pl.rect.x) && (player.GetPos().x < pl.rect.x + pl.rect.width);
			if (!hx) continue;

			// Перевірка падіння та торкання верхньої грані платформи
			if (player.GetVelY() > 0 && prevBottom <= pl.rect.y && bottom >= pl.rect.y) {
				if (pl.type == PlatformType::BROKEN) {
					pl.isBroken = true; // Коричнева платформа ламається і гравець падає далі
				}
				else {
					player.SetPosY(pl.rect.y - playerH);
					player.Bounce();
				}
				break;
			}
		}
	}
}

void Game::game_over(Player& player)
{
	if (player.GetPos().y - player.GetHeight() <= 0) {
		gameOver = true;
		DrawText("Game Over", GetScreenWidth() / 2 - 80, GetScreenHeight() / 2, 40, BLACK);
	}
}