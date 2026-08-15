#include <raylib.h>
#include "Player.h"
#include "Platforms.h"

int main()
{
    InitWindow(400, 600, "Doodle Jump");
    SetTargetFPS(60);

    Platforms platforms;
    platforms.Init(500.0f);

    Player player;
    bool isGameOver = false;
    bool paused = false;

    while (!WindowShouldClose())
    {
        // Toggle pause
        if (IsKeyPressed(KEY_P)) paused = !paused;

        // 1. Оновлення стану (тільки якщо не Game Over і не на паузі)
        if (!isGameOver && !paused) {
            player.Update(platforms.GetList());
            platforms.Update(player.GetPos());

            // 2. Прокрутка екрана: якщо гравець піднявся вище середини (Y < 300)
            if (player.GetPos().y < 300.0f)
            {
                float deltaY = 300.0f - player.GetPos().y;
                player.SetPosY(300.0f);   // Фіксуємо гравця по центру Y
                platforms.ShiftAll(deltaY); // Зсуваємо всі платформи вниз
            }

        // (pause overlay drawing moved to drawing section)

            // 3. Перевірка Game Over — якщо впав занадто низько
            if (player.GetPos().y > GetScreenHeight() + 300.0f) {
                isGameOver = true;
            }

            // Якщо гравець вийшов за межі по горизонталі або зверху — телепортувати в центр
            const float H_MARGIN = 100.0f; // поріг по X
            const float TOP_MARGIN = -100.0f; // поріг зверху
            Vector2 p = player.GetPos();
            if (p.x < -H_MARGIN || p.x > GetScreenWidth() + H_MARGIN || p.y < TOP_MARGIN) {
                player.Reset();
            }
        }

        // 3. Малювання
        BeginDrawing();
        ClearBackground(RAYWHITE);

        platforms.Draw();
        player.Draw();

        // Draw pause overlay (must be between BeginDrawing/EndDrawing)
        if (paused && !isGameOver) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4f));
            const char *pmsg = "PAUSED";
            int pW = MeasureText(pmsg, 60);
            DrawText(pmsg, GetScreenWidth()/2 - pW/2, GetScreenHeight()/2 - 30, 60, YELLOW);
            DrawText("Press P to resume", GetScreenWidth()/2 - 120, GetScreenHeight()/2 + 40, 20, LIGHTGRAY);
        }

        if (isGameOver) {
            // Draw Game Over text and instructions
            const char *msg = "GAME OVER";
            const char *instr = "Press R to restart, ESC to quit";
            int msgW = MeasureText(msg, 40);
            int instrW = MeasureText(instr, 20);
            DrawText(msg, GetScreenWidth()/2 - msgW/2, GetScreenHeight()/2 - 40, 40, RED);
            DrawText(instr, GetScreenWidth()/2 - instrW/2, GetScreenHeight()/2 + 10, 20, DARKGRAY);

            // Restart
            if (IsKeyPressed(KEY_R)) {
                platforms.Init( (float)GetScreenHeight() - 80.0f );
                player.Reset();
                isGameOver = false;
            }
            // Quit
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}