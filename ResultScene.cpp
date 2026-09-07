// =============================
// Scenes/Result/ResultScene.cpp
// =============================
#include "ResultScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"

void ResultScene::Init()
{
    DxLib::SetBackgroundColor(128, 64, 0);
    fontHandle = RM().GetFont(ResourceKeys::Font_Title);
	Gameover_background = RM().GridAt(ResourceKeys::gameover_background);
	Gameover_background = RM().GridAt(ResourceKeys::gameover_background);
    StartFadeIn(0.0f);
}

void ResultScene::Update()
{
    using namespace DxPlus::Input;
    if (GetButtonDown(PLAYER1) & BUTTON_START)
    {
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
        return;
    }
}

void ResultScene::Render() const
{
    gameContext->Draw();
    
    Gameover_background->Draw({0,0});

		gameover_character_1->Draw({ 0, 0 });


 
}
