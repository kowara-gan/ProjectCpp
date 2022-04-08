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
	//�Q�[��������������
	bool Initialize();
	//�Q�[���I�[�o�[�܂ŃQ�[�����[�v�����s����
	void RunLoop();
	//�Q�[�����V���b�g�_�E������
	void Shutdown();
private:
	//�Q�[�����[�v�̂��߂̃w���p�[�֐��Q
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//SDL�����E�B���h�E
	SDL_Window* mWindow;
	//2D�`��̂��߂̃����_�����O
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	//�Q�[���̑��s���x������
	bool mIsRunning;

	//Pong �̏ڍ�
	//�p�h���̕���
	std::vector<int> mPaddlesDir;
	//�p�h���̈ʒu
	std::vector<Vector2> mPaddlesPos;
	//�{�[��
	std::vector<Ball> mBalls;
	//�{�[���̐�
	static const int BALL_NUM = 2;
};
#endif