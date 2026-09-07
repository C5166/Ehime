// =============================
// Scenes/Result/ResultScene.h
// =============================
#pragma once
#include "Scene.h"

class ResultScene final : public Scene
{
public:
    explicit ResultScene(class GameContext* context) : Scene(context) {}
    void Init() override;
    void Update() override;
    void Render() const override;

private:
    int fontHandle{ -1 };
    const DxPlus::Sprite::SpriteBase* Gameover_background{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_character_1{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_character_2{ nullptr };
    
};
