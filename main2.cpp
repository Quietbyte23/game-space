#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 500;
    
    InitWindow(screenWidth, screenHeight, "Alex Space Shooter - C++ Wasm");

    Vector2 shipPosition = { (float)screenWidth / 2, (float)screenHeight - 60 };
    float shipSpeed = 6.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Input logic
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) shipPosition.x += shipSpeed;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) shipPosition.x -= shipSpeed;

        if (shipPosition.x < 20) shipPosition.x = 20;
        if (shipPosition.x > screenWidth - 20) shipPosition.x = screenWidth - 20;

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("C++ & RAYLIB SPACE MISSION (WEBASSEMBLY)", 180, 25, 18, RAYWHITE);

        // Player ship (Triangle)
        DrawTriangle(
            { shipPosition.x, shipPosition.y - 20 },
            { shipPosition.x - 15, shipPosition.y + 15 },
            { shipPosition.x + 15, shipPosition.y + 15 },
            CYAN
        );

        DrawText("Use A/D keys to move", 290, 460, 14, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
