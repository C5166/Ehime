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
	backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2);

	playerHP = 3;
	totalScore = 0;
	timer = GAME_TIME_LIMIT;

	game_03.Init();
	game_02.Init();
}

void GameContext::Reset()
{
	playerHP = 3;
	totalScore = 0;

	game_03.Reset();
	game_02.Reset();
	game_00.Reset();
	Isinit = 0;
}

void GameContext::Update()
{
	// タイマー減算（60FPS想定で 1/60 秒ずつ減算 * TIME_SPEED_RATE）
	// DxLibのデルタタイムが使える場合は `DxLib::GetDeltaTime()` 等をご利用ください
	float deltaTime = 1.0f / 60.0f;
	timer -= deltaTime * TIME_SPEED_RATE;

	// 10秒が経過（0以下）したら次のゲームに移行
	if (timer <= 0.0f)
	{
		timer = GAME_TIME_LIMIT; // 次のゲーム用にタイマーリセット
		Isinit++;
	}

	// 各ゲームの更新

	switch (Isinit)
	{
	case GameNamber::Game_3:
			game_03.Update(playerHP, totalScore);
			break;

	case GameNamber::Game_2:
		game_02.Update(playerHP, totalScore);
		break;

	case GameNamber::Game_0:
			game_00.Update(playerHP,totalScore);
			break;
	default:

		break;
	}

}

void GameContext::DrawTimer() const
{
	// 1. 残り時間が3秒より大きい場合（通常数字表示）
	if (timer > 3.0f)
	{
		int displayTime = static_cast<int>(std::ceil(timer));

		// 10以上の場合は2桁表示
		if (displayTime >= 10)
		{
			int tens = displayTime / 10;
			int ones = displayTime % 10;

			const auto* sprTens = RM().GridAt(ResourceKeys::number_countdown_b, tens, 0);
			const auto* sprOnes = RM().GridAt(ResourceKeys::number_countdown_b, ones, 0);

			// TIMER_POS を基準に左右にオフセット配置
			if (sprTens) sprTens->Draw({ TIMER_POS.x - DIGIT_OFFSET_X * 0.5f, TIMER_POS.y });
			if (sprOnes) sprOnes->Draw({ TIMER_POS.x + DIGIT_OFFSET_X * 0.5f, TIMER_POS.y });
		}
		else // 4〜9の1桁表示
		{
			const auto* spr = RM().GridAt(ResourceKeys::number_countdown_b, displayTime, 0);
			if (spr) spr->Draw(TIMER_POS);
		}
	}
	// 2. 残り時間が3秒以下の場合（3・2・1・0 のアニメーション表示）
	else
	{
		float elapsedSec = 3.0f - std::max(0.0f, timer);
		int frameIndex = static_cast<int>(elapsedSec * 40.0f);
		frameIndex = std::clamp(frameIndex, 0, 182);

		int gridX = frameIndex % 10;
		int gridY = frameIndex / 10;

		const auto* animSpr = RM().GridAt(ResourceKeys::number_countdown_321, gridX, gridY);
		if (animSpr)
		{
			animSpr->Draw(TIMER_POS);
		}
	}
}

void GameContext::Draw() const
{
	backgroundSpr->Draw({});

	switch (Isinit)
	{
	case GameNamber::Game_3:
		game_03.Draw(playerHP, totalScore);
		backgroundSpr2->Draw({ 0, 0 });
		break;

	case GameNamber::Game_2:
		game_02.Draw(playerHP, totalScore);
		backgroundSpr2->Draw({ 0, 0 });
		break;

	case GameNamber::Game_0:
		game_00.Draw(playerHP, totalScore);
		backgroundSpr2->Draw({ 0, 0 });
		break;
	default:

		break;
	}

	// 従来の DxLib::DrawFormatString の代わりに画像タイマー描画を呼び出す
	DrawTimer();
}