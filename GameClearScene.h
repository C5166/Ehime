#pragma once
#include "Scene.h"
#include "GameContext.h"

class GameClearScene final : public Scene
{
public:
    explicit GameClearScene(GameContext* context) : Scene(context) {}
    ~GameClearScene() override = default;

    void Init() override;
    void Update() override;
    void Render() const override;

private:
    void UpdateGameClearLogoAnimation(); // アニメーション更新関数
    void DrawGameClearUI() const;        // ロゴ描画関数

private:
    bool isGameClearInput{ false };
    int isGameClearInputCount{ 0 };
    const int isGameClearInputMax{ 10 };

    // --- ロゴアニメーション・描画設定 ---
    DxPlus::Vec2 logoPos{ 960.0f, 740.0f };    // 表示位置
    DxPlus::Vec2 logoScale{ 1.5f, 1.5f };      // 拡大率
    int animFrameInterval{ 6 };                // フレーム間隔（大きいほど遅くなる）
    const int totalLogoFrames{ 42 };           // 総コマ数（21コマ）
    const int logoColumns{ 10 };               // スプライトシートの横コマ数（10列）

    // アニメーション制御用変数
    int logoAnimTimer{ 0 };
    int currentLogoIndex{ 0 };

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameclear_background{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameclear_character_1{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameclear_character_2{ nullptr };

    int voice;

    int  touch[5];
    int poti;
    int kirakira;
    int a = 0;

	GameContext gameContext;
};