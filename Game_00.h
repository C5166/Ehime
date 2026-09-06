// Game_00.h
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

    // ImGui 用 UI・ギズモの描画関数（SceneManager や DebugInspector の ImGui 描画部から呼び出す）
    void DrawImGui();

private:
    std::vector<Object> subjects;
    int selectedObjectIndex{ -1 }; // 選択中のオブジェクトのインデックス（-1は未選択）

    // カメラ情報
    DxPlus::Vec2 cameraPos{ 960.0f, 540.0f };
    DxPlus::Vec2 cameraSize{ 240.0f, 180.0f };
    float cameraSpeed{ 10.0f };

    // 演出用
    float shutterAnimTimer{ 0.0f };
    int flashAlpha{ 0 };

    const float screenWidth = DxPlus::CLIENT_WIDTH;
    const float screenHeight = DxPlus::CLIENT_HEIGHT;

    // 画像描画用のヘルパー関数
    const DxPlus::Sprite::SpriteBase* GetSpriteForType(ObjectType type) const;

    // オブジェクト追加用ヘルパー
    void AddObject(ObjectType type, const char* defaultName);

    // 2D ギズモ描画・操作処理
    void DrawGizmo(Object& obj);
};