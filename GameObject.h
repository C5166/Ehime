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
    // --- 識別・親子関係管理用 ---
    int id{ -1 };                 // 一意のオブジェクトID
    int parentId{ -1 };           // 親のID（-1 は親なし）
    std::vector<int> childIds;    // 子オブジェクトのIDリスト

    // --- 既存のプロパティ ---
    std::string name{ "Object" };

    // 親がいる場合、position / rotation は「親からの相対値（ローカル値）」として扱います
    DxPlus::Vec2 position{ 0.0f, 0.0f };
    DxPlus::Vec2 size{ 100.0f, 100.0f };
    float rotation{ 0.0f };

    DxPlus::Vec2 velocity{ 0.0f, 0.0f };
    DxPlus::Vec2 acceleration{ 0.0f, 0.0f };

    bool isColliding{ false };
    bool active{ true };

    BallType type{ BallType::Target };
    ObjectType objType{ ObjectType::Neko };

    // ---------------------------------------------------------------------
    // 親の移動・回転を反映した「ワールド座標」を取得する関数
    // ---------------------------------------------------------------------
    DxPlus::Vec2 GetWorldPosition(const std::vector<Object>& allObjs) const
    {
        // 親がいなければそのままの座標（ワールド座標）を返す
        if (parentId == -1) return position;

        // 親を探して計算
        for (const auto& parent : allObjs)
        {
            if (parent.id == parentId)
            {
                // 親のワールド座標を取得（再帰）
                DxPlus::Vec2 parentWorldPos = parent.GetWorldPosition(allObjs);
                float parentWorldRot = parent.GetWorldRotation(allObjs);

                // 親の回転角度を考慮してローカル座標を回転させる
                float cosR = std::cos(parentWorldRot);
                float sinR = std::sin(parentWorldRot);
                float rx = position.x * cosR - position.y * sinR;
                float ry = position.x * sinR + position.y * cosR;

                return { parentWorldPos.x + rx, parentWorldPos.y + ry };
            }
        }
        return position;
    }

    // ---------------------------------------------------------------------
    // 親の回転を反映した「ワールド回転角（ラジアン）」を取得する関数
    // ---------------------------------------------------------------------
    float GetWorldRotation(const std::vector<Object>& allObjs) const
    {
        if (parentId == -1) return rotation;

        for (const auto& parent : allObjs)
        {
            if (parent.id == parentId)
            {
                return parent.GetWorldRotation(allObjs) + rotation;
            }
        }
        return rotation;
    }
};

// --- JSON 相互変換用定義 ---
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