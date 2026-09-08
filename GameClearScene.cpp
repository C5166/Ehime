#include "GameClearScene.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "SceneManager.h"

void GameClearScene::Init()
{
    backgroundSpr = RM().GridAt(ResourceKeys::Background);
    backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2);
    gameclear_background = RM().GridAt(ResourceKeys::gameclear_background);
    gameclear_character_1 = RM().GridAt(ResourceKeys::gameclear_character_1);
    gameclear_character_2 = RM().GridAt(ResourceKeys::gameclear_character_2);

    isGameClearInput = false;
    isGameClearInputCount = 0;

    // アニメーション用タイマーのリセット
    logoAnimTimer = 0;
    currentLogoIndex = 0;

    touch[0] = RM().GetSound(ResourceKeys::SE_TouchVoice1);
    touch[1] = RM().GetSound(ResourceKeys::SE_TouchVoice2);
    touch[2] = RM().GetSound(ResourceKeys::SE_TouchVoice3);
    touch[3] = RM().GetSound(ResourceKeys::SE_TouchVoice4);
    touch[4] = RM().GetSound(ResourceKeys::SE_TouchVoice5);

    poti = RM().GetSound(ResourceKeys::SE_poti);
    kirakira = RM().GetSound(ResourceKeys::SE_kirakira);


    voice = RM().GetSound(ResourceKeys::SE_GameClearVoice);
    int bgm = RM().GetMusic(ResourceKeys::BGM_Game);
    if (bgm >= 0)
    {
        PlaySoundMem(voice, DX_PLAYTYPE_BACK);
        DxLib::ChangeVolumeSoundMem(128, bgm);
        DxLib::PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
    }
    StartFadeIn();
}

void GameClearScene::Update()
{
	gameContext.Reset();
    using namespace DxPlus::Input;
    int buttonDown = GetButtonDown(PLAYER1);

    if (buttonDown & BUTTON_START || buttonDown & BUTTON_TRIGGER2)
    {
        if (isGameClearInputCount <= isGameClearInputMax)
        {
            a++;
            PlaySoundMem(touch[a], DX_PLAYTYPE_BACK);
            if (a >= 4)
            {
                a = 0;
            }
        }
        isGameClearInput = true;
        isGameClearInputCount++;
    }
    else if (isGameClearInput)
    {
        isGameClearInput = false;
    }

    // カウント上限に達したら TitleScene へ遷移
    if (isGameClearInputCount >= isGameClearInputMax)
    {
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
        if (CheckSoundMem(kirakira) == 0) {
            PlaySoundMem(kirakira, DX_PLAYTYPE_BACK);
        }
    }

    // ロゴのアニメーションを更新（常にループ）
    UpdateGameClearLogoAnimation();
}

void GameClearScene::UpdateGameClearLogoAnimation()
{
    logoAnimTimer++;
    if (logoAnimTimer >= animFrameInterval)
    {
        logoAnimTimer = 0;
        currentLogoIndex++;

        // 総コマ数（21コマ）を超えたら 0 に戻してループさせる
        if (currentLogoIndex >= totalLogoFrames)
        {
            currentLogoIndex = 0;
        }
    }
}

void GameClearScene::DrawGameClearUI() const
{
    // 現在のインデックスから X (列) と Y (行) を計算
    int gridX = currentLogoIndex % logoColumns;
    int gridY = currentLogoIndex / logoColumns;

    const auto* Gameclear_logo = RM().GridAt(ResourceKeys::gameclear_logo, gridX, gridY);
    if (Gameclear_logo)
    {
        // 拡大率(logoScale)を指定して描画
        Gameclear_logo->Draw(logoPos, logoScale);
    }
}

void GameClearScene::Render() const
{
    if (backgroundSpr) backgroundSpr->Draw({});
    if (backgroundSpr2) backgroundSpr2->Draw({ 0, 0 });
    if (gameclear_background) gameclear_background->Draw({ 0, 0 });

    if (!isGameClearInput && gameclear_character_1)
    {
        gameclear_character_1->Draw({ 0, 0 });
    }
    else if (isGameClearInput && gameclear_character_2)
    {
        gameclear_character_2->Draw({ 0, 0 });
    }

    // ゲームクリアロゴを描画
    DrawGameClearUI();

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
            DxLib::DrawGraph(mouseX - 35, mouseY - 10, handle, TRUE);
        }
    }
}