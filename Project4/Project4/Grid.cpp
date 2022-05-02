#include "Grid.h"
#include "Tile.h"
#include "Tower.h"
#include "Enemy.h"
#include <algorithm>

Grid::Grid(class Game* game)
	:Actor(game)
	, mSelectedTile(nullptr)
{
	// 7 rows, 16 columns
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	// Create tiles
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(Vector2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize));
		}
	}

	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	// Set up adjacency lists
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col)
{
	// �I�������^�C���̃X�e�[�g���m�F
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		// ��O�ɑI�������^�C���̑I����Ԃ�����
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}
		// �I�������^�C����I����Ԃ�
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}
// �N���b�N����
void Grid::ProcessClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

// A*�o�H�T��������
bool Grid::FindPath(Tile* start, Tile* goal)
{
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			// �q���[���X�e�B�b�N�l
			mTiles[i][j]->g = 0.0f;
			// �I�[�v���Z�b�g�ɋA��
			mTiles[i][j]->mInOpenSet = false;
			// �N���[�Y�h�Z�b�g�ɋA��
			mTiles[i][j]->mInClosedSet = false;
		}
	}
	// �I�[�v���Z�b�g�̓��I�z����`
	std::vector<Tile*> openSet;

	// �J�����g�m�[�h���L�^���邽�߂̕ϐ�
	// �X�^�[�g�̃m�[�h�ŏ�����
	Tile* current = start;
	// �J�����g�m�[�h���N���[�Y�h�Z�b�g�ɒǉ�
	current->mInClosedSet = true;
	// �S�[������܂Ń��[�v
	do
	{
		// �אڃm�[�h���I�[�v���Z�b�g�ɒǉ�
		for (Tile* neighbor : current->mAdjacent)
		{
			// �אڃm�[�h���ǂȂ�continue
			if (neighbor->mBlocked)
			{
				continue;
			}

			// �אڃm�[�h���N���[�Y�h�Z�b�g�łȂ�����ǉ�
			if (!neighbor->mInClosedSet)
			{
				// �I�[�v���Z�b�g�łȂ��Ȃ�
				if (!neighbor->mInOpenSet)
				{
					// �אڃm�[�h�̐e�m�[�h��ݒ肷��
					neighbor->mParent = current;
					// �אڃm�[�h�̃q���[���X�e�B�b�N�l��ݒ肷��
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// g(x) �o�H�R�X�g
					// �e�m�[�h�̌o�H�R�X�g + �G�b�W��H��R�X�g
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					// �אڃm�[�h���I�[�v���Z�b�g�ɒǉ�
					openSet.emplace_back(neighbor);
					// �I�[�v���Z�b�g�̃X�N���b�`�f�[�^��ݒ�
					neighbor->mInOpenSet = true;
				}
				else
				{
					// �I�[�v���Z�b�g�̏ꍇ�A���Ƃ��Ɛݒ肵�Ă���g(x)��
					// ���݂̃J�����g�m�[�h����v�Z����g(x)���r���āA
					// ���X�ݒ肵�Ă���g(x)�̕����傫���ꍇ�́A���݂̃J�����g�m�[�h����v�Z����g(x)��ݒ�
					
					// ���݂̃J�����g�m�[�h����g(x)���v�Z
					float newG = current->g + TileSize;
					if (newG < neighbor->g)
					{
						// �J�����g�m�[�h���̗p
						neighbor->mParent = current;
						neighbor->g = newG;
						// g(x)��ύX�����̂�f(x)���ύX
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		// �I�[�v���Z�b�g����Ȃ�A�S�Ẳ\���̂���o�H�͒T��������
		if (openSet.empty())
		{
			break;
		}

		// �ł��R�X�g�̒Ⴂ�m�[�h���I�[�v���Z�b�g����T��
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) {
				return a->f < b->f;
			});
		// �ł��R�X�g�̒Ⴂ�m�[�h���J�����g�m�[�h�ɐݒ�
		current = *iter;
		// �I�[�v���Z�b�g����ł��R�X�g�̒Ⴂ�m�[�h������
		openSet.erase(iter);
		// �N���[�Y�h�Z�b�g��ݒ�
		current->mInOpenSet = false;
		current->mInClosedSet = true;
	} while (current != goal);

	// �S�[���������ꂽ���ۂ�
	return (current == goal) ? true : false;
}
// �o�H�^�C�����X�V
void Grid::UpdatePathTiles(class Tile* start)
{
	// �X�^�[�g�ƃG���h�ȊO�̃^�C���̏�Ԃ����Z�b�g
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	// start�̐e�m�[�h�ɏ�����(�t���Ȃ̂Őe�m�[�h�̓X�^�[�g�̉��̃m�[�h)
	Tile* t = start->mParent;
	// t���G���h�^�C���ɂȂ�܂Ńm�[�h�������̂ڂ�
	while (t != GetEndTile())
	{
		// �����̂ڂ����^�C���͌o�H�^�C���ɐݒ�
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}
// �������Ă�
void Grid::BuildTower()
{
	// �I�������^�C�����������Ă���Ȃ�
	if (mSelectedTile && !mSelectedTile->mBlocked)
	{
		// �I�������^�C���̏�Ԃ�blocked�ɕύX
		mSelectedTile->mBlocked = true;
		// �o�H�T���ŃX�^�[�g����S�[���܂œ��B�ł����Ȃ�
		if (FindPath(GetEndTile(), GetStartTile()))
		{
			// �������Ă�
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else
		{
			// �I�������^�C���ɓ������Ă�ƃS�[�����ł��Ȃ��̂ŁA�������ĂȂ��悤�ɂ���
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile()
{
	return mTiles[3][0];
}

Tile* Grid::GetEndTile()
{
	return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Is it time to spawn a new enemy?
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.0f)
	{
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}