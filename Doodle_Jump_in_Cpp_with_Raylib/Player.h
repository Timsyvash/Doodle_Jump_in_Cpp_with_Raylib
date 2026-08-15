#pragma once
#include <raylib.h>
#include <vector>
#include "Platforms.h"

class Player {
private:
    Vector2 pos;
    Vector2 size = { 60.0f, 60.0f };
    float velocityY = 0.0f;
    float gravity = 0.4f;
    float jumpForce = 15.0f;
    float speedX = 6.0f;

    Texture2D textureLeft;
    Texture2D textureRight;
    bool facingRight = true;

public:
    Player();
    ~Player();

    // Прибрано const у параметрі platforms
    void Update(std::vector<Platform>& platforms);
    void Draw() const;

    Vector2 GetPos() const { return pos; }
    void SetPos(Vector2 p) { pos = p; }
    void SetPosY(float y) { pos.y = y; }

    float GetWidth() const { return size.x; }
    float GetHeight() const { return size.y; }
    float GetVelY() const { return velocityY; }
    void SetVelY(float vy) { velocityY = vy; }

    void Bounce() { velocityY = -jumpForce; }
    void Reset();
};