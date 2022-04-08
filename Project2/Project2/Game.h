#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// テクスチャの配列
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// ゲーム上の全てのアクター
	std::vector<class Actor*> mActors;
	// 待ち状態のアクター
	std::vector<class Actor*> mPendingActors;

	// 全てのスプライトコンポーネントの配列
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	// / アクターが更新中か
	bool mIsUpdatingActors;

	class Ship* mShip; // 宇宙船
	class Character* mCharacter; // 宇宙船
};
