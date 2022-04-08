#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true)
{

}

bool Game::Initialize() {
	// SDL������
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDL���������ł��܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter1)", // �E�B���h�E�̃^�C�g��
		100, // �E�B���h�E�������x���W
		100, // �E�B���h�E�������y���W
		1024, // �E�B���h�E�̕�(width)
		768, // �E�B���h�E�̍���(height)
		0 // �t���O(�ݒ肵�Ȃ�����0)
	);

	if (!mWindow) {
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�����_���[���쐬
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	Ball ball0_info = { {1024.0f / 2.0f, 768.0f / 2.0f}, {-200.0f, 235.0f} };
	Ball ball1_info = { {1024.0f / 4.0f, 768.0f / 2.0f}, {200.0f, 235.0f} };

	mBalls.push_back(ball0_info);
	mBalls.push_back(ball1_info);

	mPaddlesPos.push_back({ 10.0f,768.0f / 2.0f });
	mPaddlesPos.push_back({ 1000.0f,768.0f / 2.0f });

	mPaddlesDir.push_back(0);
	mPaddlesDir.push_back(0);

	return true;
}

void Game::ProcessInput() {
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddlesDir[0] = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddlesDir[0] -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddlesDir[0] += 1;
	}

	mPaddlesDir[1] = 0;
	if (state[SDL_SCANCODE_I]) {
		mPaddlesDir[1] -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mPaddlesDir[1] += 1;
	}


}

void Game::UpdateGame() {
	//�O�̃t���[������16ms���o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatime�͑O�̃t���[���Ƃ̎����̍���b�ɕϊ������l
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatime���ő�l�Ő�������
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	/*
	 * 2�̃{�[���p�̃��[�v
	 */
	for (int b = 0; b < 2; b++) {
		//mBallPos.x += mBallVel.x * deltaTime;
		//mBallPos.y += mBallVel.y * deltaTime;
		mBalls[b].pos.x += mBalls[b].vel.x * deltaTime;
		mBalls[b].pos.y += mBalls[b].vel.y * deltaTime;

		float diff[2] = { 0,0 };
		/*
		 * 2�̃p�h���p�̃��[�v
		 */
		for (int i = 0; i < 2; i++) {
			if (mPaddlesDir[i] != 0) {
				mPaddlesPos[i].y += mPaddlesDir[i] * 300.0f * deltaTime;
				// �p�h������ʂ���o�Ȃ��悤�ɂ���
				if (mPaddlesPos[i].y < (paddleH / 2.0f + thickness)) {
					mPaddlesPos[i].y = paddleH / 2.0f + thickness;
				}
				else if (mPaddlesPos[i].y > (768.0f - paddleH / 2.0f - thickness)) {
					mPaddlesPos[i].y = 768.0f - paddleH / 2.0f - thickness;
				}
			}


			// �p�h���ɂ�钵�˕Ԃ菈��
			// �p�h���ƃ{�[���̏c�̋���
			diff[i] = mPaddlesPos[i].y - mBalls[b].pos.y;
			// �������Βl��
			diff[i] = (diff[i] > 0.0f) ? diff[i] : -diff[i];
			if (
				// �������߂�
				(diff[0] <= paddleH / 2.0f &&
					// �{�[������ʍ��[
					mBalls[b].pos.x <= 25.0f && mBalls[b].pos.x >= 20.0f &&
					// �{�[�����������ɐi��
					mBalls[b].vel.x < 0.0f) ||
				// �������߂�
				(diff[1] <= paddleH / 2.0f &&
					// �{�[������ʉE�[
					mBalls[b].pos.x <= 990.0f && mBalls[b].pos.x >= 985.0f &&
					// �{�[�����E�����ɐi��
					mBalls[b].vel.x > 0.0f)
				)
			{
				mBalls[b].vel.x *= -1.0f;
			}
			// �{�[������ʊO�ɂ���ꍇ
			else if (mBalls[b].pos.x <= 0.0f || mBalls[b].pos.x >= 1024.0f)
			{
				mIsRunning = false;
			}

			// ��ǂł̒��˕Ԃ�
			if (mBalls[b].pos.y <= thickness && mBalls[b].vel.y < 0.0f)
			{
				mBalls[b].vel.y *= -1;
			}
			// ���ǂł̒��˕Ԃ�
			else if (mBalls[b].pos.y >= (768 - thickness) &&
				mBalls[b].vel.y > 0.0f)
			{
				mBalls[b].vel.y *= -1;
			}
		}
	}


}

void Game::GenerateOutput() {

	// �`��F��F�ɐݒ�
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 0, 255, 255 // (R,G,B,A)
	);

	// �o�b�N�o�b�t�@��F�œh��Ԃ�
	SDL_RenderClear(mRenderer);

	// �`��F�𔒐F�ɐݒ�
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// �ǂ��쐬
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// width
		thickness	// height

	};
	SDL_RenderFillRect(mRenderer, &wall);

	// ���ǂ��쐬
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// �p�h���쐬
	SDL_Rect paddle0{
		static_cast<int>(mPaddlesPos[0].x),
		static_cast<int>(mPaddlesPos[0].y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle0);

	SDL_Rect paddle1{
		static_cast<int>(mPaddlesPos[1].x),
		static_cast<int>(mPaddlesPos[1].y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle1);

	// �{�[���쐬
	SDL_Rect ball0{
		static_cast<int>(mBalls[0].pos.x - thickness / 2),
		static_cast<int>(mBalls[0].pos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball0);

	SDL_Rect ball1{
		static_cast<int>(mBalls[1].pos.x - thickness / 2),
		static_cast<int>(mBalls[1].pos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball1);

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ�
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}