#include "Game.h"
#include <vector>

Game::Game() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	SetTargetFPS(120);
	InitWindow(800, 450, "Brick N Ball");

}

Game::~Game() {
	CloseWindow();
}

void Game::Start() {
	while (!WindowShouldClose()) {
		UpdateAndDrawFrame();
	}
}

void Game::UpdateAndDrawFrame() {
	BeginDrawing();
	ClearBackground(BLACK);

	EndDrawing();
}
