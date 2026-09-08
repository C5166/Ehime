#pragma once
#include "DxPlus/DxPlus.h"

class Scene
{
public:
    explicit Scene(class GameContext* context) : gameContext(context) {}
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update() {}
    virtual void Render() const {}
    virtual void End() {}

    void Drive();

    virtual bool IsFinished() const { return finished; }
    virtual Scene* GetNextScene() { return nextScene; }
    void SetNextScene(Scene* scene);

    void RequestChangeScene(Scene* next, float fadeDuration = 1.0f)
    {
        if (!next) return;
        SetNextScene(next);
        StartFadeOut(fadeDuration);
    }

    void SetGameContext(class GameContext* context) { gameContext = context; }
    void DrawFadeOverlay() const;

    // 外部（DebugInspector等）からフェード開始を呼べるように public に変更、または上記の RequestChangeScene を利用
    void StartFadeIn(float duration = 1.0f);
    void StartFadeOut(float duration = 1.0f);

protected:
    void UpdateFadeController();

protected:
    GameContext* gameContext = nullptr;
    Scene* nextScene = nullptr;
    bool finished = false;

private:
    DxPlus::FadeController fade;
};