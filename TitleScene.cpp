// =============================
// Scenes/Title/TitleScene.cpp
// =============================
#include "TitleScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ResourceKeys.h"
#include "GameContext.h"

void TitleScene::Init()
{
    // OS標準のマウスカーソルを非表示にする
    DxLib::SetMouseDispFlag(FALSE);

    DxLib::SetBackgroundColor(16, 128, 224);
    frameCount = 0;

    startvoice = RM().GetSound(ResourceKeys::SE_StartVoice);
    fontHandle = RM().GetFont(ResourceKeys::Font_Title);

    bgSprite = RM().GridAt(ResourceKeys::Title_frame);
    title_character = RM().GridAt(ResourceKeys::title_character);
    title_character2 = RM().GridAt(ResourceKeys::title_character_2);

    isTitleInput = false;
    isTitleInputCount = 0;

	sousasetumei = RM().GridAt(ResourceKeys::sousasetumei);

    poti = RM().GetSound(ResourceKeys::SE_poti);
    kirakira = RM().GetSound(ResourceKeys::SE_kirakira);

    touch[0] = RM().GetSound(ResourceKeys::SE_TouchVoice1);
    touch[1] = RM().GetSound(ResourceKeys::SE_TouchVoice2);
    touch[2] = RM().GetSound(ResourceKeys::SE_TouchVoice3);
    touch[3] = RM().GetSound(ResourceKeys::SE_TouchVoice4);
    touch[4] = RM().GetSound(ResourceKeys::SE_TouchVoice5);

    // アニメーション制御変数のリセット
    bgAnimTimer = 0;
    currentBgIndex = 0;

    int bgm = RM().GetMusic(ResourceKeys::BGM_Game);
    if (bgm >= 0)
    {
        DxLib::StopSoundMem(bgm);
        DxLib::ChangeVolumeSoundMem(128, bgm);
        DxLib::PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
    }
    StartFadeIn();
}

void TitleScene::Update()
{
    using namespace DxPlus::Input;

    // --- デバッグ用パラメータ調整操作 ---
    // [UP / DOWN]: キャラクター判定半径の変更
    if (DxLib::CheckHitKey(KEY_INPUT_UP))   characterRadius += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_DOWN)) characterRadius = (std::max)(1.0f, characterRadius - 1.0f);

    // [RIGHT / LEFT]: マウス判定半径の変更
    if (DxLib::CheckHitKey(KEY_INPUT_RIGHT)) mouseCollisionRadius += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_LEFT))  mouseCollisionRadius = (std::max)(1.0f, mouseCollisionRadius - 1.0f);

    // [W / S / A / D]: キャラクター判定円の中心位置の調整
    if (DxLib::CheckHitKey(KEY_INPUT_W)) characterOffsetY -= 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_S)) characterOffsetY += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_A)) characterOffsetX -= 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_D)) characterOffsetX += 1.0f;

    // --- クリック・決定処理 ---
    if (GetButtonDown(PLAYER1) & BUTTON_START || GetButtonDown(PLAYER1) & BUTTON_TRIGGER2)
    {
        int mouseX = 0, mouseY = 0;
        DxLib::GetMousePoint(&mouseX, &mouseY);

        // オフセットを加味したキャラ判定位置
        float targetCenterX = characterX + characterOffsetX;
        float targetCenterY = characterY + characterOffsetY;

        // 円の範囲外をクリックした場合はカウントしない
        if (!CheckMouseCircleCollision(mouseX, mouseY, mouseCollisionRadius, targetCenterX, targetCenterY, characterRadius))
        {
            return;
        }

        // --- 以下、円内クリック成功時の処理 ---
        PlaySoundMem(poti, DX_PLAYTYPE_BACK);
        if (TenCount == 10)
        {
            Scene* gameScene = SceneManager::GetInstance().GetScene(SceneID::Game);
            SetNextScene(gameScene);
            TenCount = 0;
            StartFadeOut();
            if (CheckSoundMem(kirakira) == 0) {
                PlaySoundMem(kirakira, DX_PLAYTYPE_BACK);
            }
            WaitTimer(1500);
            if (CheckSoundMem(startvoice) == 0) {
                PlaySoundMem(startvoice, DX_PLAYTYPE_BACK);
            }
        }
        else
        {
            a++;
            PlaySoundMem(touch[a], DX_PLAYTYPE_BACK);
            if (a >= 4)
            {
                a = 0;
            }
        }

        TenCount++;

        if (isTitleInput == false)
        {
            isTitleInput = true;
        }

        return;
    }

    if (isTitleInput)
    {
        isTitleInputCount++;
    }

    if (isTitleInput && isTitleInputCount > isTitleInputMax)
    {
        isTitleInput = false;
        isTitleInputCount = 0;
    }

    frameCount++;

    // 背景アニメーションの更新（無限ループ）
    UpdateTitleBgAnimation();
}

void TitleScene::UpdateTitleBgAnimation()
{
    bgAnimTimer++;
    if (bgAnimTimer >= animFrameInterval)
    {
        bgAnimTimer = 0;
        currentBgIndex++;

        // 総フレーム数（37）を超えたら 0 に戻して無限ループ
        if (currentBgIndex >= totalBgFrames)
        {
            currentBgIndex = 0;
        }
    }
}

void TitleScene::Render() const
{
    // --- 1. アニメーション背景の描画 ---
    int gridX = currentBgIndex % bgColumns;
    int gridY = currentBgIndex / bgColumns;

    const auto* titleBgAnim = RM().GridAt(ResourceKeys::title_background, gridX, gridY);
    if (titleBgAnim)
    {
        titleBgAnim->Draw(bgPos, bgScale);
    }
    if (bgSprite)
    {
        // フォールバック描画
        bgSprite->Draw({ 0, 0 });
    }

    // キャラクター本体描画
    if (isTitleInput)
    {
        if (title_character2) title_character2->Draw({ characterX, characterY });
    }
    else
    {
        if (title_character) title_character->Draw({ characterX, characterY });
    }

    int mouseX = 0, mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

    // 判定円の実際の中心位置
    float targetCenterX = characterX + characterOffsetX;
    float targetCenterY = characterY + characterOffsetY;

    // 当たり判定チェック
    bool isHit = CheckMouseCircleCollision(mouseX, mouseY, mouseCollisionRadius, targetCenterX, targetCenterY, characterRadius);

    // 接触時は黄色円を赤色に変化
    unsigned int charCircleColor = isHit ? DxLib::GetColor(255, 0, 0) : DxLib::GetColor(255, 255, 0);

	sousasetumei->Draw({ 241, DxPlus::CLIENT_HEIGHT - 50});

    // --- 当たり判定ガイドライン（円）の描画 ---
    /*DxLib::DrawCircle(static_cast<int>(targetCenterX), static_cast<int>(targetCenterY), static_cast<int>(characterRadius), charCircleColor, FALSE);
    DxLib::DrawCircle(mouseX, mouseY, static_cast<int>(mouseCollisionRadius), DxLib::GetColor(0, 255, 0), FALSE);*/

    // --- 調整値ガイドライン情報描画 ---
   /* int white = DxLib::GetColor(255, 255, 255);
    DxLib::DrawFormatString(10, 10, charCircleColor, L"Hit: %s (Count: %d / 10)", isHit ? L"HIT!" : L"OUT", TenCount);
    DxLib::DrawFormatString(10, 30, white, L"Char Radius [UP/DOWN]: %.1f", characterRadius);
    DxLib::DrawFormatString(10, 50, white, L"Mouse Radius [LEFT/RIGHT]: %.1f", mouseCollisionRadius);
    DxLib::DrawFormatString(10, 70, white, L"Char Offset [W/A/S/D]: X:%.1f, Y:%.1f", characterOffsetX, characterOffsetY);*/

    // マウス左ボタンが押されているか判定
    bool isClicking = (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    // 押されている場合は cursor_2（グー）、離している場合は cursor_1（パー）
    const wchar_t* cursorKey = isClicking ? ResourceKeys::cursor_2 : ResourceKeys::cursor_1;

    const auto* sprite = RM().GridAt(cursorKey);
    if (sprite)
    {
        int handle = sprite->GetID();
        if (handle != -1)
        {
            DxLib::DrawGraph(mouseX - 52, mouseY - 50, handle, TRUE);
        }
    }
}

void TitleScene::Draw() const
{

}