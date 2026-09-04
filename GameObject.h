#pragma once
#include "DxPlus/DxPlus.h"

// 的の種類
enum class BallType
{
    Red,  // ターゲット（加点）
    Blue  // お手付き（HP減）
};

struct Object
{
    DxPlus::Vec2 position;
    DxPlus::Vec2 size;       // 半径代わりに size.x や radius を使用
    float rotation{ 0.0f };
    DxPlus::Vec2 velocity;    // 移動速度
    DxPlus::Vec2 acceleration{ 0, 0 };
    bool active{ true };      // クリックされて消えたかどうか
    BallType type{ BallType::Red };
};