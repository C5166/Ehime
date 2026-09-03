
// GameTimerはゲームのタイマーで、経過時間やフレーム間の時間を計測するためのクラスです。

#pragma once
#include "DxPlus/DxPlus.h"

class GameTimer
{
	public:
	GameTimer() = default;
	~GameTimer() = default;
	void Start();
	void Stop();
	void Reset();
	void Update();
	float GetElapsedTime() const; // 経過時間を秒単位で取得
	float GetDeltaTime() const;   // 前回のフレームからの経過時間を秒単位で取得
private:
	bool isRunning{ false };
	float startTime{ 0.0f };
	float elapsedTime{ 0.0f };
	float lastUpdateTime{ 0.0f };
};