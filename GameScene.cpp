// =============================
// Scenes/Game/GameScene.cpp
// =============================
#include "GameScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"

void GameScene::Init()
{
    // OS標準のマウスカーソルを非表示にする
    DxLib::SetMouseDispFlag(FALSE);

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
    // --- デバッグ用パラメータ調整操作 ---
    // [UP / DOWN]: マウス描画サイズ（スケール）の変更
    if (DxLib::CheckHitKey(KEY_INPUT_UP))   mouseScale += 0.05f;
    if (DxLib::CheckHitKey(KEY_INPUT_DOWN)) mouseScale = (std::max)(0.1f, mouseScale - 0.05f);

    // [RIGHT / LEFT]: マウス判定半径の変更
    if (DxLib::CheckHitKey(KEY_INPUT_RIGHT)) mouseCollisionRadius += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_LEFT))  mouseCollisionRadius = (std::max)(1.0f, mouseCollisionRadius - 1.0f);

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

    // --- 当たり判定ガイドライン（緑の円）の描画 ---
 /*   DxLib::DrawCircle(mouseX, mouseY, static_cast<int>(mouseCollisionRadius), DxLib::GetColor(0, 255, 0), FALSE);*/

    // --- デバッグ情報描画 ---
  /*  int white = DxLib::GetColor(255, 255, 255);
    DxLib::DrawFormatString(10, 10, white, L"Mouse Scale [UP/DOWN]: %.2f", mouseScale);
    DxLib::DrawFormatString(10, 30, white, L"Mouse Radius [LEFT/RIGHT]: %.1f", mouseCollisionRadius);*/

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
            // スケールを指定して中心位置基準で回転・拡大縮小描画
            DxLib::DrawRotaGraph(mouseX, mouseY, static_cast<double>(mouseScale), 0.0, handle, TRUE);
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