// =============================
// Scenes/Game/GameScene.cpp
// =============================
#include "GameScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"

void GameScene::Init()
{
    DxLib::SetBackgroundColor(64, 64, 128);
    gameContext->Reset();

    int bgm = RM().GetMusic(ResourceKeys::BGM_Game);
    if (bgm >= 0)
    {
        DxLib::StopSoundMem(bgm);
        DxLib::ChangeVolumeSoundMem(128, bgm);
        DxLib::PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
    }

	isGameOverInput = false;

    StartFadeIn();
}

void GameScene::Update()
{
	gameContext->Update(isGameOverInput);

    using namespace DxPlus::Input;
    int buttonDown = GetButtonDown(PLAYER1);

    if (gameContext->GetHP() < 1)
    {
        Scene* gameOverScene = SceneManager::GetInstance().GetScene(SceneID::GameOver);
        SetNextScene(gameOverScene);
        StartFadeOut();
        return;
    }

    if (gameContext->GetCurrentMiniGame() == GameNamber::Game_3 && gameContext->GetScore() >= 10)
    {
        Scene* gameClearScene = SceneManager::GetInstance().GetScene(SceneID::GameClear);
        SetNextScene(gameClearScene);
        StartFadeOut();
        return;
    }
}

void GameScene::Render() const
{
    gameContext->Draw();
}

void GameScene::End()
{
    int bgm = RM().GetMusic(ResourceKeys::BGM_Game);
    if (bgm >= 0)
    {
        DxLib::StopSoundMem(bgm);
    }
}
