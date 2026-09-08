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
    // Reset game context once on entering GameClearScene (avoid heavy per-frame reset)
    gameContext.Reset();
}

void GameClearScene::Update()
{
    using namespace DxPlus::Input;

    // --- デバッグ用パラメータ調整操作 ---
    if (DxLib::CheckHitKey(KEY_INPUT_UP))   characterRadius += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_DOWN)) characterRadius = (std::max)(1.0f, characterRadius - 1.0f);

    if (DxLib::CheckHitKey(KEY_INPUT_RIGHT)) mouseCollisionRadius += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_LEFT))  mouseCollisionRadius = (std::max)(1.0f, mouseCollisionRadius - 1.0f);

    if (DxLib::CheckHitKey(KEY_INPUT_W)) characterOffsetY -= 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_S)) characterOffsetY += 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_A)) characterOffsetX -= 1.0f;
    if (DxLib::CheckHitKey(KEY_INPUT_D)) characterOffsetX += 1.0f;

    int buttonDown = GetButtonDown(PLAYER1);

    if (buttonDown & BUTTON_START || buttonDown & BUTTON_TRIGGER2)
    {
        int mouseX = 0, mouseY = 0;
        DxLib::GetMousePoint(&mouseX, &mouseY);

        float targetCenterX = characterX + characterOffsetX;
        float targetCenterY = characterY + characterOffsetY;

        // 円の判定外をクリックした場合は処理しない
        if (!CheckMouseCircleCollision(mouseX, mouseY, mouseCollisionRadius, targetCenterX, targetCenterY, characterRadius))
        {
            return;
        }

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

    // カウント上限達成でタイトルシーンへ遷移
    if (isGameClearInputCount >= isGameClearInputMax)
    {
        DxLib::StopSoundMem(bgm);
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
        if (CheckSoundMem(kirakira) == 0) {
            PlaySoundMem(kirakira, DX_PLAYTYPE_BACK);
        }
    }

    UpdateGameClearLogoAnimation();
}

void GameClearScene::UpdateGameClearLogoAnimation()
{
    logoAnimTimer++;
    if (logoAnimTimer >= animFrameInterval)
    {
        logoAnimTimer = 0;
        currentLogoIndex++;

        if (currentLogoIndex >= totalLogoFrames)
        {
            currentLogoIndex = 0;
        }
    }
}

void GameClearScene::DrawGameClearUI() const
{
    int gridX = currentLogoIndex % logoColumns;
    int gridY = currentLogoIndex / logoColumns;

    const auto* Gameclear_logo = RM().GridAt(ResourceKeys::gameclear_logo, gridX, gridY);

    if (Gameclear_logo)
    {
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

    DrawGameClearUI();

    int mouseX = 0;
    int mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

    // 判定円の中心座標
    float targetCenterX = characterX + characterOffsetX;
    float targetCenterY = characterY + characterOffsetY;

    // 当たり判定チェック
    bool isHit = CheckMouseCircleCollision(mouseX, mouseY, mouseCollisionRadius, targetCenterX, targetCenterY, characterRadius);

    // 接触時は判定円の色を変更
    unsigned int charCircleColor = isHit ? DxLib::GetColor(255, 0, 0) : DxLib::GetColor(255, 255, 0);

    // --- 当たり判定ガイドライン（円）描画 ---
   /* DxLib::DrawCircle(static_cast<int>(targetCenterX), static_cast<int>(targetCenterY), static_cast<int>(characterRadius), charCircleColor, FALSE);
    DxLib::DrawCircle(mouseX, mouseY, static_cast<int>(mouseCollisionRadius), DxLib::GetColor(0, 255, 0), FALSE);*/

    // --- 調整値デバッグ情報描画 ---
 /*   int white = DxLib::GetColor(255, 255, 255);
    DxLib::DrawFormatString(10, 10, charCircleColor, L"Hit: %s (Count: %d / %d)", isHit ? L"HIT!" : L"OUT", isGameClearInputCount, isGameClearInputMax);
    DxLib::DrawFormatString(10, 30, white, L"Char Radius [UP/DOWN]: %.1f", characterRadius);
    DxLib::DrawFormatString(10, 50, white, L"Mouse Radius [LEFT/RIGHT]: %.1f", mouseCollisionRadius);
    DxLib::DrawFormatString(10, 70, white, L"Char Offset [W/A/S/D]: X:%.1f, Y:%.1f", characterOffsetX, characterOffsetY);*/

    // カーソル描画
    bool isClicking = (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
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