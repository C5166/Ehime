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
}