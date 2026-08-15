#include "Player.h"

Player::Player()
{
    Image imgR = LoadImage("images/Doodle_Jump_1.png");
    textureRight = LoadTextureFromImage(imgR);
    UnloadImage(imgR);

    Image imgL = LoadImage("images/Doodle_Jump_2.png");
    textureLeft = LoadTextureFromImage(imgL);
    UnloadImage(imgL);

    pos = { (float)GetScreenWidth() / 2.0f - size.x / 2.0f, 300.0f };
    velocityY = -jumpForce;
}

Player::~Player()
{
    UnloadTexture(textureLeft);
    UnloadTexture(textureRight);
}

void Player::Update(std::vector<Platform>& platforms)
{
    // 1. Рух
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        pos.x -= speedX;
        facingRight = false;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        pos.x += speedX;
        facingRight = true;
    }

    // Wrap around
    if (pos.x < -size.x) pos.x = (float)GetScreenWidth();
    if (pos.x > GetScreenWidth()) pos.x = -size.x;

    // 2. Гравітація
    velocityY += gravity;
    pos.y += velocityY;

    // Перезапуск при падінні за екран
    // Уважно: більше не робимо автоматичний рестарт тут, щоб Game могла обробити Game Over
    // Якщо потрібно телепортувати гравця при невеликому виході за екран — це робиться у main (щоб не перешкоджати Game Over)

    // 3. Колізія з платформами (тільки під час падіння)
    if (velocityY > 0.0f)
    {
        Rectangle playerFeet = { pos.x + 10.0f, pos.y + size.y - 10.0f, size.x - 20.0f, 10.0f };

        for (auto& platform : platforms)
        {
            // Ігноруємо вже зламані платформи
            if (platform.isBroken) continue;

            if (CheckCollisionRecs(playerFeet, platform.rect))
            {
                if (pos.y + size.y <= platform.rect.y + 15.0f)
                {
                    if (platform.type == PlatformType::BROKEN)
                    {
                        // Коричнева платформа ламається, Bounce() НЕ викликається (гравець падає далі)
                        platform.isBroken = true;
                    }
                    else
                    {
                        // Звичайна платформа — відштовхуємося
                        Bounce();
                    }
                    break;
                }
            }
        }
    }
}

void Player::Reset()
{
    // Reposition player to center-top area and clear vertical velocity
    pos = { (float)GetScreenWidth() / 2.0f - size.x / 2.0f, 200.0f };
    velocityY = 0.0f;
    facingRight = true;
}

void Player::Draw() const
{
    Texture2D currentTex = facingRight ? textureRight : textureLeft;

    if (currentTex.id > 0)
    {
        DrawTexturePro(currentTex,
            { 0, 0, (float)currentTex.width, (float)currentTex.height },
            { pos.x, pos.y, size.x, size.y },
            { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        DrawRectangleV(pos, size, GREEN);
    }
}