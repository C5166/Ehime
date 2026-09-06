#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>

class Game_02
{
public:
    Game_02() = default;
    void Init();
    void Reset();
    void DrawPerfectAnimation(int x, int y) ;
    void Update();
    void Draw() const;

private:

    int background; 

    int a[6];

    // 画面の幅・高さ（必要に応じてゲームの画面サイズに合わせて変更してください）
    const float screenWidth = 1920.0f;
    const float screenHeight = 1080.0f;

	const DxPlus::Sprite::SpriteBase* mihon_3{ nullptr };

    int change = 0;
    int number{ 0 };  // スコア
    int HP{ 3 };      // 仮HP（ハート3個分）

    //一旦
    int waku;
    int perfectSheetID;
	int notsheetID;

	bool Animating = false; 

    struct notFrameRect {
        int x, y, w, h;
    };

    int frame = 0;
};
