#include "DebugInspector.h"
#include "SceneManager.h"
#include "GameContext.h"
#include "DxPlus/DxPlus.h"
#include <DxLib.h>
#include "imgui.h"

void DebugInspector::Update()
{
    // キー入力での切替（従来通り残すことも可能）
    if (DxLib::CheckHitKey(KEY_INPUT_1))
    {
        SceneManager::GetInstance().SetScene(SceneManager::GetInstance().GetScene(SceneID::Title));
    }
    if (DxLib::CheckHitKey(KEY_INPUT_2))
    {
        SceneManager::GetInstance().SetScene(SceneManager::GetInstance().GetScene(SceneID::Game));
    }
    if (DxLib::CheckHitKey(KEY_INPUT_3))
    {
        SceneManager::GetInstance().SetScene(SceneManager::GetInstance().GetScene(SceneID::Result));
    }
}

void DebugInspector::Draw()
{
    // ImGuiウィンドウの描画処理
    ImGui::Begin("Debug Inspector");

    ImGui::Text("Scene Select");
    if (ImGui::Button("Title [1]"))
    {
        SceneManager::GetInstance().SetScene(SceneManager::GetInstance().GetScene(SceneID::Title));
    }
    ImGui::SameLine();
    if (ImGui::Button("Game [2]"))
    {
        SceneManager::GetInstance().SetScene(SceneManager::GetInstance().GetScene(SceneID::Game));
    }
    ImGui::SameLine();
    if (ImGui::Button("Result [3]"))
    {
        SceneManager::GetInstance().SetScene(SceneManager::GetInstance().GetScene(SceneID::Result));
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
}