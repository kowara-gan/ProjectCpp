#pragma once
#ifndef GAME_HEADER
#define GAME_HEADER
#define WIN_X SDL_WINDOWPOS_UNDEFINED
#define WIN_Y SDL_WINDOWPOS_UNDEFINED
#define WIN_W 640
#define WIN_H 480
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926
#endif
#include "SDL.h"

struct Vector2 {
	float x;
	float y;
};

struct Ball {
	Vector2 pos;
	Vector2 vel;
};

struct Score {
	int player, enemy;
};

class Game {
public:
	Game();
	//ゲームを初期化する
	bool Initialize();
	//ゲームオーバーまでゲームループを実行する
	void RunLoop();
	//ゲームをシャットダウンする
	void Shutdown();
private:
	//ゲームループのためのヘルパー関数群
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDLが作るウィンドウ
	SDL_Window* mWindow;
	//2D描画のためのレンダリング
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	//ゲームの続行を支持する
	bool mIsRunning;

	//Pong の詳細
	//パドルの方向
	std::vector<int> mPaddlesDir;
	//パドルの位置
	std::vector<Vector2> mPaddlesPos;
	//ボール
	std::vector<Ball> mBalls;
	//ボールの数
	static const int BALL_NUM = 2;
};
#endif