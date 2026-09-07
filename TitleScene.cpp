// =============================
// Scenes/Title/TitleScene.cpp
// =============================
#include "TitleScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ResourceKeys.h"
#include "GameContext.h"

void TitleScene::Init()
{
    DxLib::SetBackgroundColor(16, 128, 224);
    frameCount = 0;

	startvoice = RM().GetSound(ResourceKeys::SE_StartVoice);

    fontHandle = RM().GetFont(ResourceKeys::Font_Title);

    bgSprite = RM().GridAt(ResourceKeys::Title_frame);
	title_character = RM().GridAt(ResourceKeys::title_character);
	title_character2 = RM().GridAt(ResourceKeys::title_character_2);

	isTitleInput = false;
	isTitleInputCount = 0;

	poti = RM().GetSound(ResourceKeys::SE_poti);
	kirakira = RM().GetSound(ResourceKeys::SE_kirakira);

    touch[0] = RM().GetSound(ResourceKeys::SE_TouchVoice1);
    touch[1] = RM().GetSound(ResourceKeys::SE_TouchVoice2);
    touch[2] = RM().GetSound(ResourceKeys::SE_TouchVoice3);
    touch[3] = RM().GetSound(ResourceKeys::SE_TouchVoice4);
    touch[4] = RM().GetSound(ResourceKeys::SE_TouchVoice5);

	//titlecharacter[0] = LoadGraph(L"./Data/Images/title_character.png");
	//titlecharacter[1] = LoadGraph(L"./Data/Images/title_character_2.png");

   /* LoadDivGraph(L"./Data/Images/title_background.png", 84, 10, 8, 960, 540, titlebackground);*/

    int bgm = RM().GetMusic(ResourceKeys::BGM_Game);
    if (bgm >= 0)
    {
        DxLib::StopSoundMem(bgm);
        DxLib::ChangeVolumeSoundMem(128, bgm);
        DxLib::PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
    }
    StartFadeIn();
}

void TitleScene::Update()
{
    using namespace DxPlus::Input;
    if (GetButtonDown(PLAYER1) & BUTTON_START||GetButtonDown(PLAYER1)& BUTTON_TRIGGER2)
    {
        PlaySoundMem(poti, DX_PLAYTYPE_BACK);
        if(TenCount == 10)
        {
            Scene* gameScene = SceneManager::GetInstance().GetScene(SceneID::Game);
            SetNextScene(gameScene);
			TenCount = 0;
            StartFadeOut();
            if (CheckSoundMem(kirakira) == 0) {
                PlaySoundMem(kirakira, DX_PLAYTYPE_BACK);
            }
			WaitTimer(1500);
            if (CheckSoundMem(startvoice) == 0) {
                PlaySoundMem(startvoice, DX_PLAYTYPE_BACK);
            }
		}

        else
        {
            a++;
            PlaySoundMem(touch[a], DX_PLAYTYPE_BACK);
            if(a >= 4)
            {
                a = 0;
			}
        }

		TenCount++;

        if (isTitleInput == false)
        {
            isTitleInput = true;
        }
        
        return;
    }

    if (isTitleInput)
    {
		isTitleInputCount++;
    }

    if (isTitleInput && isTitleInputCount > isTitleInputMax)
    {
        isTitleInput = false;
		isTitleInputCount = 0;
    }

    frameCount++;
}

void TitleScene::Render() const
{

    if (bgSprite)
    {
        bgSprite->Draw({ 0, 0 });
    }
    const int white = DxLib::GetColor(255, 255, 255);
    DxPlus::Text::DrawString(L"ehime",
        { DxPlus::CLIENT_WIDTH * 0.5f, DxPlus::CLIENT_HEIGHT * 0.25f },
        white, DxPlus::Text::TextAlign::MIDDLE_CENTER, { 2, 2 }, 0, fontHandle);

    const int yellow = DxLib::GetColor(255, 255, 0);
    if (frameCount & 0x20)
    {
        DxPlus::Text::DrawString(L"Push Enter",
            { DxPlus::CLIENT_WIDTH * 0.5f, DxPlus::CLIENT_HEIGHT * 0.75f },
            yellow, DxPlus::Text::TextAlign::MIDDLE_CENTER, { 1,1 }, 0, fontHandle);
    }
    if (isTitleInput)
    {
        title_character2->Draw({ 960, 540 });
    }
    else if (!isTitleInput)
    {
        title_character->Draw({ 960, 540 });
    }
}

void TitleScene::Draw() const
{

    /*while (TenCount <= 10)
    {
        i++;
        DrawGraph(0, 0, titlebackground[i], FALSE);

        if (i >= 84) i = 1;
    }

    for (i = 0; i < 84; i++)
    {
        DeleteGraph(titlebackground[i]);
    }*/
    
}
