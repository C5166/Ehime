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
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;
    void DrawPerfectAnimation(int x, int y) ;

private:

    int background; 

    int a[6];

    // 画面の幅・高さ（必要に応じてゲームの画面サイズに合わせて変更してください）
    const float screenWidth = 1920.0f;
    const float screenHeight = 1080.0f;

	const DxPlus::Sprite::SpriteBase* mihon_3{ nullptr };

    int change = 0;

    //一旦
    int waku;
    int perfectSheetID;
	int notsheetID;

    int poti;

	bool Animating = false; 

    struct notFrameRect {
        int x, y, w, h;
    };

    int frame = 0;

    int bubu;

    int good[3];

    int perfect;

    int setumeivoice;

};