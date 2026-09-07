// =============================
// Scenes/Base/SceneManager.cpp
// =============================
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DebugInspector.h"
#include "DxPlus/DxPlus.h"

// ImGui用ヘッダー
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

void SceneManager::Init()
{
    constexpr bool WINDOWED = true;
    DxPlus::Initialize(DxPlus::CLIENT_WIDTH, DxPlus::CLIENT_HEIGHT, WINDOWED);

    // --- ImGui 初期化 ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    HWND hWnd = DxLib::GetMainWindowHandle();
    ID3D11Device* pDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
    ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(pDevice, pContext);
    // --------------------

    ResourceManager::GetInstance().LoadAll();
    gameContext.Init();

    titleScene.SetGameContext(&gameContext);
    gameScene.SetGameContext(&gameContext);
    gameOverScene.SetGameContext(&gameContext);
    gameClearScene.SetGameContext(&gameContext);

    scene = &titleScene;
}

void SceneManager::Shutdown()
{
    // --- ImGui 破棄 ---
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    // ------------------

    ResourceManager::GetInstance().UnloadAll();
    DxPlus::Shutdown();
}

void SceneManager::SetScene(Scene* newScene)
{
    if (!newScene || newScene == scene) return;
    scene = newScene;
}

Scene* SceneManager::GetScene(SceneID id)
{
    switch (id)
    {
    case SceneID::Title:    return &titleScene;
    case SceneID::Game:     return &gameScene;
    case SceneID::GameOver: return &gameOverScene;
    case SceneID::GameClear: return &gameClearScene;
    }
    return &titleScene;
}

void SceneManager::Run()
{
    if (scene) scene->Init();
    while (DxPlus::GameLoop())
    {
        DxPlus::Input::Update();

        // --- ImGui 新規フレーム開始 ---
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        // ------------------------------

        DebugInspector::GetInstance().Update();

        if (scene)
        {
            DxLib::ClearDrawScreen();

            scene->Drive();
            scene->Render();

            if (scene->IsFinished())
            {
                scene->End();

                Scene* next = scene->GetNextScene();
                scene->SetNextScene(nullptr);

                if (!next) { DxLib::ScreenFlip(); break; }
                SetScene(next);
                next->Init();
            }
            DxPlus::Debug::Draw();
            DebugInspector::GetInstance().Draw();
            scene->DrawFadeOverlay();

            // --- ImGui 描画 ---
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            // ------------------

            DxLib::ScreenFlip();
        }
    }
}