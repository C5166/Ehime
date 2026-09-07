// DebugInspector.cpp
#include "DebugInspector.h"
#include "SceneManager.h"
#include "GameContext.h"
#include "DxPlus/DxPlus.h"
#include <DxLib.h>
#include "imgui.h"

namespace
{
    // TitleScene と同様のフローでシーン遷移を行う関数
    void TransitionToScene(SceneID targetID)
    {
        Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
        Scene* nextScene = SceneManager::GetInstance().GetScene(targetID);

        // 現在のシーンが存在し、遷移先が現在と異なる場合のみ実行
        if (currentScene && nextScene && currentScene != nextScene)
        {
            // TitleScene と同じやり方： SetNextScene して StartFadeOut
            currentScene->SetNextScene(nextScene);
            currentScene->StartFadeOut();
        }
    }
}

void DebugInspector::Update()
{
    // キー入力での切り替え（TitleSceneと同じフェード遷移を使う）
    if (DxLib::CheckHitKey(KEY_INPUT_1))
    {
        TransitionToScene(SceneID::Title);
    }
    if (DxLib::CheckHitKey(KEY_INPUT_2))
    {
        TransitionToScene(SceneID::Game);
    }
    if (DxLib::CheckHitKey(KEY_INPUT_3))
    {
    }
}

void DebugInspector::Draw()
{
    ImGui::Begin("Debug Inspector");

    ImGui::Text("Scene Select");

    // ImGui ボタンから TitleScene と同じ流れで切り替え
    if (ImGui::Button("Title [1]"))
    {
        TransitionToScene(SceneID::Title);
    }
    ImGui::SameLine();
    if (ImGui::Button("Game [2]"))
    {
        TransitionToScene(SceneID::Game);
    }
    ImGui::SameLine();
    if (ImGui::Button("Result [3]"))
    {
    }

    ImGui::Separator();

    ImGui::Text("MiniGame Select");
    GameContext& gc = SceneManager::GetInstance().GetGameState();
    if (ImGui::Button("Game 00 [F1]")) gc.SetCurrentMiniGame(GameNamber::Game_0);
    ImGui::SameLine();
    if (ImGui::Button("Game 02 [F2]")) gc.SetCurrentMiniGame(GameNamber::Game_2);
    ImGui::SameLine();
    if (ImGui::Button("Game 03 [F3]")) gc.SetCurrentMiniGame(GameNamber::Game_3);

    ImGui::End();

    auto& gameContext = SM().GetGameState();
    if (gameContext.GetCurrentMiniGame() == GameNamber::Game_0)
    {
        gameContext.GetGame00().DrawImGui();
    }
}