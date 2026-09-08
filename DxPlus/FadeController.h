// FadeController.h
#pragma once

namespace DxPlus
{
    class FadeController
    {
    public:
        enum class State { FadeIn, Stay, FadeOut };

        FadeController();

        // フェードイン・アウトを開始する（duration = 0.0f 以下で自動計算）
        void StartFadeIn(float duration = -1.0f);
        void StartFadeOut(float duration = -1.0f);
        void Update();      // 更新処理
        void Draw() const;  // 描画処理

        // スピード（再生フレームレート）の調整設定
        void SetFrameRate(float fps) { frameRate = fps; }
        float GetFrameRate() const { return frameRate; }

        // 状態判定
        State GetState() const;
        bool IsFadeOutDone() const;
        bool IsStable() const;

    private:
        float timer = 0.0f;
        float duration = 2.0f;
        float frameRate = 30.0f; // 1秒間に何コマ進めるか（FPS）
        State state;
    };
} 