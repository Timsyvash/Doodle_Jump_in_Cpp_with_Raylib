#pragma once
#include <raylib.h>
#include <vector>
#include <string>

enum class PlatformType {
    NORMAL,
    BROKEN
};

struct Platform {
    Rectangle rect;
    PlatformType type = PlatformType::NORMAL;
    bool isBroken = false;
    float fallSpeed = 0.0f; // Швидкість падіння уламка
    int texIndex = -1;
};

class Platforms {
private:
    std::vector<Platform> list;
    std::vector<Texture2D> textures;
    Texture2D brokenTexture;      // platform_3.png (ціла)
    Texture2D brokenAnimTexture;  // platform_4.png (зламана)

    float highestY = 0.0f;
    float defaultWidth = 60.0f;
    float defaultHeight = 15.0f;
    int maxPlatforms = 10;
    float minSpacing = 60.0f;
    float maxSpacing = 120.0f;

public:
    Platforms();
    ~Platforms();

    void Init(float startY);
    void Update(const Vector2& playerPos);
    void Draw() const;
    void ShiftAll(float dy);

    std::vector<Platform>& GetList() { return list; }
};