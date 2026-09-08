#include "GameContext.h"
#include <algorithm>
#include <cstdlib>
#include "Entity2D.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "Consts.h"
#include "Collision2D.h"

void GameContext::GenerateRandomGameQueue()
{
	gameQueue.clear();
	static const GameNamber availableGames[] = { GameNamber::Game_0, GameNamber::Game_2, GameNamber::Game_3 };

	for (int i = 0; i < TOTAL_MINI_GAMES; ++i)
	{
		int randomIndex = std::rand() % 3;
		gameQueue.push_back(availableGames[randomIndex]);
	}
}

void GameContext::SetupCurrentGame()
{
	if (currentGameIndex >= static_cast<int>(gameQueue.size())) return;

	Isinit = gameQueue[currentGameIndex];

	switch (Isinit)
	{
	case GameNamber::Game_3: game_03.Reset(); break;
	case GameNamber::Game_2: game_02.Reset(); break;
	case GameNamber::Game_0: game_00.Reset(); break;
	}
}

void GameContext::AdvanceToNextGame(bool perfectAchieved)
{
	if (!perfectAchieved && playerHP > 0)
	{
		playerHP -= 1;
	}

	showPerfect = false;
	perfectFrame = 0;
	perfectTimer = 0;
	perfectElapsedFrames = 0;
	perfectAlpha = 255;

	timer = GAME_TIME_LIMIT;
	currentGameIndex++;

	if (currentGameIndex >= TOTAL_MINI_GAMES)
	{
		sequenceFinished = true;
		return;
	}

	SetupCurrentGame();
	sequenceState = SequenceState::Explanation;
	sequenceTimer = 0.0f;
}

void GameContext::DrawStageProgress() const
{
	const int TOTAL_FRAMES = 23;
	const int COLUMNS = 10;

	// タイマーに連動したアニメーションフレーム計算 (0〜22コマ)
	int currentFrame = static_cast<int>((10.0f - timer) * 20.0f) % TOTAL_FRAMES;
	if (currentFrame < 0) currentFrame = 0;

	int animX = currentFrame % COLUMNS;
	int animY = currentFrame / COLUMNS;

	// TOTAL_MINI_GAMES (5回分) を右から左（または左から右）へ並べて描画
	// 画像サンプルに合わせて右側から古いステージ（クリア済み）として描画する配置例
	for (int i = 0; i < TOTAL_MINI_GAMES; ++i)
	{
		// 右から左に並べる場合: (TOTAL_MINI_GAMES - 1 - i)
		DxPlus::Vec2 pos = {
			STAGE_PROGRESS_POS.x + (TOTAL_MINI_GAMES - 1 - i) * STAGE_PROGRESS_OFFSET_X,
			STAGE_PROGRESS_POS.y
		};

		// クリア済み・通過済み(i < currentGameIndex) は game_stage_2（暗い矢印）
		// 未クリア・挑戦中(i >= currentGameIndex) は game_stage_1（明るい矢印）
		if (i < currentGameIndex)
		{
			const auto* spr = RM().GridAt(ResourceKeys::game_stage_2, animX, animY);
			if (spr) spr->Draw(pos); // スケールは表示サイズに合わせて調整
		}
		else
		{
			const auto* spr = RM().GridAt(ResourceKeys::game_stage_1, animX, animY);
			if (spr) spr->Draw(pos);
		}
	}
}

void GameContext::Init()
{
	backgroundSpr = RM().GridAt(ResourceKeys::Background);
	backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2);

	Game_setumei_2 = RM().GridAt(ResourceKeys::game_setumei_2);

	gamestart = RM().GetSound(ResourceKeys::SE_GameStart);

	Game_start = RM().GridAt(ResourceKeys::game_start);

	CountSE_1 = RM().GetSound(ResourceKeys::SE_Count_1);
	CountSE_2 = RM().GetSound(ResourceKeys::SE_Count_2);

	game_03.Init();
	game_02.Init();
	game_00.Init();

	Reset();

	// game_perfect スプライトをロード（2行 x 10列、合計17コマ）
	perfectSheetID = DxLib::LoadGraph(L"./Data/Images/game_perfect.png");
	if (perfectSheetID >= 0)
	{
		int w = 0, h = 0;
		DxLib::GetGraphSize(perfectSheetID, &w, &h);
		if (w > 0 && h > 0)
		{
			perfectFrameW = w / perfectColumns;
			perfectFrameH = h / 2; // 2行構成
		}
	}
}

void GameContext::Reset()
{
	playerHP = 3;
	totalScore = 0;
	timer = GAME_TIME_LIMIT;
	currentGameIndex = 0;

	GenerateRandomGameQueue();
	SetupCurrentGame();

	sequenceState = SequenceState::Explanation;
	sequenceTimer = 0.0f;

	// リセット時にシーケンスフラグや完了演出をリセット
	sequenceFinished = false;
	showPerfect = false;
	perfectFrame = 0;
	perfectTimer = 0;
	perfectElapsedFrames = 0;
	perfectAlpha = 255;
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

	// game_perfect 表示中はゲーム進行を止める
	if (showPerfect)
	{
		perfectTimer++;
		if (perfectTimer >= perfectAnimInterval)
		{
			perfectTimer = 0;
			perfectFrame = (perfectFrame + 1) % perfectTotalFrames;
		}

		perfectElapsedFrames++;

		if (perfectElapsedFrames < perfectFadeFrames)
		{
			perfectAlpha = static_cast<int>(255.0f * (static_cast<float>(perfectElapsedFrames) / perfectFadeFrames));
		}
		else if (perfectElapsedFrames > (perfectDisplayFrames - perfectFadeFrames))
		{
			int remain = perfectDisplayFrames - perfectElapsedFrames;
			perfectAlpha = static_cast<int>(255.0f * (static_cast<float>(remain) / perfectFadeFrames));
			if (perfectAlpha < 0) perfectAlpha = 0;
		}
		else
		{
			perfectAlpha = 255;
		}

		if (perfectElapsedFrames >= perfectDisplayFrames)
		{
			AdvanceToNextGame(true);
		}

		return;
	}

	// ふつうのプレイ中だけゲームを更新
	if (playerHP >= 1 && !sequenceFinished)
	{
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

		// 各ミニゲームの完了判定 -> 完了演出を開始
		if (!showPerfect)
		{
			if (Isinit == GameNamber::Game_0)
			{
				if (game_00.AllTargetsCollected())
				{
					showPerfect = true;
					perfectFrame = 0;
					perfectTimer = 0;
					perfectElapsedFrames = 0;
				}
			}
			else if (Isinit == GameNamber::Game_2)
			{
				if (game_02.AllCorrect())
				{
					showPerfect = true;
					perfectFrame = 0;
					perfectTimer = 0;
					perfectElapsedFrames = 0;
				}
			}
			else if (Isinit == GameNamber::Game_3)
			{
				if (game_03.AllTargetsCollected())
				{
					showPerfect = true;
					perfectFrame = 0;
					perfectTimer = 0;
					perfectElapsedFrames = 0;
				}
			}
		}
	}

	if (showPerfect || sequenceFinished)
	{
		return;
	}

	timer -= deltaTime * TIME_SPEED_RATE;

	int currentSecond = static_cast<int>(std::ceil(timer));
	// 秒数が前回の判定から変化した瞬間に音を鳴らす
	if (currentSecond != lastPlayTime && currentSecond >= 0)
	{
		if (currentSecond <= 3 && currentSecond > 0)
		{
			// 残り3秒以下のカウントダウン音
			PlaySoundMem(CountSE_2, DX_PLAYTYPE_BACK);
		}
		else if (currentSecond > 3)
		{
			// 通常のカウントダウン音
			PlaySoundMem(CountSE_1, DX_PLAYTYPE_BACK);
		}

		// 再生した秒数を記録
		lastPlayTime = currentSecond;
	}
	// 10秒経過で次のミニゲームへ移行
	if (timer <= 0.0f)
	{
		AdvanceToNextGame(false);
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

			if (sprTens) sprTens->Draw({ TIMER_POS.x - DIGIT_OFFSET_X * 0.5f, TIMER_POS.y }, { 0.7,0.7 });
			if (sprOnes) sprOnes->Draw({ TIMER_POS.x + DIGIT_OFFSET_X * 0.5f, TIMER_POS.y }, { 0.7,0.7 });
		}
		else
		{
			const auto* spr = RM().GridAt(ResourceKeys::number_countdown_b, displayTime, 0);
			if (spr) spr->Draw(TIMER_POS, { 0.7,0.7 });
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
		GameNamber currentGame = static_cast<GameNamber>(Isinit);

		if (currentGame == GameNamber::Game_3)
		{
			const auto* spr = game_03.GetExplanationSprite();
			if (spr) spr->Draw({ 0, 0 });
		}
		else if (currentGame == GameNamber::Game_2)
		{
			const auto* spr = game_02.GetExplanationSprite();
			if (spr) spr->Draw({ 0, 0 });
		}
		else if (currentGame == GameNamber::Game_0)
		{
			const auto* spr = game_00.GetExplanationSprite();
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
	backgroundSpr->Draw({}); // 背景

	// プレイ状態のときだけゲーム本体を描画
	if (sequenceState == SequenceState::Playing)
	{
		switch (Isinit)
		{
		case GameNamber::Game_3:
			game_03.Draw(playerHP, totalScore);
			break;
		case GameNamber::Game_2:
			game_02.Draw(playerHP, totalScore);
			break;
		case GameNamber::Game_0:
			game_00.Draw(playerHP, totalScore);
			break;
		default:
			break;
		}
	}

	// 枠・タイマー・HPの描画
	backgroundSpr2->Draw({ 0, 0 });
	// =========================================================
	// 最前列（画面の一番手前）に Game_00 のターゲットアイコンを描画
	// =========================================================
	if (Isinit == GameNamber::Game_0)
	{
		const auto* targetSpr = game_00.GetTargetSprite();
		//if (targetSpr)
		//{
		//	// アルファブレンド等の状態をリセット
		//	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	DxPlus::Vec2 targetPos = { 100.0f, DxPlus::CLIENT_HEIGHT - 50.0f };
		//	DxPlus::Vec2 targetScale = { 0.5f, 0.5f };

		//	targetSpr->Draw(targetPos, targetScale);
		//}

		const auto* moziSpr = game_00.GetMoziSprite();
		if (moziSpr)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// アイコンの右隣（X: 300など）に調整して描画
			DxPlus::Vec2 moziPos = { 300.0f, DxPlus::CLIENT_HEIGHT - 50.0f };
			DxPlus::Vec2 moziScale = { 1.0f, 1.0f }; // 画像サイズに合わせて変更

			moziSpr->Draw(moziPos, moziScale);
		}


	}
	DrawTimer();
	DrawHP();

	DrawStageProgress();

	// 最前面に説明・カウントダウン画像を描画
	DrawSequenceUI();


	// 完了演出の描画
    if (showPerfect && perfectSheetID >= 0)
	{
		int frameIndex = perfectFrame;
		if (frameIndex < perfectTotalFrames)
		{
			int gridX = frameIndex % perfectColumns;
			int gridY = frameIndex / perfectColumns;
			int sx = gridX * perfectFrameW;
			int sy = gridY * perfectFrameH;
			int dx = 960 - perfectFrameW / 2;
			int dy = 540 - perfectFrameH / 2;
			// フェード用アルファを設定してから描画
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, perfectAlpha);
			DrawRectGraph(dx, dy, sx, sy, perfectFrameW, perfectFrameH, perfectSheetID, TRUE);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}