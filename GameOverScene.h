#pragma once
#include "Scene.h"

class GameOverScene final : public Scene
{
public:
    explicit GameOverScene(GameContext* context) : Scene(context) {}
    ~GameOverScene() override = default;

    void Init() override;
    void Update() override;
    void Render() const override;

private:
    void DrawGameOverUI() const;

private:
    bool isGameOverInput{ false };
    int isGameOverInputCount{ 0 };
    const int isGameOverInputMax{ 10 };

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_background{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_character_1{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_character_2{ nullptr };

    int bgm;

    int  touch[5];
    int a;
    int poti;
    int kirakira;

    int voice;

};