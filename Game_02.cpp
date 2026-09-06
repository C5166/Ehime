#include "Game_02.h"
#include "ResourceKeys.h"
#include "ResourceManager.h"
#include <cstdlib>

void Game_02::Init()
{
    Reset();
	mihon_3 = RM().GridAt(ResourceKeys::game2_back, 0, 0);
	/*background = DxLib::LoadGraph(L"./Data/Images/mihon_3.png");*/
    a[0] = DxLib::LoadGraph(L"./Data/Images/question1.png");
    a[1] = DxLib::LoadGraph(L"./Data/Images/question2.png");
    a[2] = DxLib::LoadGraph(L"./Data/Images/question3.png");
    a[3] = DxLib::LoadGraph(L"./Data/Images/question4.png");
    a[4] = DxLib::LoadGraph(L"./Data/Images/question5.png");
    a[5] = DxLib::LoadGraph(L"./Data/Images/question6.png");
}

void Game_02::Reset()
{
   
}


void Game_02::Update(int& hp, int& score)
{
    using namespace DxPlus::Input;
    int button = GetButtonDown(PLAYER1);
    bool isClicked = (button & BUTTON_TRIGGER2);

    int mouseX = 0, mouseY = 0;

    

    switch (change)
    {
    case 0:
        if (isClicked)
        {
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if(mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                hp--;
                change = GetRand(5);
            }
			else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                score++;
                change = GetRand(5);
			}
        }
        break;
	case 1:
        if (isClicked)
        {
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                score++;
                change = GetRand(5);
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                hp--;
                change = GetRand(5);
            }
        }
		break;
    case 2:
        if (isClicked)
        {
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                score++;
                change = GetRand(5);
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                hp--;
                change = GetRand(5);
            }
        }
	    break;
	case 3:
        if (isClicked)
        {
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                score++;
                change = GetRand(5);
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                hp--;
                change = GetRand(5);
            }
        }
		break;
	case 4:
        if (isClicked)
        {
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                score++;
                change = GetRand(5);
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                hp--;
                change = GetRand(5);
            }
        }
		break;
	case 5:
        if (isClicked)
        {
            DxLib::GetMousePoint(&mouseX, &mouseY);
            if (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                score++;
                change = GetRand(5);
            }
            else if (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482)
            {
                hp--;
                change = GetRand(5);
            }
        }
		break;
    default:
        break;
    }
}

void Game_02::Draw(int hp, int score) const
{
	//DrawGraph(0, 0, background, TRUE);

	mihon_3->Draw({ 0, 0 });
	DrawGraph(157, 320, a[change], TRUE);

    // UI•\Ž¦
    DxLib::DrawFormatString(10, 10, DxLib::GetColor(255, 255, 255), L"Score: %d", score);
    DxLib::DrawFormatString(10, 30, DxLib::GetColor(255, 255, 255), L"HP: %d", hp);
}