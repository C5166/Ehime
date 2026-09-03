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
}

void GameContext::Reset()
{
}

void GameContext::Update()
{

}

void GameContext::Draw() const
{
	backgroundSpr->Draw({});

}