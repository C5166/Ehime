#include "GameClearScene.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "SceneManager.h"

void GameClearScene::Init()
{
    backgroundSpr = RM().GridAt(ResourceKeys::Background);
    backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2);
    gameclear_background = RM().GridAt(ResourceKeys::gameclear_background);
    gameclear_character_1 = RM().GridAt(ResourceKeys::gameclear_character_1);
    gameclear_character_2 = RM().GridAt(ResourceKeys::gameclear_character_2);

    isGameClearInput = false;
    isGameClearInputCount = 0;

    // アニメーション用タイマーのリセット
    logoAnimTimer = 0;
    currentLogoIndex = 0;

    StartFadeIn();
}

void GameClearScene::Update()
{
    using namespace DxPlus::Input;
    int buttonDown = GetButtonDown(PLAYER1);

    if (buttonDown & BUTTON_START || buttonDown & BUTTON_TRIGGER2)
    {
        isGameClearInput = true;
        isGameClearInputCount++;
    }
    else if (isGameClearInput)
    {
        isGameClearInput = false;
    }

    // カウント上限に達したら TitleScene へ遷移
    if (isGameClearInputCount >= isGameClearInputMax)
    {
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
    }

    // ロゴのアニメーションを更新（常にループ）
    UpdateGameClearLogoAnimation();
}

void GameClearScene::UpdateGameClearLogoAnimation()
{
    logoAnimTimer++;
    if (logoAnimTimer >= animFrameInterval)
    {
        logoAnimTimer = 0;
        currentLogoIndex++;

        // 総コマ数（21コマ）を超えたら 0 に戻してループさせる
        if (currentLogoIndex >= totalLogoFrames)
        {
            currentLogoIndex = 0;
        }
    }
}

void GameClearScene::DrawGameClearUI() const
{
    // 現在のインデックスから X (列) と Y (行) を計算
    int gridX = currentLogoIndex % logoColumns;
    int gridY = currentLogoIndex / logoColumns;

    const auto* Gameclear_logo = RM().GridAt(ResourceKeys::gameclear_logo, gridX, gridY);
    if (Gameclear_logo)
    {
        // 拡大率(logoScale)を指定して描画
        Gameclear_logo->Draw(logoPos, logoScale);
    }
}

void GameClearScene::Render() const
{
    if (backgroundSpr) backgroundSpr->Draw({});
    if (backgroundSpr2) backgroundSpr2->Draw({ 0, 0 });
    if (gameclear_background) gameclear_background->Draw({ 0, 0 });

    if (!isGameClearInput && gameclear_character_1)
    {
        gameclear_character_1->Draw({ 0, 0 });
    }
    else if (isGameClearInput && gameclear_character_2)
    {
        gameclear_character_2->Draw({ 0, 0 });
    }

    // ゲームクリアロゴを描画
    DrawGameClearUI();
}