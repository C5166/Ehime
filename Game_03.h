#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>

// ルールの種類（6つの指示に対応）
enum class RuleType
{
    PickGreen,      // 緑を全て拾ってね (game_setumei_1)
    PickRed,        // 赤を全て拾ってね (game_setumei_2)
    PickBlue,       // 青を全て拾ってね (game_setumei_3)
    AvoidBlue,      // 青を拾わないでね (game_setumei_4)
    AvoidRed,       // 赤を拾わないでね (game_setumei_5)
    AvoidGreen      // 緑を拾わないでね (game_setumei_6)
};

class Game_03
{
public:
    Game_03() = default;
    void Init();
    void Reset();
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;

    // 現在の指示画像を取得する関数
    const DxPlus::Sprite::SpriteBase* GetExplanationSprite() const { return currentExplanationSpr; }

    // 指定のターゲットがすべて選択されたか
    bool AllTargetsCollected() const;

private:
    void SpawnBalls();

    const DxPlus::Sprite::SpriteBase* Herat1ID{ nullptr }; // 青
    const DxPlus::Sprite::SpriteBase* Herat2ID{ nullptr }; // 赤
    const DxPlus::Sprite::SpriteBase* Herat3ID{ nullptr }; // 緑

    // 6つの説明画像用ポインタ
    const DxPlus::Sprite::SpriteBase* explanationSprites[6]{ nullptr };
    const DxPlus::Sprite::SpriteBase* currentExplanationSpr{ nullptr };

    RuleType currentRule{ RuleType::PickRed };

    std::vector<Object> balls;

    const float screenX = 50.0f;
    const float screenY = 100.0f;
    const float screenWidth = 1852.0f;
    const float screenHeight = 860.0f;

    int poti;
    int bubu;
    int good[3];
    int perfect;

    int setumeivoice[2];

};