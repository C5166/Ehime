#include "Game_01.h"
#include <cstdlib>
#include <cmath>

void Game_01::Init()
{
    Reset();
}

void Game_01::Reset()
{
    number = 0;
    HP = 3;
    cameraPos = { screenWidth * 0.5f, screenHeight * 0.5f };
    shutterAnimTimer = 0.0f;
    flashAlpha = 0;

    subjects.clear();

    // 被写体の生成（例：ターゲット10体、お手付き10体）
    int totalTarget = 10;
    int totalPenalty = 10;
    for (int i = 0; i < totalTarget + totalPenalty; ++i)
    {
        Object subj;
		subj.type = (i < totalTarget) ? BallType::Target : BallType::Penalty;
        subj.size = { 40.0f, 40.0f };
        subj.active = true;

        // 地面付近から画面中央あたりのランダムな位置
        subj.position.x = static_cast<float>(rand() % static_cast<int>(screenWidth - 100) + 50);
        subj.position.y = static_cast<float>(rand() % 200 + 450); // 地面ライン

        // 左右移動の速度設定
        float vx = static_cast<float>((rand() % 100 - 50) / 10.0f);
        if (std::abs(vx) < 0.5f) vx = 1.5f;
        subj.velocity = { vx, 0.0f };

        subjects.push_back(subj);
    }
}

void Game_01::Update()
{
    using namespace DxPlus::Input;

    // ----------------------------------------------------
    // 1. カメラ移動（WASD ＆ マウス操作の両方に対応）
    // ----------------------------------------------------
    // WASDキー入力
    if (DxLib::CheckHitKey(KEY_INPUT_W)) cameraPos.y -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_S)) cameraPos.y += cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_A)) cameraPos.x -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_D)) cameraPos.x += cameraSpeed;

    // マウス移動（マウスが動いた場合にカメラ位置を追従）
    int mouseX = 0, mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

    // マウス座標の移動量や直接追従（ここではマウス位置に滑らかに追従させる例）
    static int prevMouseX = mouseX, prevMouseY = mouseY;
    if (mouseX != prevMouseX || mouseY != prevMouseY)
    {
        cameraPos.x = static_cast<float>(mouseX);
        cameraPos.y = static_cast<float>(mouseY);
        prevMouseX = mouseX;
        prevMouseY = mouseY;
    }

    // 画面外はみ出し制限
    if (cameraPos.x < cameraSize.x * 0.5f) cameraPos.x = cameraSize.x * 0.5f;
    if (cameraPos.x > screenWidth - cameraSize.x * 0.5f) cameraPos.x = screenWidth - cameraSize.x * 0.5f;
    if (cameraPos.y < cameraSize.y * 0.5f) cameraPos.y = cameraSize.y * 0.5f;
    if (cameraPos.y > screenHeight - cameraSize.y * 0.5f) cameraPos.y = screenHeight - cameraSize.y * 0.5f;

    // ----------------------------------------------------
    // 2. 被写体の移動処理（地面上の歩行と画面端の反射）
    // ----------------------------------------------------
    for (auto& subj : subjects)
    {
        if (!subj.active) continue;

        subj.position.x += subj.velocity.x;
        if (subj.position.x - subj.size.x * 0.5f < 0 || subj.position.x + subj.size.x * 0.5f > screenWidth)
        {
            subj.velocity.x *= -1.0f;
        }
    }

    // ----------------------------------------------------
    // 3. 撮影入力判定（Enterキー 判定 ＆ 左クリック 判定）
    // ----------------------------------------------------
    bool isEnterPressed = (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1);

    // パッド・マウス入力取得（TRIGGER2＝左クリック等）
    int button = GetButtonDown(PLAYER1);
    bool isMousePressed = (button & BUTTON_TRIGGER2) != 0;

    static bool prevEnterState = false;
    bool isShutterTriggered = (isEnterPressed && !prevEnterState) || isMousePressed;
    prevEnterState = isEnterPressed;

    if (isShutterTriggered)
    {
        // 撮影モーションとフラッシュの開始設定
        shutterAnimTimer = 1.0f;
        flashAlpha = 180;        // 画面を一瞬白く光らせる

        // ファインダー（カメラ枠）の範囲計算
        float camMinX = cameraPos.x - cameraSize.x * 0.5f;
        float camMaxX = cameraPos.x + cameraSize.x * 0.5f;
        float camMinY = cameraPos.y - cameraSize.y * 0.5f;
        float camMaxY = cameraPos.y + cameraSize.y * 0.5f;

        // 枠内に入っている被写体を判定
        for (auto& subj : subjects)
        {
            if (!subj.active) continue;

            // 被写体の中心がカメラ枠内に収まっているか判定
            if (subj.position.x >= camMinX && subj.position.x <= camMaxX &&
                subj.position.y >= camMinY && subj.position.y <= camMaxY)
            {
                subj.active = false; // 撮影成功で消去

                if (subj.type == BallType::Target)
                {
                    number++; // スコア加点
                }
                else if (subj.type == BallType::Penalty)
                {
                    HP--;     // ペナルティ
                }
            }
        }
    }

    // 撮影モーションタイマーの減衰
    if (shutterAnimTimer > 0.0f) shutterAnimTimer -= 0.1f;
    if (shutterAnimTimer < 0.0f) shutterAnimTimer = 0.0f;

    if (flashAlpha > 0) flashAlpha -= 15;
    if (flashAlpha < 0) flashAlpha = 0;
}

void Game_01::Draw() const
{
    // 1. 背景描画（空・遠景）
    DxLib::DrawBox(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(200, 220, 240), TRUE);

    // 地面描画
    DxLib::DrawBox(0, 500, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(160, 160, 160), TRUE);

    // 2. 被写体描画
    for (const auto& subj : subjects)
    {
        if (!subj.active) continue;

        unsigned int color = (subj.type == BallType::Target)
            ? DxLib::GetColor(210, 140, 40)   // 茶色（ターゲット）
            : DxLib::GetColor(40, 120, 210);  // 青色（お手付き）

        DxLib::DrawBox(
            static_cast<int>(subj.position.x - subj.size.x * 0.5f),
            static_cast<int>(subj.position.y - subj.size.y * 0.5f),
            static_cast<int>(subj.position.x + subj.size.x * 0.5f),
            static_cast<int>(subj.position.y + subj.size.y * 0.5f),
            color, TRUE
        );
    }

    // 3. カメラ（ファインダー）描画 ＆ 撮るモーション演出
    // モーション効果：撮影時に少しサイズが拡大して戻る
    float scale = 1.0f + shutterAnimTimer * 0.15f;
    DxPlus::Vec2 currentCamSize = { cameraSize.x * scale, cameraSize.y * scale };

    int cMinX = static_cast<int>(cameraPos.x - currentCamSize.x * 0.5f);
    int cMinY = static_cast<int>(cameraPos.y - currentCamSize.y * 0.5f);
    int cMaxX = static_cast<int>(cameraPos.x + currentCamSize.x * 0.5f);
    int cMaxY = static_cast<int>(cameraPos.y + currentCamSize.y * 0.5f);

    // カメラ外枠（黒色のファインダー枠）
    DxLib::DrawBox(cMinX, cMinY, cMaxX, cMaxY, DxLib::GetColor(30, 30, 30), FALSE);
    DxLib::DrawBox(cMinX - 1, cMinY - 1, cMaxX + 1, cMaxY + 1, DxLib::GetColor(30, 30, 30), FALSE);

    // 中央照準
    DxLib::DrawLine(static_cast<int>(cameraPos.x) - 10, static_cast<int>(cameraPos.y), static_cast<int>(cameraPos.x) + 10, static_cast<int>(cameraPos.y), DxLib::GetColor(255, 0, 0));
    DxLib::DrawLine(static_cast<int>(cameraPos.x), static_cast<int>(cameraPos.y) - 10, static_cast<int>(cameraPos.x), static_cast<int>(cameraPos.y) + 10, DxLib::GetColor(255, 0, 0));

    // 4. フラッシュ演出（撮影時のホワイトアウト）
    if (flashAlpha > 0)
    {
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashAlpha);
        DxLib::DrawBox(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(255, 255, 255), TRUE);
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // 5. UI表示（見つける対象の提示・スコア・HP）
    DxLib::DrawFormatString(10, 10, DxLib::GetColor(0, 0, 0), L"Score: %d", number);
    DxLib::DrawFormatString(10, 30, DxLib::GetColor(200, 0, 0), L"HP: %d", HP);
    DxLib::DrawString(10, 680, L"【操作】WASD / マウス : カメラ移動 | Enter / クリック : 撮影", DxLib::GetColor(0, 0, 0));
}