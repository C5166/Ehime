// =============================
// Scenes/Title/TitleScene.h
// =============================
#pragma once
#include "Scene.h"
#include "GameContext.h"

class TitleScene final : public Scene
{
public:
    explicit TitleScene(GameContext* context) : Scene(context) {}
    void Init() override;
    void Update() override;
    void Render() const override;
    void Draw() const;

private:
//10‰ñƒJƒEƒ“ƒg
	int TenCount{ 1 };
    int frameCount{ 0 };
    int fontHandle{ -1 };

	bool isTitleInput{ false };
	int isTitleInputCount{ 0 };
	int isTitleInputMax{ 10 };

    const DxPlus::Sprite::SpriteBase* bgSprite{ nullptr };
    const DxPlus::Sprite::SpriteBase* title_character{ nullptr };
    const DxPlus::Sprite::SpriteBase* title_character2{ nullptr };

    int  touch[5];
    int poti;
    int kirakira;
    int a=0;
    int startvoice;
};
