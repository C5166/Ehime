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
	game_02.Init();
}

void GameContext::Reset()
{
	game_03.Reset();
	Isinit = false;
}

void GameContext::Update()
{
	using namespace DxPlus::Input;
	int Button = GetButtonDown(PLAYER1);

	if(Isinit == false)
	{
		game_03.Update();
	}
	else if (Isinit == true)
	{
		game_02.Update();
	}

	if(Button& BUTTON_START)
	{
		Isinit = true;
	}
}

void GameContext::Draw() const
{
	backgroundSpr->Draw({});
	if (Isinit == false)
	{
		game_03.Draw();
	}
	else if (Isinit == true)
	{
		game_02.Draw();
	}
}