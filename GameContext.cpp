// =============================
// Core/GameContext.cpp
// =============================
#include "GameContext.h"
#include <algorithm>
#include "Entity2D.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "Consts.h"
#include "Collision2D.h"

void GameContext::Init()
{
//    DxLib::SetMouseDispFlag(FALSE);
	backgroundSpr = RM().GridAt(ResourceKeys::Background);

	game_03.Init();
}

void GameContext::Reset()
{
	game_03.Reset();
}

void GameContext::Update()
{
	game_03.Update();
}

void GameContext::Draw() const
{
	backgroundSpr->Draw({});

	game_03.Draw();
}