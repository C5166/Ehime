// GameObject.h
#pragma once
#include "DxPlus/DxPlus.h"
#include <string>
#include "nlohmann/json.hpp"

enum class BallType
{
    Red, Blue, Green, Target, Penalty
};

enum class ObjectType
{
    Nekosima,
    Neko,
    Hitu,
    Inu,
    Kesi1,
    Kesi2,
    Kesi3
};

struct Object
{
    std::string name{ "Object" };
    DxPlus::Vec2 position{ 0.0f, 0.0f };
    DxPlus::Vec2 size{ 100.0f, 100.0f };
    float rotation{ 0.0f };
    DxPlus::Vec2 velocity{ 0.0f, 0.0f };
    DxPlus::Vec2 acceleration{ 0.0f, 0.0f };
    bool isColliding{ false };
    bool active{ true };

    BallType type{ BallType::Target };
    ObjectType objType{ ObjectType::Neko };
};

// --- JSON ‘ŠŒÝ•ÏŠ·—p’è‹` ---
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Object,
    name,
    position.x, position.y,
    size.x, size.y,
    rotation,
    active,
    type,
    objType
)