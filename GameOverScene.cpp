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


    touch[0] = RM().GetSound(ResourceKeys::SE_TouchVoice1);
    touch[1] = RM().GetSound(ResourceKeys::SE_TouchVoice2);
    touch[2] = RM().GetSound(ResourceKeys::SE_TouchVoice3);
    touch[3] = RM().GetSound(ResourceKeys::SE_TouchVoice4);
    touch[4] = RM().GetSound(ResourceKeys::SE_TouchVoice5);

    poti = RM().GetSound(ResourceKeys::SE_poti);
    kirakira = RM().GetSound(ResourceKeys::SE_kirakira);


	voice = RM().GetSound(ResourceKeys::SE_GameOverVoice);
    isGameOverInput = false;
    isGameOverInputCount = 0;
	bgm = RM().GetMusic(ResourceKeys::SE_GameOver);
    if (bgm >= 0)
    {
        PlaySoundMem(voice, DX_PLAYTYPE_BACK);
        DxLib::ChangeVolumeSoundMem(128, bgm);
        DxLib::PlaySoundMem(bgm, DX_PLAYTYPE_BACK);
    }
    StartFadeIn();
}

void GameOverScene::Update()
{
    using namespace DxPlus::Input;
    int buttonDown = GetButtonDown(PLAYER1);

    if (buttonDown & BUTTON_START || buttonDown & BUTTON_TRIGGER2)
    {
        if(isGameOverInputCount<= isGameOverInputMax)
            {
                a++;
                PlaySoundMem(touch[a], DX_PLAYTYPE_BACK);
                if (a >= 4)
                {
                    a = 0;
                }
            }

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
        DxLib::StopSoundMem(bgm);
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
        if (CheckSoundMem(kirakira) == 0) {
            PlaySoundMem(kirakira, DX_PLAYTYPE_BACK);
        }
    }
   
}

void GameOverScene::DrawGameOverUI() const
{
    static int GameOverlogoTotalfram = 0;
    static int GameOverlogoCountX = 0;
    static int GameOverlogoCountY = 0;



    // Gameover_logoのアニメーション計算
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
                GameOverlogoTotalfram = 0;
            }
        }
    }

    const auto* Gameover_logo = RM().GridAt(ResourceKeys::gameover_logo, GameOverlogoCountX, GameOverlogoCountY);
    if (Gameover_logo)
    {
        Gameover_logo->Draw({ 960, 540 });
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