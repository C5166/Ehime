// =============================
// App/main.cpp
// =============================
#include "DxPlus/DxPlus.h"
#include "SceneManager.h"
#include <crtdbg.h>

// ImGuiのWin32ハンドラ宣言
#include "imgui.h"
#include "imgui_impl_win32.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK CustomWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // ImGuiにイベントを渡す
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    if (msg == WM_KEYDOWN && wParam == VK_ESCAPE)
    {
        PostQuitMessage(0);
    }

    return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
    srand((unsigned int)time(NULL));
    DxLib::SetHookWinProc(CustomWinProc);

    SM().Init();
    SM().Run();
    SM().Shutdown();

    return 0;
}