// Game_00.h
#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>
#include <string>

class Game_00
{
public:
    Game_00() = default;

    void Init();
    void Reset();
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;

    // ImGui 用 UI・ギズモの描画関数
    void DrawImGui();

    // ファイル名を指定して保存・読み込み
    void SaveToFile(const std::string& filename = "scene_game00.json");
    void LoadFromFile(const std::string& filename = "scene_game00.json");

    void SetTargetObjectType(ObjectType type) { targetObjectType = type; }
    ObjectType GetTargetObjectType() const { return targetObjectType; }

private:
    std::vector<Object> subjects;
    int selectedObjectIndex{ -1 }; // 選択中のオブジェクトのインデックス

    ObjectType targetObjectType{ ObjectType::Nekosima }; // デフォルトターゲット

    // ImGuiでの保存・読み込み用ファイル名バッファ
    char saveFileNameBuf[128]{ "game_setumei_9.json" };

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

    bool isCameraLocked{ false };

    int CAMERA;
    int bubu;
    int good[3];
    int perfect;

        int nextUniqueId{ 1 };          // ID自動インクリメント用
    int copiedObjectId{ -1 };        // コピー中のオブジェクトID

    // ID検索ヘルパー
    Object* FindObjectById(int id);
    const Object* FindObjectById(int id) const;

    // 階層関係の追加・解除
    void SetParent(int childId, int newParentId);

    // ツリー再生描画用関数
    void DrawHierarchyTree(int objId);

    // ディープコピー処理（子要素もまとめて複製）
    int DuplicateObjectRecursive(int srcId, int newParentId = -1);

    int setumeivoice;

};