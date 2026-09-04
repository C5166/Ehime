#pragma once
#include "DxPlus/DxPlus.h"

// ボールタイプ
enum class BallType
{
    Red,
    Blue,
    Target,  // 撮影対象（加点）
    Penalty  // お手付き（減点）
};

struct Object
{
    DxPlus::Vec2 position;
    DxPlus::Vec2 size;
    float rotation{ 0.0f };
    DxPlus::Vec2 velocity;
    DxPlus::Vec2 acceleration{ 0, 0 };
    bool isColliding{ false };
    bool active{ true };

    // Game_01 で使う型に合わせます
    BallType type{ BallType::Target };
};