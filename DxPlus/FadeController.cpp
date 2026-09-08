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
        timer = 0.0f; // 0.0f から duration へカウントアップする方式に統一
        state = State::FadeIn;
    }

    void FadeController::StartFadeOut(float duration_)
    {
        duration = duration_;
        if (duration <= 0.0f)
        {
            timer = 0.0f;
            state = State::Stay;
            return;
        }
        timer = 0.0f; // 0.0f から duration へカウントアップ
        state = State::FadeOut;
    }

    void FadeController::Update()
    {
        if (duration <= 0.0f || state == State::Stay)
        {
            return;
        }

        float speed = 1.0f / (duration * 60.0f);
        timer += speed;

        if (timer >= duration)
        {
            timer = duration;
            if (state == State::FadeIn)
            {
                state = State::Stay; // フェードイン完了で非表示状態へ
            }
        }
    }

    void FadeController::Draw() const
    {
        if (state == State::Stay || duration <= 0.0f) { return; }

        int frameIndex = 0;
        const wchar_t* resourceKey = nullptr;

        // 進行度: 開始(0.0) -> 終了(1.0)
        float progress = std::clamp(timer / duration, 0.0f, 1.0f);

        if (state == State::FadeIn)
        {
            // transition_1: 0コマ目(完全に覆う) -> 15コマ目(画面が開く)
            int offset = static_cast<int>(progress * 15.0f + 0.5f);
            frameIndex = std::clamp(offset, 0, 15);
            resourceKey = ResourceKeys::transition_1;
        }
        else if (state == State::FadeOut)
        {
            // transition_2: 0コマ目(画面が開いている) -> 15コマ目(完全に覆う)
            int offset = static_cast<int>(progress * 15.0f + 0.5f);
            frameIndex = std::clamp(offset, 0, 15);
            resourceKey = ResourceKeys::transition_2;
        }

        if (!resourceKey) { return; }

        int gridX = frameIndex % 10;
        int gridY = frameIndex / 10;

        const auto* sprite = RM().GridAt(resourceKey, gridX, gridY);
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