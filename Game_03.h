#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"

class Game_03
{
public:
	Game_03() = default;
	void Init();
	void Reset();
	void Update();
	void Draw() const;
private:

	Object RedBall;
	Object BlueBall;

	//数字
	int number { 0 };

	//仮㏋
	int HP{ 100 };

};