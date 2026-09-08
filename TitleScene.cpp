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
    if (GetButtonDown(PLAYER1) & BUTTON_START || GetButtonDown(PLAYER1) & BUTTON_TRIGGER2)
    {
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

    if (isTitleInput)
    {
        if (title_character2) title_character2->Draw({ 960, 540 });
    }
    else
    {
        if (title_character) title_character->Draw({ 960, 540 });
    }

    int mouseX = 0;
    int mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

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
            DxLib::DrawGraph(mouseX-35, mouseY-10, handle, TRUE);
        }
    }
}

void TitleScene::Draw() const
{
    
}