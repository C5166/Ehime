// GameObject.h
#pragma once
#include "DxPlus/DxPlus.h"
#include <string>

enum class BallType
{
    Red, Blue, Green, Target, Penalty
};

enum class ObjectType
{
    Nekosima, // ターゲット
    Neko,     // ターゲット
    Hitu,     // ターゲット
    Inu,      // ターゲット
    Kesi1,    // ダミー
    Kesi2,    // ダミー
    Kesi3     // ダミー
};

struct Object
{
    std::string name{ "Object" }; // ヒエラルキー用表示名
    DxPlus::Vec2 position{ 0.0f, 0.0f };
    DxPlus::Vec2 size{ 100.0f, 100.0f };
    float rotation{ 0.0f }; // ラジアン表記
    DxPlus::Vec2 velocity{ 0.0f, 0.0f };
    DxPlus::Vec2 acceleration{ 0.0f, 0.0f };
    bool isColliding{ false };
    bool active{ true };

    BallType type{ BallType::Target };
    ObjectType objType{ ObjectType::Neko };
};