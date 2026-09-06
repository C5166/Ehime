#include "Game_01.h"
#include <cstdlib>
#include <cmath>

// ----------------------------------------------------
// 初期化・リセット処理
// ----------------------------------------------------
void Game_01::Init()
{
    Reset();
}

void Game_01::Reset()
{
    // ゲームパラメータの初期化
    number = 0; // スコアリセット
    HP = 3;     // 残機（HP）リセット

    // カメラ・演出パラメーターの初期化
    cameraPos = { screenWidth * 0.5f, screenHeight * 0.5f }; // 画面中央に配置
    shutterAnimTimer = 0.0f;
    flashAlpha = 0;

    // オブジェクトリストをクリアして再生成
    subjects.clear();

    const int totalTarget = 10;  // 撮影成功対象（得点）
    const int totalPenalty = 10; // 撮影NG対象（ペナルティ）

    for (int i = 0; i < totalTarget + totalPenalty; ++i)
    {
        Object subj;
        // 規定数までをターゲット、それ以外をペナルティに設定
        subj.type = (i < totalTarget) ? BallType::Target : BallType::Penalty;
        subj.size = { 40.0f, 40.0f };
        subj.active = true;

        // 【出現位置】画面の左右端を避け、地面（y: 450〜650）付近にランダム配置
        subj.position.x = static_cast<float>(rand() % static_cast<int>(screenWidth - 100) + 50);
        subj.position.y = static_cast<float>(rand() % 200 + 450);

        // 【移動速度】左右の移動速度をランダム設定（停止しないよう最小速度を補正）
        float vx = static_cast<float>((rand() % 100 - 50) / 10.0f);
        if (std::abs(vx) < 0.5f) vx = 1.5f;
        subj.velocity = { vx, 0.0f };

        subjects.push_back(subj);
    }
}

// ----------------------------------------------------
// 更新処理（フレーム毎の動作）
// ----------------------------------------------------
void Game_01::Update()
{
    using namespace DxPlus::Input;

    // ====================================================
    // 1. カメラ操作（WASD ＆ マウス座標追従）
    // ====================================================
    // WASDキーによる移動
    if (DxLib::CheckHitKey(KEY_INPUT_W)) cameraPos.y -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_S)) cameraPos.y += cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_A)) cameraPos.x -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_D)) cameraPos.x += cameraSpeed;

    // マウスカーソル移動検知（動いた場合のみカメラ位置を更新）
    int mouseX = 0, mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

    static int prevMouseX = mouseX, prevMouseY = mouseY;
    if (mouseX != prevMouseX || mouseY != prevMouseY)
    {
        cameraPos.x = static_cast<float>(mouseX);
        cameraPos.y = static_cast<float>(mouseY);
        prevMouseX = mouseX;
        prevMouseY = mouseY;
    }

    // カメラ枠が画面外へ飛び出さないよう移動範囲を制御（クランプ処理）
    if (cameraPos.x < cameraSize.x * 0.5f) cameraPos.x = cameraSize.x * 0.5f;
    if (cameraPos.x > screenWidth - cameraSize.x * 0.5f) cameraPos.x = screenWidth - cameraSize.x * 0.5f;
    if (cameraPos.y < cameraSize.y * 0.5f) cameraPos.y = cameraSize.y * 0.5f;
    if (cameraPos.y > screenHeight - cameraSize.y * 0.5f) cameraPos.y = screenHeight - cameraSize.y * 0.5f;

    // ====================================================
    // 2. 被写体（オブジェクト）の移動 ＆ 端での跳ね返り
    // ====================================================
    for (auto& subj : subjects)
    {
        if (!subj.active) continue;

        subj.position.x += subj.velocity.x;

        // 画面左右の端に接触したら移動方向を反転
        if (subj.position.x - subj.size.x * 0.5f < 0.0f ||
            subj.position.x + subj.size.x * 0.5f > screenWidth)
        {
            subj.velocity.x *= -1.0f;
        }
    }

    // ====================================================
    // 3. シャッター入力判定 ＆ 撮影当たり判定
    // ====================================================
    // キー入力（Enterキーの押下瞬間を検知）
    bool isEnterPressed = (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1);
    static bool prevEnterState = false;
    bool isEnterTriggered = (isEnterPressed && !prevEnterState);
    prevEnterState = isEnterPressed;

    // パッド・マウス入力（左クリック判定）
    int button = GetButtonDown(PLAYER1);
    bool isMousePressed = (button & BUTTON_TRIGGER2) != 0;

    // Enterキーのトリガーまたはマウス左クリックで撮影実行
    bool isShutterTriggered = isEnterTriggered || isMousePressed;

    if (isShutterTriggered)
    {
        // 撮影エフェクト開始設定
        shutterAnimTimer = 1.0f; // 枠の拡大・縮小アニメーション開始
        flashAlpha = 180;        // フラッシュの初期不透明度（画面白化）

        // 現在のカメラ枠（ファインダー）の境界座標（AABB）を算出
        float camMinX = cameraPos.x - cameraSize.x * 0.5f;
        float camMaxX = cameraPos.x + cameraSize.x * 0.5f;
        float camMinY = cameraPos.y - cameraSize.y * 0.5f;
        float camMaxY = cameraPos.y + cameraSize.y * 0.5f;

        // 被写体ごとの枠内判定
        for (auto& subj : subjects)
        {
            if (!subj.active) continue;

            // 被写体の中心座標がカメラ枠内に完全に収まっているか判定
            bool isInCamera = (subj.position.x >= camMinX && subj.position.x <= camMaxX &&
                subj.position.y >= camMinY && subj.position.y <= camMaxY);

            if (isInCamera)
            {
                subj.active = false; // 撮影されたオブジェクトを非表示化

                // 種類に応じたスコア／HPの増減処理
                if (subj.type == BallType::Target)
                {
                    number++; // ターゲット撮影成功：加点
                }
                else if (subj.type == BallType::Penalty)
                {
                    HP--;     // お手付き対象を撮影：ダメージ
                }
            }
        }
    }

    // 撮影演出用タイマーと透明度の減衰減算
    if (shutterAnimTimer > 0.0f)
    {
        shutterAnimTimer -= 0.1f;
        if (shutterAnimTimer < 0.0f) shutterAnimTimer = 0.0f;
    }

    if (flashAlpha > 0)
    {
        flashAlpha -= 15;
        if (flashAlpha < 0) flashAlpha = 0;
    }
}

// ----------------------------------------------------
// 描画処理
// ----------------------------------------------------
void Game_01::Draw() const
{
    // ====================================================
    // 1. 背景描画（空・地面）
    // ====================================================
    // 空（画面全体）
    DxLib::DrawBox(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(200, 220, 240), TRUE);
    // 地面（y: 500以降）
    DxLib::DrawBox(0, 500, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(160, 160, 160), TRUE);

    // ====================================================
    // 2. 被写体（オブジェクト）描画
    // ====================================================
    for (const auto& subj : subjects)
    {
        if (!subj.active) continue;

        // タイプごとに色分け（Target: 茶色, Penalty: 青色）
        unsigned int color = (subj.type == BallType::Target)
            ? DxLib::GetColor(210, 140, 40)
            : DxLib::GetColor(40, 120, 210);

        DxLib::DrawBox(
            static_cast<int>(subj.position.x - subj.size.x * 0.5f),
            static_cast<int>(subj.position.y - subj.size.y * 0.5f),
            static_cast<int>(subj.position.x + subj.size.x * 0.5f),
            static_cast<int>(subj.position.y + subj.size.y * 0.5f),
            color, TRUE
        );
    }

    // ====================================================
    // 3. カメラ（ファインダー枠・照準）描画
    // ====================================================
    // 撮影瞬間にサイズが膨らむアニメーション用のスケール計算
    float scale = 1.0f + shutterAnimTimer * 0.15f;
    DxPlus::Vec2 currentCamSize = { cameraSize.x * scale, cameraSize.y * scale };

    int cMinX = static_cast<int>(cameraPos.x - currentCamSize.x * 0.5f);
    int cMinY = static_cast<int>(cameraPos.y - currentCamSize.y * 0.5f);
    int cMaxX = static_cast<int>(cameraPos.x + currentCamSize.x * 0.5f);
    int cMaxY = static_cast<int>(cameraPos.y + currentCamSize.y * 0.5f);

    // ファインダー外枠（太さを出すために重なる2つの枠を描画）
    DxLib::DrawBox(cMinX, cMinY, cMaxX, cMaxY, DxLib::GetColor(30, 30, 30), FALSE);
    DxLib::DrawBox(cMinX - 1, cMinY - 1, cMaxX + 1, cMaxY + 1, DxLib::GetColor(30, 30, 30), FALSE);

    // 中央照準（赤十字）
    DxLib::DrawLine(static_cast<int>(cameraPos.x) - 10, static_cast<int>(cameraPos.y), static_cast<int>(cameraPos.x) + 10, static_cast<int>(cameraPos.y), DxLib::GetColor(255, 0, 0));
    DxLib::DrawLine(static_cast<int>(cameraPos.x), static_cast<int>(cameraPos.y) - 10, static_cast<int>(cameraPos.x), static_cast<int>(cameraPos.y) + 10, DxLib::GetColor(255, 0, 0));

    // ====================================================
    // 4. フラッシュ演出描画（半透明白の全画面被せ）
    // ====================================================
    if (flashAlpha > 0)
    {
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashAlpha);
        DxLib::DrawBox(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(255, 255, 255), TRUE);
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 描画モードを元に戻す
    }

    // ====================================================
    // 5. UI（スコア・HP・操作ヘルプ）描画
    // ====================================================
    DxLib::DrawString(10, 680, L"【操作】WASD / マウス : カメラ移動 | Enter / クリック : 撮影", DxLib::GetColor(0, 0, 0));
}