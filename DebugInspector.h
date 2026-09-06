#pragma once

class DebugInspector
{
public:
    static DebugInspector& GetInstance()
    {
        static DebugInspector instance;
        return instance;
    }

    void Update(); // キー入力を監視
    void Draw();   // 画面上に選択状態を描画

private:
    DebugInspector() = default;
    ~DebugInspector() = default;

    int selectedScene = 0;   // 0:Title, 1:Game, 2:Result
    int selectedGame = 0;    // 0:Game00, 1:Game02, 2:Game03
    bool isGameMode = false; // シーン選択かゲーム選択か
};