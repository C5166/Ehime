// =============================
// Core/GameContext.h
// =============================
#pragma once
#include "DxPlus/DxPlus.h"
#include <memory>
#include <vector>
#include "Game_03.h"
#include "Game_02.h"
#include "Game_00.h"

// ゲーム全体で共有する"文脈（コンテキスト）"。
// プレイヤー・敵・マップ・アイテムなどを束ね、
// シーン（Title / Game / Result）から参照できるようにする。
class GameContext
{
public:
    GameContext() = default;
    ~GameContext() = default;

    // 一度だけ呼ぶ初期化（リソースロードは ResourceManager 側で行う）
    void Init();

    // ステージ再開・リトライ用の軽量リセット
    void Reset();

    // プレイの更新と描画（必要なシーンで呼ぶ）
    void Update();
    void Draw() const;

private:
    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };

	Game_03 game_03;
	Game_02 game_02;
    

	bool Isinit{ false };
    Game_00 game_00;
};
