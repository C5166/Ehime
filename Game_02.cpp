#include "Game_02.h"
#include "ResourceKeys.h"
#include "ResourceManager.h"
#include <cstdlib>


void Game_02::Init()
{
    Reset();
	/*background = DxLib::LoadGraph(L"./Data/Images/mihon_3.png");*/
    a[0] = DxLib::LoadGraph(L"./Data/Images/game2_1.png");
    a[1] = DxLib::LoadGraph(L"./Data/Images/game2_2.png");
    a[2] = DxLib::LoadGraph(L"./Data/Images/game2_3.png");
    a[3] = DxLib::LoadGraph(L"./Data/Images/game2_4.png");
    a[4] = DxLib::LoadGraph(L"./Data/Images/game2_5.png");
    a[5] = DxLib::LoadGraph(L"./Data/Images/game2_6.png");

	good[0] = RM().GetSound(ResourceKeys::SE_GoodVoice1);
	good[1] = RM().GetSound(ResourceKeys::SE_GoodVoice2);
	good[2] = RM().GetSound(ResourceKeys::SE_GoodVoice3);

	waku = DxLib::LoadGraph(L"./Data/Images/title_frame_2.png");

	perfectSheetID = DxLib::LoadGraph(L"./Data/Images/game_perfect.png");
	notsheetID = DxLib::LoadGraph(L"./Data/Images/game_error.png");

    poti = RM().GetSound(ResourceKeys::SE_poti);

    bubu = RM().GetSound(ResourceKeys::SE_bubu);

    perfect = RM().GetSound(ResourceKeys::SE_PerfectVoice);

    setumeivoice[0] = RM().GetSound(ResourceKeys::SE_GameVoice2_1);
    setumeivoice[1] = RM().GetSound(ResourceKeys::SE_GameVoice2_2);

}

void Game_02::Reset()
{
	IsVoice = true;
}


void Game_02::Update(int& hp, int& score)
{
    frame = 0;
    using namespace DxPlus::Input;
    int button = GetButtonDown(PLAYER1);
    bool isClicked = (button & BUTTON_TRIGGER2);

    int mouseX = 0, mouseY = 0;

    frame++;
    switch (change)
    {
    case 0:
        if (isClicked)
        {
            PlaySoundMem(poti, DX_PLAYTYPE_BACK);
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if(mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {

                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);

                hp--;
                change ++;
            }
			else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);

                score++;
                change ++;
			}
			Animating = true;

        }
        break;
	case 1:
        if (isClicked)
        {
            PlaySoundMem(poti, DX_PLAYTYPE_BACK);
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);

                score++;
                change++;
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {

                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);

                hp--;
                change++;
            }
            Animating = true;
        }
		break;
    case 2:
        if (isClicked)
        {
            PlaySoundMem(poti, DX_PLAYTYPE_BACK);
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);

                score++;
                change ++;
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {

                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);

                hp--;
                change ++;
            }
            Animating = true;
        }
	    break;
	case 3:
        if (isClicked)
        {
            PlaySoundMem(poti, DX_PLAYTYPE_BACK);
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);

                score++;
                change ++;
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {

                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);

                hp--;
                change ++;
            }
            Animating = true;
        }
		break;
	case 4:
        if (isClicked)
        {
            PlaySoundMem(poti, DX_PLAYTYPE_BACK);
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);

                score++;
                change++;
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {

                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);

                hp--;
                change++;
            }
            Animating = true;
        }
		break;
	case 5:
        if (isClicked)
        {
            PlaySoundMem(poti, DX_PLAYTYPE_BACK);
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);

                score++;
                change = 0;
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {

                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);

                hp--;
                change = 0;
            }
        }
		break;
    default:
        break;
    }
}

void Game_02::Draw(int hp, int score) const
{
    DrawGraph(157, 320, a[change], TRUE);
    DrawGraph(0, 0, waku, TRUE);
}