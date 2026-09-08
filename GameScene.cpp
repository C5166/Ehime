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

    // 5つのミニゲームをクリアしたら GameClearScene へ遷移
    if (gameContext->IsSequenceFinished())
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

    int mouseX = 0;
    int mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

    // マウス左ボタンが押されているか判定
    bool isClicking = (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    // 押されている場合は cursor_2（グー）、離している場合は cursor_1（パー）
    const wchar_t* cursorKey = isClicking ? ResourceKeys::cursor_2 : ResourceKeys::cursor_1;

    const auto* sprite = RM().GridAt(cursorKey);
    if (sprite)
    {
        int handle = sprite->GetID();
        if (handle != -1)
        {
            DxLib::DrawGraph(mouseX - 35, mouseY - 10, handle, TRUE);
        }
    }
}

void GameScene::End()
{
    int bgm = RM().GetMusic(ResourceKeys::BGM_Game);
    if (bgm >= 0)
    {
        DxLib::StopSoundMem(bgm);
    }
}
