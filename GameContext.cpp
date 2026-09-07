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

	Game_start = RM().GridAt(ResourceKeys::game_start);
	Gameover_background = RM().GridAt(ResourceKeys::gameover_background);
	Gameover_character_1 = RM().GridAt(ResourceKeys::gameover_character_1);
	Gameover_character_2 = RM().GridAt(ResourceKeys::gameover_character_2);
	/*Gameover_logo = RM().GridAt(ResourceKeys::gameover_logo);*/


	playerHP = 3;
	totalScore = 0;
	timer = GAME_TIME_LIMIT;

	sequenceState = SequenceState::Explanation;
	sequenceTimer = 0.0f;

	game_03.Init();
	game_02.Init();
	game_00.Init();
	isGameOverInput = false;
}

void GameContext::Reset()
{
	playerHP = 3;
	totalScore = 0;

	game_03.Reset();
	game_02.Reset();
	game_00.Reset();
	Isinit = 0;
	isGameOverInputCount = 0;

	sequenceState = SequenceState::Explanation;
	sequenceTimer = 0.0f;

	isGameOverInput = false;
}

void GameContext::Update(bool& input)
{
	float deltaTime = 1.0f / 60.0f;

	// --- 演出フェーズの更新 ---
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

		// 演出再生中はゲームの処理を行わずリターン
		return;
	}

	// --- メインゲームの更新（演出終了後のみ進行） ---
	timer -= deltaTime * TIME_SPEED_RATE;

	// 10秒経過したら次のゲームに移行＆演出リセット
	if (timer <= 0.0f)
	{
		timer = GAME_TIME_LIMIT;
		Isinit++;

		// 次のミニゲーム用に演出を初期化
		sequenceState = SequenceState::Explanation;
		sequenceTimer = 0.0f;
	}
	if (!(playerHP < 0)) {
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
			game_00.Update(playerHP, totalScore);
			break;

		default:
			break;
		}
	}

	if(playerHP < 0)
	{
		using namespace DxPlus::Input;
		int bottom = GetButtonDown(PLAYER1);
		if (bottom & BUTTON_START || bottom & BUTTON_TRIGGER2)
		{
			isGameOverInput = true;
			isGameOverInputCount++;
		}
		else if(isGameOverInput)
		{
			isGameOverInput = false;
		}

		if (isGameOverInputCount == isGameOverInputMax)
		{
			//タイトル画面(TitleScene)に戻る
			input = true;

			// ゲームオーバー入力カウントをリセット

			isGameOverInputCount = 0;

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

			if (sprTens) sprTens->Draw({ TIMER_POS.x - DIGIT_OFFSET_X * 0.5f, TIMER_POS.y });
			if (sprOnes) sprOnes->Draw({ TIMER_POS.x + DIGIT_OFFSET_X * 0.5f, TIMER_POS.y });
		}
		else
		{
			const auto* spr = RM().GridAt(ResourceKeys::number_countdown_b, displayTime, 0);
			if (spr) spr->Draw(TIMER_POS);
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
			animSpr->Draw(TIMER_POS);
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

//void GameContext::DrawGameOverLogo() const
//{
//	//gameover_logoの総フレーム52をループして描画する
//	int totalFrames = 36;
//
//	int currentFrame = static_cast<int>((10.0f - timer) * 20.0f) % totalFrames;
//
//	int animX = currentFrame % 10;
//
//	int animY = currentFrame / 10;
//
//	const auto* spr = RM().GridAt(ResourceKeys::gameover_logo, animX, animY);
//
//	if (spr) spr->Draw({ 960.0f, 540.0f });
//}

void GameContext::DrawSequenceUI() const
{
	switch (sequenceState)
	{
	case SequenceState::Explanation:
	{
		if (Game_setumei_2) Game_setumei_2->Draw(EXPLANATION_POS);
	}
	break;

	case SequenceState::Countdown:
	{
		// 3 -> 2 -> 1 の順に切り替え (0.0s~1.0s: '3' / 1.0s~2.0s: '2' / 2.0s~3.0s: '1')
		int countStep = static_cast<int>(sequenceTimer / COUNTDOWN_STEP_TIME);
		int animX = 2 - countStep; // コマ0='1', 1='2', 2='3' なので 3 からカウントダウン
		if (animX < 0) animX = 0;

		// ↓↓↓ 直接 RM().GridAt に animX を渡して描画します ↓↓↓
		const auto* spr = RM().GridAt(ResourceKeys::game_start123, animX, 0);
		if (spr) spr->Draw(COUNTDOWN_POS);
	}
	break;

	case SequenceState::StartMsg:
	{
		if (Game_start) Game_start->Draw(START_MSG_POS);
	}
	break;

	case SequenceState::Playing:

		break;
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
	DrawSequenceUI();

	// 最前面に演出画像を描画

	backgroundSpr2->Draw({ 0, 0 });
	DrawTimer();
	DrawHP();

	if(playerHP < 0)
	{
		backgroundSpr->Draw({});
		backgroundSpr2->Draw({ 0, 0 });
		Gameover_background->Draw({ 0, 0 });
		if (isGameOverInput == false)
		{
			Gameover_character_1->Draw({ 0, 0 });
		}
		if (isGameOverInput)
		{
			Gameover_character_2->Draw({ 0, 0 });
		}
		
		/*DrawGameOverLogo();*/
		
	}

}