#include "GameOverScene.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "SceneManager.h"

void GameOverScene::Init()
{
    // OS標準のマウスカーソルを非表示にする
    DxLib::SetMouseDispFlag(FALSE);

    backgroundSpr = RM().GridAt(ResourceKeys::Background);
    backgroundSpr2 = RM().GridAt(ResourceKeys::title_frame_2);
    gameover_background = RM().GridAt(ResourceKeys::gameover_background);
    gameover_character_1 = RM().GridAt(ResourceKeys::gameover_character_1);
    gameover_character_2 = RM().GridAt(ResourceKeys::gameover_character_2);

    // Reset game context once when entering GameOverScene (avoid per-frame reset)
    gameContext.Reset();

    touch[0] = RM().GetSound(ResourceKeys::SE_TouchVoice1);
    touch[1] = RM().GetSound(ResourceKeys::SE_TouchVoice2);
    touch[2] = RM().GetSound(ResourceKeys::SE_TouchVoice3);
    touch[3] = RM().GetSound(ResourceKeys::SE_TouchVoice4);
    touch[4] = RM().GetSound(ResourceKeys::SE_TouchVoice5);

    poti = RM().GetSound(ResourceKeys::SE_poti);
    kirakira = RM().GetSound(ResourceKeys::SE_kirakira);

    voice = RM().GetSound(ResourceKeys::SE_GameOverVoice);
    isGameOverInput = false;
    isGameOverInputCount = 0;
    a = 0;

    // アニメーション用タイマーのリセット
    logoAnimTimer = 0;
    currentLogoIndex = 0;

    bgm = RM().GetMusic(ResourceKeys::SE_GameOver);
    if (bgm >= 0)
    {
        PlaySoundMem(voice, DX_PLAYTYPE_BACK);
        DxLib::ChangeVolumeSoundMem(128, bgm);
        DxLib::PlaySoundMem(bgm, DX_PLAYTYPE_BACK);
    }

    StartFadeIn();
}

void GameOverScene::Update()
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

        // 円の当たり判定外の場合は入力を受け付けない
        if (!CheckMouseCircleCollision(mouseX, mouseY, mouseCollisionRadius, targetCenterX, targetCenterY, characterRadius))
        {
            return;
        }

        if (isGameOverInputCount <= isGameOverInputMax)
        {
            a++;
            PlaySoundMem(touch[a], DX_PLAYTYPE_BACK);
            if (a >= 4)
            {
                a = 0;
            }
        }

        isGameOverInput = true;
        isGameOverInputCount++;
    }
    else if (isGameOverInput)
    {
        isGameOverInput = false;
    }

    // カウント上限に達したら TitleScene へ遷移
    if (isGameOverInputCount >= isGameOverInputMax)
    {
        DxLib::StopSoundMem(bgm);
        Scene* titleScene = SceneManager::GetInstance().GetScene(SceneID::Title);
        SetNextScene(titleScene);
        StartFadeOut();
        if (CheckSoundMem(kirakira) == 0) {
            PlaySoundMem(kirakira, DX_PLAYTYPE_BACK);
        }
    }

    // ロゴのアニメーションを更新（常にループ）
    UpdateGameOverLogoAnimation();
}

void GameOverScene::UpdateGameOverLogoAnimation()
{
    logoAnimTimer++;
    if (logoAnimTimer >= animFrameInterval)
    {
        logoAnimTimer = 0;
        currentLogoIndex++;

        // 総コマ数（26）を超えたら 0 に戻して永遠にループさせる
        if (currentLogoIndex >= totalLogoFrames)
        {
            currentLogoIndex = 0;
        }
    }
}

void GameOverScene::DrawGameOverUI() const
{
    // 現在のインデックスから X (列) と Y (行) を計算
    int gridX = currentLogoIndex % logoColumns;
    int gridY = currentLogoIndex / logoColumns;

    const auto* Gameover_logo = RM().GridAt(ResourceKeys::gameover_logo, gridX, gridY);

    if (Gameover_logo)
    {
        // 拡大率(logoScale)を指定して描画
        Gameover_logo->Draw(logoPos, logoScale);
    }
}

void GameOverScene::Render() const
{
    if (backgroundSpr) backgroundSpr->Draw({});
    if (backgroundSpr2) backgroundSpr2->Draw({ 0, 0 });
    if (gameover_background) gameover_background->Draw({ 0, 0 });

    if (!isGameOverInput && gameover_character_1)
    {
        gameover_character_1->Draw({ 0, 0 });
    }
    else if (isGameOverInput && gameover_character_2)
    {
        gameover_character_2->Draw({ 0, 0 });
    }

    DrawGameOverUI();

    int mouseX = 0;
    int mouseY = 0;
    DxLib::GetMousePoint(&mouseX, &mouseY);

    // 判定円の実際の中心位置
    float targetCenterX = characterX + characterOffsetX;
    float targetCenterY = characterY + characterOffsetY;

    // 当たり判定チェック
    bool isHit = CheckMouseCircleCollision(mouseX, mouseY, mouseCollisionRadius, targetCenterX, targetCenterY, characterRadius);

    // 接触時は判定円の色を変更
    unsigned int charCircleColor = isHit ? DxLib::GetColor(255, 0, 0) : DxLib::GetColor(255, 255, 0);

    // --- 当たり判定ガイドライン（円）の描画 ---
   /* DxLib::DrawCircle(static_cast<int>(targetCenterX), static_cast<int>(targetCenterY), static_cast<int>(characterRadius), charCircleColor, FALSE);
    DxLib::DrawCircle(mouseX, mouseY, static_cast<int>(mouseCollisionRadius), DxLib::GetColor(0, 255, 0), FALSE);*/

    // --- 調整値ガイドライン情報描画 ---
   /* int white = DxLib::GetColor(255, 255, 255);
    DxLib::DrawFormatString(10, 10, charCircleColor, L"Hit: %s (Count: %d / %d)", isHit ? L"HIT!" : L"OUT", isGameOverInputCount, isGameOverInputMax);
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