#include "GameOverScene.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "SceneManager.h"

void GameOverScene::Init()
{
    backgroundSpr = RM().GridAt(ResourceKeys::Background);
    backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2);
    gameover_background = RM().GridAt(ResourceKeys::gameover_background);
    gameover_character_1 = RM().GridAt(ResourceKeys::gameover_character_1);
    gameover_character_2 = RM().GridAt(ResourceKeys::gameover_character_2);

    isGameOverInput = false;
    isGameOverInputCount = 0;

    // アニメーション用タイマーのリセット
    logoAnimTimer = 0;
    currentLogoIndex = 0;

    StartFadeIn();
}

void GameOverScene::Update()
{
    using namespace DxPlus::Input;
    int buttonDown = GetButtonDown(PLAYER1);

    if (buttonDown & BUTTON_START || buttonDown & BUTTON_TRIGGER2)
    {
        isGameOverInput = true;
        isGameOverInputCount++;
    }
    else if (isGameOverInput)
    {
        isGameOverInput = false;
    }

    // カウント上限に達したら TitleScene へ遷移
    if (isGameOverInputCount >= isGameOverInputMax)
    {
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
    }

    // ロゴのアニメーションを更新（常にループ）
    UpdateGameOverLogoAnimation();
}

void GameOverScene::UpdateGameOverLogoAnimation()
{
    logoAnimTimer++;
    if (logoAnimTimer >= animFrameInterval)
    {
        logoAnimTimer = 0;
        currentLogoIndex++;

        // 総コマ数（26）を超えたら 0 に戻して永遠にループさせる
        if (currentLogoIndex >= totalLogoFrames)
        {
            currentLogoIndex = 0;
        }
    }
}

void GameOverScene::DrawGameOverUI() const
{
    // 現在のインデックスから X (列) と Y (行) を計算
    int gridX = currentLogoIndex % logoColumns;
    int gridY = currentLogoIndex / logoColumns;

    const auto* Gameover_logo = RM().GridAt(ResourceKeys::gameover_logo, gridX, gridY);
    if (Gameover_logo)
    {
        // 拡大率(logoScale)を指定して描画
        Gameover_logo->Draw(logoPos, logoScale);
    }
}

void GameOverScene::Render() const
{
    if (backgroundSpr) backgroundSpr->Draw({});
    if (backgroundSpr2) backgroundSpr2->Draw({ 0, 0 });
    if (gameover_background) gameover_background->Draw({ 0, 0 });

    if (!isGameOverInput && gameover_character_1)
    {
        gameover_character_1->Draw({ 0, 0 });
    }
    else if (isGameOverInput && gameover_character_2)
    {
        gameover_character_2->Draw({ 0, 0 });
    }

    DrawGameOverUI();
}