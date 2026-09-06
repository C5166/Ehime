#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>

class Game_00
{
public:
    Game_00() = default;

    void Init();
    void Reset();
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;

private:
    std::vector<Object> subjects;

    // カメラ（ファインダー）情報
    DxPlus::Vec2 cameraPos{ 640.0f, 360.0f };
    DxPlus::Vec2 cameraSize{ 160.0f, 120.0f }; // ファインダー枠のサイズ
    float cameraSpeed{ 8.0f };

    // 撮るモーション（演出用）
    float shutterAnimTimer{ 0.0f };
    int flashAlpha{ 0 };

    const float screenWidth =  DxPlus::CLIENT_WIDTH ;
    const float screenHeight = DxPlus::CLIENT_HEIGHT;

};