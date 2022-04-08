#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true)
{

}

bool Game::Initialize() {
	// SDL初期化
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("SDLを初期化できません : %s", SDL_GetError());
		return false;
	}

	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow(
		"Game Programming in C++(Chapter1)", // ウィンドウのタイトル
		100, // ウィンドウ左上隅のx座標
		100, // ウィンドウ左上隅のy座標
		1024, // ウィンドウの幅(width)
		768, // ウィンドウの高さ(height)
		0 // フラグ(設定しない時は0)
	);

	if (!mWindow) {
		SDL_Log("ウィンドウの作成に失敗しました : %s", SDL_GetError());
		return false;
	}

	// SDLレンダラーを作成
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
	// キューにイベントがあれば繰り返す
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
	//前のフレームから16msが経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// deltatimeは前のフレームとの時刻の差を秒に変換した値
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	//deltatimeを最大値で制限する
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	/*
	 * 2つのボール用のループ
	 */
	for (int b = 0; b < 2; b++) {
		//mBallPos.x += mBallVel.x * deltaTime;
		//mBallPos.y += mBallVel.y * deltaTime;
		mBalls[b].pos.x += mBalls[b].vel.x * deltaTime;
		mBalls[b].pos.y += mBalls[b].vel.y * deltaTime;

		float diff[2] = { 0,0 };
		/*
		 * 2つのパドル用のループ
		 */
		for (int i = 0; i < 2; i++) {
			if (mPaddlesDir[i] != 0) {
				mPaddlesPos[i].y += mPaddlesDir[i] * 300.0f * deltaTime;
				// パドルが画面から出ないようにする
				if (mPaddlesPos[i].y < (paddleH / 2.0f + thickness)) {
					mPaddlesPos[i].y = paddleH / 2.0f + thickness;
				}
				else if (mPaddlesPos[i].y > (768.0f - paddleH / 2.0f - thickness)) {
					mPaddlesPos[i].y = 768.0f - paddleH / 2.0f - thickness;
				}
			}


			// パドルによる跳ね返り処理
			// パドルとボールの縦の距離
			diff[i] = mPaddlesPos[i].y - mBalls[b].pos.y;
			// 距離を絶対値化
			diff[i] = (diff[i] > 0.0f) ? diff[i] : -diff[i];
			if (
				// 距離が近い
				(diff[0] <= paddleH / 2.0f &&
					// ボールが画面左端
					mBalls[b].pos.x <= 25.0f && mBalls[b].pos.x >= 20.0f &&
					// ボールが左向きに進む
					mBalls[b].vel.x < 0.0f) ||
				// 距離が近い
				(diff[1] <= paddleH / 2.0f &&
					// ボールが画面右端
					mBalls[b].pos.x <= 990.0f && mBalls[b].pos.x >= 985.0f &&
					// ボールが右向きに進む
					mBalls[b].vel.x > 0.0f)
				)
			{
				mBalls[b].vel.x *= -1.0f;
			}
			// ボールが画面外にある場合
			else if (mBalls[b].pos.x <= 0.0f || mBalls[b].pos.x >= 1024.0f)
			{
				mIsRunning = false;
			}

			// 上壁での跳ね返り
			if (mBalls[b].pos.y <= thickness && mBalls[b].vel.y < 0.0f)
			{
				mBalls[b].vel.y *= -1;
			}
			// 下壁での跳ね返り
			else if (mBalls[b].pos.y >= (768 - thickness) &&
				mBalls[b].vel.y > 0.0f)
			{
				mBalls[b].vel.y *= -1;
			}
		}
	}


}

void Game::GenerateOutput() {

	// 描画色を青色に設定
	SDL_SetRenderDrawColor(
		mRenderer,
		0, 0, 255, 255 // (R,G,B,A)
	);

	// バックバッファを青色で塗りつぶす
	SDL_RenderClear(mRenderer);

	// 描画色を白色に設定
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// 壁を作成
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// width
		thickness	// height

	};
	SDL_RenderFillRect(mRenderer, &wall);

	// 下壁を作成
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// パドル作成
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

	// ボール作成
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

	// バックバッファとフロントバッファを入れ替え
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