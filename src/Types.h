#pragma once
#include <raylib.h>

constexpr int baseWidth = 1280;
constexpr int baseHeight = 720;

enum class ScreenState {
    MENU,
    PLAYING,
    GAME_OVER,
    VICTORY,
    HISTORY
};

struct GameConfig {
    float ballSpeed = 580.0f;
    int blockRows = 5;
    int blockCols = 11;
};



struct Star {
    Vector2 pos;
    float speed;
    float radius;
    float alpha;
};

struct Particle {
    Vector2 pos;
    Vector2 vel;
    float life;
    float maxLife;
    float size;
    Color color;
};

struct Brick {
    int id;
    Rectangle rect;
    int hp;
    int maxHp;
    Color color;
};

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius;
    bool active;
};

struct Paddle {
    Rectangle rect;
    float speed;
};