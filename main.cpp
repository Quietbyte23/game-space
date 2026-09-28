#if __has_include(<raylib.h>)
#include <raylib.h>
#elif __has_include("raylib.h")
#include "raylib.h"
#else
#error "raylib.h not found. Install raylib and add its include directory to your compiler's include path."
#endif

int main() {
    // Window dimensions for our web build
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Alex Space Shooter - C++ Demo");

    // Player ship starting position
    Vector2 shipPosition = { (float)screenWidth / 2, (float)screenHeight - 80 };
    float shipSpeed = 6.0f;

    SetTargetFPS(60); // Run at a smooth 60 frames per second

    // Main game loop
    while (!WindowShouldClose()) {
        // --- 1. Update Logic (Input & Positions) ---
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) shipPosition.x += shipSpeed;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) shipPosition.x -= shipSpeed;

        // Keep the ship inside the screen bounds
        if (shipPosition.x < 20) shipPosition.x = 20;
        if (shipPosition.x > screenWidth - 20) shipPosition.x = screenWidth - 20;

        // --- 2. Render Graphics (Drawing) ---
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw a clean title
        DrawText("SPACE MISSION - C++ & WebAssembly", 150, 20, 20, RAYWHITE);

        // Draw the player ship (a simple triangle)
        DrawTriangle(
            { shipPosition.x, shipPosition.y - 20 },
            { shipPosition.x - 15, shipPosition.y + 15 },
            { shipPosition.x + 15, shipPosition.y + 15 },
            CYAN
        );

        // Instructions
        DrawText("Use A/D or Arrow Keys to move", 240, 550, 14, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}