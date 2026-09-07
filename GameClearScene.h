#pragma once
#include "Scene.h"

class GameClearScene final : public Scene
{
public:
    explicit GameClearScene(GameContext* context) : Scene(context) {}
    ~GameClearScene() override = default;

    void Init() override;
    void Update() override;
    void Render() const override;

private:
    bool isGameClearInput{ false };
    int isGameClearInputCount{ 0 };
    const int isGameClearInputMax{ 10 };

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
};