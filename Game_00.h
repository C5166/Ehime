// Game_00.h
#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>
#include <string>

enum class InstructionsType
{
    nekosimaPick,    // game_setumei_9の説明画像   指示、game_3_nekosimaを撮れ
    nekoPick,   // game_setumei_10の説明画像   指示、game_3_nekoを撮れ
    hituPick,   // game_setumei_11の説明画像  指示、game_3_hituを撮れ
    inuPick,   // game_setumei_12の説明画像  指示、game_3_inuを撮れ
    game_3_kesi1Pick,   // game_setumei_13の説明画像  指示、game_3_kesi1を撮れ
    game_3_kesi2Pick,   // game_setumei_14の説明画像  指示、game_3_kesi2を撮れ
    game_3_kesi3Pick    // game_setumei_15の説明画像  指示、game_3_kesi3を撮れ
};

class Game_00
{
public:
    Game_00() = default;

    void Init();
    void Reset();
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;
    const DxPlus::Sprite::SpriteBase* GetMoziSprite() const { return currentMoziSpr; }
    // ImGui 用 UI・ギズモの描画関数
    void DrawImGui();

    // ファイル名を指定して保存・読み込み
    void SaveToFile(const std::string& filename = "scene_game00.json");
    void LoadFromFile(const std::string& filename = "scene_game00.json");

    void SetTargetObjectType(ObjectType type) { targetObjectType = type; }
    ObjectType GetTargetObjectType() const { return targetObjectType; }

    // 現在選択されている指示画像を取得
    const DxPlus::Sprite::SpriteBase* GetExplanationSprite() const { return currentExplanationSpr; }

    // 目標がすべて選択されたか
    bool AllTargetsCollected() const;

    const DxPlus::Sprite::SpriteBase* GetTargetSprite() const {
        return GetSpriteForType(targetObjectType);
    }

private:
    std::vector<Object> subjects;
    int selectedObjectIndex{ -1 }; // 選択中のオブジェクトのインデックス

    ObjectType targetObjectType{ ObjectType::Nekosima }; // デフォルトターゲット
    const DxPlus::Sprite::SpriteBase* currentExplanationSpr{ nullptr }; // 現在の指示画像

    // ImGuiでの保存・読み込み用ファイル名バッファ
    char saveFileNameBuf[128]{ "game_setumei_9.json" };

    // カメラ情報
    DxPlus::Vec2 cameraPos{ 960.0f, 540.0f };
    DxPlus::Vec2 cameraSize{ 240.0f, 180.0f };
    float cameraSpeed{ 10.0f };

    const DxPlus::Sprite::SpriteBase* currentMoziSpr{ nullptr };

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

    int setumeivoice[4];

    // 現在選択されているパターンのインデックス
    int currentTargetIndex{ 0 };
};