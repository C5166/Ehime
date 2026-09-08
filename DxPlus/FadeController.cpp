// FadeController.cpp
#include "FadeController.h"
#include "DxPlus.h"
#include "../ResourceManager.h"
#include "../ResourceKeys.h"
#include <algorithm>

namespace DxPlus
{
    FadeController::FadeController() : timer(0.0f), duration(1.0f), state(State::Stay) {}

    void FadeController::StartFadeIn(float duration_)
    {
        duration = duration_;
        if (duration <= 0.0f)
        {
            timer = 0.0f;
            state = State::Stay;
            return;
        }
        timer = duration; // duration から 0.0f へカウントダウン
        state = State::FadeIn;
    }

    void FadeController::StartFadeOut(float duration_)
    {
        duration = duration_;
        if (duration <= 0.0f)
        {
            timer = 0.0f;
            state = State::FadeOut;
            return;
        }
        timer = 0.0f; // 0.0f から duration へカウントアップ
        state = State::FadeOut;
    }

    void FadeController::Update()
    {
        if (duration <= 0.0f)
        {
            return;
        }

        float speed = 1.0f / (duration * 60.0f);
        if (state == State::FadeIn)
        {
            timer -= speed;
            if (timer <= 0.0f)
            {
                timer = 0.0f;
                state = State::Stay;
            }
        }
        else if (state == State::FadeOut)
        {
            timer += speed;
            if (timer >= duration)
            {
                timer = duration;
            }
        }
    }

    void FadeController::Draw() const
    {
        if (state == State::Stay || duration <= 0.0f) { return; }

        int frameIndex = 0;

        if (state == State::FadeIn)
        {
            // 前半15コマ (14 -> 0)
            float progress = std::clamp(timer / duration, 0.0f, 1.0f);
            int totalFrames = 15;
            int offset = static_cast<int>(progress * (totalFrames - 1));
            frameIndex = offset; // 14から0に向かって再生
        }
        else if (state == State::FadeOut)
        {
            // 後半15コマ (15 -> 29)
            float progress = std::clamp(timer / duration, 0.0f, 1.0f);
            int totalFrames = 15;
            int offset = static_cast<int>(progress * (totalFrames - 1));
            frameIndex = 15 + offset; // 15から29に向かって再生
        }

        int gridX = frameIndex % 10;
        int gridY = frameIndex / 10;

        const auto* sprite = RM().GridAt(ResourceKeys::transition, gridX, gridY);
        if (sprite)
        {
            int handle = sprite->GetID();
            if (handle != -1)
            {
                DxLib::DrawExtendGraph(
                    0, 0,
                    static_cast<int>(DxPlus::CLIENT_WIDTH),
                    static_cast<int>(DxPlus::CLIENT_HEIGHT),
                    handle, TRUE
                );
            }
        }
    }

    FadeController::State DxPlus::FadeController::GetState() const
    {
        return state;
    }

    bool DxPlus::FadeController::IsFadeOutDone() const
    {
        return state == State::FadeOut && timer >= duration;
    }

    bool FadeController::IsStable() const
    {
        return state == State::Stay;
    }
} // namespace DxPlus