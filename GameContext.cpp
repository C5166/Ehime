#include "GameContext.h"
#include <algorithm>
#include "Entity2D.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "Consts.h"
#include "Collision2D.h"

void GameContext::Init()
{
	backgroundSpr = RM().GridAt(ResourceKeys::Background);
	backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2); 

	Game_setumei_2 = RM().GridAt(ResourceKeys::game_setumei_2);

	gamestart = RM().GetSound(ResourceKeys::SE_GameStart);


	Game_start = RM().GridAt(ResourceKeys::game_start);

	playerHP = 3;
	totalScore = 0;
	timer = GAME_TIME_LIMIT;

	sequenceState = SequenceState::Explanation;
	sequenceTimer = 0.0f;

	game_03.Init();
	game_02.Init();
	game_00.Init();
}

void GameContext::Reset()
{
	playerHP = 3;
	totalScore = 0;
	timer = GAME_TIME_LIMIT;

	game_03.Reset();
	game_02.Reset();
	game_00.Reset();
	
	sequenceState = SequenceState::Explanation;
	sequenceTimer = 0.0f;


}

void GameContext::Update(bool& input)
{
	float deltaTime = 1.0f / 60.0f;

	if (sequenceState != SequenceState::Playing)
	{
		sequenceTimer += deltaTime;

		switch (sequenceState)
		{
		case SequenceState::Explanation:
			if (sequenceTimer >= EXPLANATION_DURATION)
			{
				sequenceState = SequenceState::Countdown;
				sequenceTimer = 0.0f;
			}
			break;

		case SequenceState::Countdown:
			if (sequenceTimer >= COUNTDOWN_STEP_TIME * 3.0f)
			{
				sequenceState = SequenceState::StartMsg;
				sequenceTimer = 0.0f;
			}
			break;

		case SequenceState::StartMsg:
			if (sequenceTimer >= START_MSG_DURATION)
			{
				sequenceState = SequenceState::Playing;
				sequenceTimer = 0.0f;
			}
			break;
		}

		return;
	}

	timer -= deltaTime * TIME_SPEED_RATE;

	// 10秒経過で次のゲームへ移行＆演出リセット
	if (timer <= 0.0f)
	{
		timer = GAME_TIME_LIMIT;
		Isinit++;

		// 次のゲーム用にResetを実行してランダム要素・指示画像を再初期化
		switch (Isinit)
		{
		case GameNamber::Game_3: game_03.Reset(); break;
		case GameNamber::Game_2: game_02.Reset(); break;
		case GameNamber::Game_0: game_00.Reset(); break;
		}

		sequenceState = SequenceState::Explanation;
		sequenceTimer = 0.0f;
	}

	if (!(playerHP < 1)) {
		switch (Isinit)
		{
		case GameNamber::Game_3:
			game_03.Update(playerHP, totalScore);
			break;
		case GameNamber::Game_2:
			game_02.Update(playerHP, totalScore);
			break;
		case GameNamber::Game_0:
			game_00.Update(playerHP, totalScore);
			break;
		default:
			break;
		}
	}
}

void GameContext::DrawTimer() const
{
	if (timer > 3.0f)
	{
		int displayTime = static_cast<int>(std::ceil(timer));

		if (displayTime >= 10)
		{
			int tens = displayTime / 10;
			int ones = displayTime % 10;

			const auto* sprTens = RM().GridAt(ResourceKeys::number_countdown_b, tens, 0);
			const auto* sprOnes = RM().GridAt(ResourceKeys::number_countdown_b, ones, 0);

			if (sprTens) sprTens->Draw({ TIMER_POS.x - DIGIT_OFFSET_X * 0.5f, TIMER_POS.y },{0.7,0.7});
			if (sprOnes) sprOnes->Draw({ TIMER_POS.x + DIGIT_OFFSET_X * 0.5f, TIMER_POS.y },{ 0.7,0.7 });
		}
		else
		{
			const auto* spr = RM().GridAt(ResourceKeys::number_countdown_b, displayTime, 0);
			if (spr) spr->Draw(TIMER_POS,{ 0.7,0.7 });
		}
	}
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
			animSpr->Draw(TIMER_POS, { 0.7,0.7 });
		}
	}
}

void GameContext::DrawHP() const
{
	const int TOTAL_FRAMES = 15;
	const int COLUMNS = 10;

	int currentFrame = static_cast<int>((10.0f - timer) * 20.0f) % TOTAL_FRAMES;
	if (currentFrame < 0) currentFrame = 0;

	int animX = currentFrame % COLUMNS;
	int animY = currentFrame / COLUMNS;

	for (int i = 0; i < MAX_PLAYER_HP; ++i)
	{
		DxPlus::Vec2 pos = { HP_POS.x + i * HP_ICON_OFFSET_X, HP_POS.y };

		if (i < playerHP)
		{
			const auto* spr = RM().GridAt(ResourceKeys::game_hp_1, animX, animY);
			if (spr) spr->Draw(pos);
		}
		else
		{
			const auto* spr = RM().GridAt(ResourceKeys::game_hp_2, animX, animY);
			if (spr) spr->Draw(pos);
		}
	}
}

void GameContext::DrawSequenceUI() const
{
	switch (sequenceState)
	{
	case SequenceState::Explanation:
	{
		// Game_03 が選択されている場合は動的説明画像を描画
		if (Isinit == GameNamber::Game_3)
		{
			const auto* spr = game_03.GetExplanationSprite();
			if (spr) spr->Draw({ 0, 0 });
		}
		else
		{
			if (Game_setumei_2) Game_setumei_2->Draw({ 0,0 });
		}
	}
	break;

	case SequenceState::Countdown:
	{
		int countStep = static_cast<int>(sequenceTimer / COUNTDOWN_STEP_TIME);
		int animX = 2 - countStep;
		if (animX < 0) animX = 0;

		const auto* spr = RM().GridAt(ResourceKeys::game_start123, animX, 0);
		if (spr) spr->Draw(COUNTDOWN_POS);
	}
	break;

	case SequenceState::StartMsg:
	{
		if (Game_start) {
			Game_start->Draw(START_MSG_POS);
			if (CheckSoundMem(gamestart) == 0) {
				PlaySoundMem(gamestart, DX_PLAYTYPE_BACK);
			}
		}
	}
	break;

	case SequenceState::Playing:
		break;
	}
}

void GameContext::DrawGameOverUI() const
{
	int GameOverlogoCount{ 0 };
	int GameOverlogoTotalfram{ 0 };
	int GameOverlogoCountX{ 0 };
	int GameOverlogoCountY{ 0 };

	//Gameover_logoをループして描画させる
	if (GameOverlogoCountX < 12 && GameOverlogoCountY < 5)
	{
		GameOverlogoTotalfram++;
		GameOverlogoCountX++;
		if (GameOverlogoCountX > 10)
		{
			GameOverlogoCountX = 0;
			GameOverlogoCountY++;
			if (GameOverlogoTotalfram == 26)
			{
				GameOverlogoCountX = 0;
				GameOverlogoCountY = 0;
			}
			
		}
	}

	const auto* Gameover_logo = RM().GridAt(ResourceKeys::gameover_logo, GameOverlogoCountX, GameOverlogoCountY);

	if (Gameover_logo)
	{
		Gameover_logo->Draw({ 960, 540 });
	}
}



void GameContext::Draw() const
{
	backgroundSpr->Draw({});

	switch (Isinit)
	{
	case GameNamber::Game_3:
		game_03.Draw(playerHP, totalScore);
		DrawTimer();
		break;

	case GameNamber::Game_2:
		game_02.Draw(playerHP, totalScore);
		DrawTimer();
		break;

	case GameNamber::Game_0:
		game_00.Draw(playerHP, totalScore);
		break;

	default:
		break;
	}


	// 最前面に演出画像を描画
	DrawSequenceUI();
	backgroundSpr2->Draw({ 0, 0 });
	DrawTimer();
	DrawHP();
	
}