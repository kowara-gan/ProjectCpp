#pragma once
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "SDL.h"

struct Vector2 {
	float x;
	float y;
};

struct Ball {
	Vector2 pos;
	Vector2 vel;
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

};