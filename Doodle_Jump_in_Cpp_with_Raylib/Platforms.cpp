#include "Platforms.h"
#include <algorithm>

Platforms::Platforms()
{
    // 1. Зелені платформи
    std::vector<std::string> normalPaths = {
        "images/platform_1.png",
        "images/platform_2.png"
    };

    for (const auto& path : normalPaths) {
        Image img = LoadImage(path.c_str());
        if (img.data != nullptr) {
            Texture2D t = LoadTextureFromImage(img);
            UnloadImage(img);
            if (t.id != 0) textures.push_back(t);
        }
    }

    // 2. Ціла коричнева платформа (platform_3.png)
    Image img3 = LoadImage("images/platform_3.png");
    if (img3.data != nullptr) {
        brokenTexture = LoadTextureFromImage(img3);
        UnloadImage(img3);
    }

    // 3. Зламана коричнева платформа (platform_4.png)
    Image img4 = LoadImage("images/platform_4.png");
    if (img4.data != nullptr) {
        brokenAnimTexture = LoadTextureFromImage(img4);
        UnloadImage(img4);
    }

    if (!textures.empty()) {
        defaultWidth = (float)textures[0].width;
        defaultHeight = (float)textures[0].height;
    }
}

Platforms::~Platforms()
{
    for (auto& t : textures) {
        if (t.id != 0) UnloadTexture(t);
    }
    if (brokenTexture.id != 0) UnloadTexture(brokenTexture);
    if (brokenAnimTexture.id != 0) UnloadTexture(brokenAnimTexture);
}

void Platforms::Init(float startY)
{
    list.clear();
    highestY = startY;

    Platform startP;
    int randomTex = textures.empty() ? -1 : GetRandomValue(0, (int)textures.size() - 1);
    float w = (randomTex >= 0) ? (float)textures[randomTex].width : defaultWidth;
    float h = (randomTex >= 0) ? (float)textures[randomTex].height : defaultHeight;

    startP.rect = { (GetScreenWidth() - w) / 2.0f, startY, w, h };
    startP.texIndex = randomTex;
    startP.type = PlatformType::NORMAL;
    list.push_back(startP);

    for (int i = 1; i < maxPlatforms; ++i) {
        highestY -= GetRandomValue((int)minSpacing, (int)maxSpacing);

        Platform p;
        if (GetRandomValue(1, 100) <= 20) {
            p.type = PlatformType::BROKEN;
            p.texIndex = -1;
            float pW = (brokenTexture.id > 0) ? (float)brokenTexture.width : defaultWidth;
            float pH = (brokenTexture.id > 0) ? (float)brokenTexture.height : defaultHeight;
            p.rect = { (float)GetRandomValue(0, GetScreenWidth() - (int)pW), highestY, pW, pH };
        }
        else {
            p.type = PlatformType::NORMAL;
            int texIdx = textures.empty() ? -1 : GetRandomValue(0, (int)textures.size() - 1);
            p.texIndex = texIdx;
            float pW = (texIdx >= 0) ? (float)textures[texIdx].width : defaultWidth;
            float pH = (texIdx >= 0) ? (float)textures[texIdx].height : defaultHeight;
            p.rect = { (float)GetRandomValue(0, GetScreenWidth() - (int)pW), highestY, pW, pH };
        }

        list.push_back(p);
    }
}

void Platforms::Update(const Vector2& playerPos)
{
    // Анімація падіння зламаних платформ
    for (auto& pl : list) {
        if (pl.isBroken) {
            pl.fallSpeed += 0.5f; // Гравітація для уламка
            pl.rect.y += pl.fallSpeed;
        }
    }

    // Видалення платформ, що вилетіли за нижній край
    float removeY = playerPos.y + GetScreenHeight() + 100.0f;
    list.erase(std::remove_if(list.begin(), list.end(), [removeY](const Platform& pl) {
        return pl.rect.y > removeY;
        }), list.end());

    if (!list.empty()) {
        float minY = list[0].rect.y;
        for (const auto& p : list) {
            if (p.rect.y < minY) minY = p.rect.y;
        }
        highestY = minY;
    }

    while ((int)list.size() < maxPlatforms) {
        highestY -= GetRandomValue((int)minSpacing, (int)maxSpacing);

        Platform p;
        if (GetRandomValue(1, 100) <= 20) {
            p.type = PlatformType::BROKEN;
            p.texIndex = -1;
            float pW = (brokenTexture.id > 0) ? (float)brokenTexture.width : defaultWidth;
            float pH = (brokenTexture.id > 0) ? (float)brokenTexture.height : defaultHeight;
            p.rect = { (float)GetRandomValue(0, GetScreenWidth() - (int)pW), highestY, pW, pH };
        }
        else {
            p.type = PlatformType::NORMAL;
            int texIdx = textures.empty() ? -1 : GetRandomValue(0, (int)textures.size() - 1);
            p.texIndex = texIdx;
            float pW = (texIdx >= 0) ? (float)textures[texIdx].width : defaultWidth;
            float pH = (texIdx >= 0) ? (float)textures[texIdx].height : defaultHeight;
            p.rect = { (float)GetRandomValue(0, GetScreenWidth() - (int)pW), highestY, pW, pH };
        }

        list.push_back(p);
    }
}

void Platforms::Draw() const
{
    for (const auto& pl : list)
    {
        // 1. Малювання зламаної платформи (уламків)
        if (pl.isBroken) {
            if (brokenAnimTexture.id > 0) {
                DrawTexturePro(brokenAnimTexture,
                    { 0, 0, (float)brokenAnimTexture.width, (float)brokenAnimTexture.height },
                    pl.rect, { 0, 0 }, 0.0f, WHITE);
            }
            continue;
        }

        // 2. Малювання цілої коричневої платформи
        if (pl.type == PlatformType::BROKEN) {
            if (brokenTexture.id > 0) {
                DrawTexturePro(brokenTexture,
                    { 0, 0, (float)brokenTexture.width, (float)brokenTexture.height },
                    pl.rect, { 0, 0 }, 0.0f, WHITE);
            }
            else {
                DrawRectangleRec(pl.rect, BROWN);
            }
        }
        // 3. Малювання зелених платформ
        else {
            if (pl.texIndex >= 0 && pl.texIndex < (int)textures.size() && textures[pl.texIndex].id > 0) {
                DrawTexturePro(textures[pl.texIndex],
                    { 0, 0, (float)textures[pl.texIndex].width, (float)textures[pl.texIndex].height },
                    pl.rect, { 0, 0 }, 0.0f, WHITE);
            }
            else {
                DrawRectangleRec(pl.rect, GREEN);
            }
        }
    }
}

void Platforms::ShiftAll(float dy)
{
    for (auto& p : list) {
        p.rect.y += dy;
    }
    highestY += dy;
}