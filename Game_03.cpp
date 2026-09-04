#include "Game_03.h"

void Game_03::Init()
{
	RedBall.position = { 100, 100 };
	RedBall.size = { 50, 50 };
	RedBall.rotation = 0;
	RedBall.velocity = { 0, 0 };
	RedBall.acceleration = { 0, 0 };
	RedBall.isColliding = false;

	BlueBall.position = { 300, 300 };
	BlueBall.size = { 50, 50 };
	BlueBall.rotation = 0;
	BlueBall.velocity = { 0, 0 };
	BlueBall.acceleration = { 0, 0 };
	BlueBall.isColliding = false;

	number = 0;
}

void Game_03::Reset()
{
	RedBall.position = { 100, 100 };
	RedBall.velocity = { 0, 0 };
	RedBall.acceleration = { 0, 0 };
	RedBall.isColliding = false;

	BlueBall.position = { 300, 300 };
	BlueBall.velocity = { 0, 0 };
	BlueBall.acceleration = { 0, 0 };
	BlueBall.isColliding = false;

	spawnCount = 0;

	number = 0;
}

void Game_03::Update()
{
	


	//赤いブロックを左クリックしたらRedBall.isColliding = tureにして消す、そしたらnumberを1増やす
	using namespace DxPlus::Input;
	int botton = GetButtonDown(PLAYER1);
	if (botton & BUTTON_TRIGGER2)
	{
		int mouseX, mouseY;
		DxLib::GetMousePoint(&mouseX, &mouseY);
		if (mouseX >= RedBall.position.x - RedBall.size.x * 0.5f &&
			mouseX <= RedBall.position.x + RedBall.size.x * 0.5f &&
			mouseY >= RedBall.position.y - RedBall.size.y * 0.5f &&
			mouseY <= RedBall.position.y + RedBall.size.y * 0.5f)
		{
			RedBall.isColliding = true;
			number++;
		}
	}

	if (botton & BUTTON_TRIGGER2)
	{
		int mouseX, mouseY;
		DxLib::GetMousePoint(&mouseX, &mouseY);
		if (mouseX >= BlueBall.position.x - BlueBall.size.x * 0.5f &&
			mouseX <= BlueBall.position.x + BlueBall.size.x * 0.5f &&
			mouseY >= BlueBall.position.y - BlueBall.size.y * 0.5f &&
			mouseY <= BlueBall.position.y + BlueBall.size.y * 0.5f)
		{
			BlueBall.isColliding = true;
			HP--;
		}
	}
}

void Game_03::Draw() const
{
	// 
	if (RedBall.isColliding == false) {
		DxLib::DrawBox(static_cast<int>(RedBall.position.x - RedBall.size.x * 0.5f),
			static_cast<int>(RedBall.position.y - RedBall.size.y * 0.5f),
			static_cast<int>(RedBall.position.x + RedBall.size.x * 0.5f),
			static_cast<int>(RedBall.position.y + RedBall.size.y * 0.5f),
			DxLib::GetColor(255, 0, 0), TRUE);
	}

	// Draw BlueBall
	if (BlueBall.isColliding == false) {
		DxLib::DrawBox(static_cast<int>(BlueBall.position.x - BlueBall.size.x * 0.5f),
			static_cast<int>(BlueBall.position.y - BlueBall.size.y * 0.5f),
			static_cast<int>(BlueBall.position.x + BlueBall.size.x * 0.5f),
			static_cast<int>(BlueBall.position.y + BlueBall.size.y * 0.5f),
			DxLib::GetColor(0, 0, 255), TRUE);
	}

	//スコアを表示
	DxLib::DrawFormatString(10, 10, DxLib::GetColor(255, 255, 255), L"Score: %d", number);

	//仮㏋
	DxLib::DrawFormatString(10, 30, DxLib::GetColor(255, 255, 255), L"HP: %d",HP );
}
