#include "raylib.h"
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>

// --- GAME CONFIGURATION ---
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;
const float PLAYER_SPEED = 6.5f;
const float BULLET_SPEED = 10.0f;

// --- ENTITY STRUCTURES ---
struct Vector2D {
    float x, y;
};

class GameObject {
protected:
    Vector2D position;
    Vector2D velocity;
    float radius;
    bool active;

public:
    GameObject(float px, float py, float r) : position{px, py}, velocity{0, 0}, radius(r), active(true) {}
    virtual ~GameObject() = default;
    
    virtual void Update() = 0;
    virtual void Draw() = 0;

    Vector2D GetPosition() const { return position; }
    bool IsActive() const { return active; }
    void SetActive(bool state) { active = state; }
    float GetRadius() const { return radius; }
};

class Player : public GameObject {
private:
    int health;
    int score;

public:
    Player(float px, float py) : GameObject(px, py, 15.0f), health(3), score(0) {}

    void Update() override {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += PLAYER_SPEED;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) position.x -= PLAYER_SPEED;

        // Screen boundaries
        if (position.x < 25) position.x = 25;
        if (position.x > SCREEN_WIDTH - 25) position.x = SCREEN_WIDTH - 25;
    }

    void Draw() override {
        DrawTriangle(
            { position.x, position.y - 20 },
            { position.x - 15, position.y + 15 },
            { position.x + 15, position.y + 15 },
            CYAN
        );
    }

    int GetHealth() const { return health; }
    void Damage() { health--; }
    int GetScore() const { return score; }
    void AddScore(int pts) { score += pts; }
};

class Bullet : public GameObject {
public:
    Bullet(float px, float py) : GameObject(px, py, 4.0f) {
        velocity = {0, -BULLET_SPEED};
    }

    void Update() override {
        position.y += velocity.y;
        if (position.y < 0) active = false;
    }

    void Draw() override {
        DrawRectangle(position.x - 2, position.y - 6, 4, 12, GREEN);
    }
};

class Meteor : public GameObject {
private:
    float size;

public:
    Meteor(float px, float py, float s) : GameObject(px, py, s), size(s) {
        velocity = {0, (float)(rand() % 3 + 2)};
    }

    void Update() override {
        position.y += velocity.y;
        if (position.y > SCREEN_HEIGHT + size) active = false;
    }

    void Draw() override {
        DrawRectangle(position.x - size/2, position.y - size/2, size, size, RED);
    }
};

// --- MAIN ENGINE CONTROLLER ---
class GameEngine {
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Meteor>> meteors;
    float spawnTimer;
    bool gameOver;

public:
    GameEngine() : spawnTimer(0), gameOver(false) {
        player = std::make_unique<Player>(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 60.0f);
    }

    void Run() {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Alex Space Shooter - C++ Core Engine");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            Update();
            Render();
        }

        CloseWindow();
    }

private:
    void Update() {
        if (gameOver) {
            if (IsKeyPressed(KEY_R)) {
                Reset();
            }
            return;
        }

        player->Update();

        // Shooting logic
        if (IsKeyPressed(KEY_SPACE)) {
            Vector2D pos = player->GetPosition();
            bullets.push_back(std::make_unique<Bullet>(pos.x, pos.y));
        }

        // Update Bullets
        for (auto& b : bullets) {
            b->Update();
        }

        // Spawn Meteors
        spawnTimer += GetFrameTime();
        if (spawnTimer > 0.8f) {
            spawnTimer = 0;
            float rx = (float)(rand() % (SCREEN_WIDTH - 60) + 30);
            float sz = (float)(rand() % 20 + 20);
            meteors.push_back(std::make_unique<Meteor>(rx, -30, sz));
        }

        // Update Meteors & Collisions
        for (auto& m : meteors) {
            m->Update();

            // Player collision
            Vector2D pPos = player->GetPosition();
            Vector2D mPos = m->GetPosition();
            float dx = pPos.x - mPos.x;
            float dy = pPos.y - mPos.y;
            if (sqrt(dx*dx + dy*dy) < (player->GetRadius() + m->GetRadius())) {
                m->SetActive(false);
                player->Damage();
                if (player->GetHealth() <= 0) {
                    gameOver = true;
                }
            }

            // Bullet collision
            for (auto& b : bullets) {
                if (!b->IsActive()) continue;
                Vector2D bPos = b->GetPosition();
                float bdx = bPos.x - mPos.x;
                float bdy = bPos.y - mPos.y;
                if (sqrt(bdx*bdx + bdy*bdy) < (b->GetRadius() + m->GetRadius())) {
                    m->SetActive(false);
                    m->SetActive(false);
                    player->AddScore(10);
                }
            }
        }

        // Cleanup inactive objects
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const auto& b){ return !b->IsActive(); }), bullets.end());
        meteors.erase(std::remove_if(meteors.begin(), meteors.end(), [](const auto& m){ return !m->IsActive(); }), meteors.end());
    }

    void Render() {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw HUD Header
        DrawText(TextFormat("SCORE: %d", player->GetScore()), 20, 20, 20, GREEN);
        DrawText(TextFormat("LIVES: %d", player->GetHealth()), SCREEN_WIDTH - 120, 20, 20, RED);

        player->Draw();

        for (auto& b : bullets) b->Draw();
        for (auto& m : meteors) m->Draw();

        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.85f));
            DrawText("MISSION FAILED", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 40, 30, RED);
            DrawText("PRESS [R] TO RESTART", SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 10, 16, RAYWHITE);
        }

        EndDrawing();
    }

    void Reset() {
        player = std::make_unique<Player>(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 60.0f);
        bullets.clear();
        meteors.clear();
        spawnTimer = 0;
        gameOver = false;
    }
};

int main() {
    GameEngine engine;
    engine.Run();
    return 0;
}
