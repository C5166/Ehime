// Game_00.cpp
#include "Game_00.h"
#include "ResourceManager.h"
#include "ResourceKeys.h"
#include "imgui.h"
#include <cstdlib>
#include <cmath>
#include <string>

void Game_00::Init()
{
    Reset();
}

void Game_00::Reset()
{
    cameraPos = { screenWidth * 0.5f, screenHeight * 0.5f };
    shutterAnimTimer = 0.0f;
    flashAlpha = 0;
    selectedObjectIndex = -1;

    subjects.clear();

    // デフォルトでサンプルオブジェクトを数個配置
    AddObject(ObjectType::Neko, "Neko_01");
    AddObject(ObjectType::Kesi1, "Kesi_01");
}

void Game_00::AddObject(ObjectType type, const char* defaultName)
{
    Object obj;
    obj.name = defaultName;
    obj.objType = type;
    obj.position = { screenWidth * 0.5f, screenHeight * 0.5f };
    obj.active = true;

    // タイプごとの初期化設定
    switch (type)
    {
    case ObjectType::Nekosima:
    case ObjectType::Neko:
    case ObjectType::Hitu:
    case ObjectType::Inu:
        obj.type = BallType::Target;
        obj.size = { 80.0f, 80.0f };
        break;
    case ObjectType::Kesi1:
    case ObjectType::Kesi2:
    case ObjectType::Kesi3:
        obj.type = BallType::Penalty;
        obj.size = { 100.0f, 70.0f };
        break;
    }

    subjects.push_back(obj);
    selectedObjectIndex = static_cast<int>(subjects.size()) - 1; // 生成したものを自動選択
}

const DxPlus::Sprite::SpriteBase* Game_00::GetSpriteForType(ObjectType type) const
{
    switch (type)
    {
    case ObjectType::Nekosima: return RM().GridAt(ResourceKeys::game_3_nekosima);
    case ObjectType::Neko:     return RM().GridAt(ResourceKeys::game_3_neko);
    case ObjectType::Hitu:     return RM().GridAt(ResourceKeys::game_3_hitu);
    case ObjectType::Inu:      return RM().GridAt(ResourceKeys::game_3_inu);
    case ObjectType::Kesi1:    return RM().GridAt(ResourceKeys::game_3_kesi1);
    case ObjectType::Kesi2:    return RM().GridAt(ResourceKeys::game_3_kesi2);
    case ObjectType::Kesi3:    return RM().GridAt(ResourceKeys::game_3_kesi3);
    }
    return nullptr;
}

void Game_00::Update(int& hp, int& score)
{
    // ※ ImGui操作中はゲーム内入力（カメラや判定など）をスキップしたい場合は
    // ImGui::GetIO().WantCaptureMouse をチェックしてください
    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    using namespace DxPlus::Input;

    // --- カメラ操作 ---
    if (DxLib::CheckHitKey(KEY_INPUT_W)) cameraPos.y -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_S)) cameraPos.y += cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_A)) cameraPos.x -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_D)) cameraPos.x += cameraSpeed;

    // カメラ枠制限
    if (cameraPos.x < cameraSize.x * 0.5f) cameraPos.x = cameraSize.x * 0.5f;
    if (cameraPos.x > screenWidth - cameraSize.x * 0.5f) cameraPos.x = screenWidth - cameraSize.x * 0.5f;
    if (cameraPos.y < cameraSize.y * 0.5f) cameraPos.y = cameraSize.y * 0.5f;
    if (cameraPos.y > screenHeight - cameraSize.y * 0.5f) cameraPos.y = screenHeight - cameraSize.y * 0.5f;

    // --- シャッター判定 ---
    bool isEnterPressed = (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1);
    static bool prevEnterState = false;
    bool isEnterTriggered = (isEnterPressed && !prevEnterState);
    prevEnterState = isEnterPressed;

    int button = GetButtonDown(PLAYER1);
    bool isMousePressed = (button & BUTTON_TRIGGER2) != 0;

    if (isEnterTriggered || isMousePressed)
    {
        shutterAnimTimer = 1.0f;
        flashAlpha = 180;

        float camMinX = cameraPos.x - cameraSize.x * 0.5f;
        float camMaxX = cameraPos.x + cameraSize.x * 0.5f;
        float camMinY = cameraPos.y - cameraSize.y * 0.5f;
        float camMaxY = cameraPos.y + cameraSize.y * 0.5f;

        bool hitTarget = false;

        for (auto& subj : subjects)
        {
            if (!subj.active) continue;

            bool isInCamera = (subj.position.x >= camMinX && subj.position.x <= camMaxX &&
                subj.position.y >= camMinY && subj.position.y <= camMaxY);

            if (isInCamera && subj.type == BallType::Target)
            {
                subj.active = false;
                score++;
                hitTarget = true;
            }
        }

        if (!hitTarget)
        {
            hp--;
        }
    }

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

void Game_00::Draw(int hp, int score) const
{
    // 1. オブジェクト描画
    for (const auto& subj : subjects)
    {
        if (!subj.active) continue;

        const auto* spr = GetSpriteForType(subj.objType);
        if (spr)
        {
            DxPlus::Vec2 scale = { subj.size.x / 100.0f, subj.size.y / 100.0f };

            // スケールと回転の両方を渡す
            spr->Draw(subj.position, scale, subj.rotation);
        }
        else
        {
            // フォールバック描画
            DxLib::DrawBox(
                static_cast<int>(subj.position.x - 20),
                static_cast<int>(subj.position.y - 20),
                static_cast<int>(subj.position.x + 20),
                static_cast<int>(subj.position.y + 20),
                DxLib::GetColor(255, 0, 0), TRUE
            );
        }
    }

    // 2. カメラ描画
    const auto* cameraSpr = RM().GridAt(ResourceKeys::game3_camera);
    if (cameraSpr)
    {
        cameraSpr->Draw(cameraPos);
    }

    // 3. フラッシュ演出
    if (flashAlpha > 0)
    {
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashAlpha);
        DxLib::DrawBox(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(255, 255, 255), TRUE);
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

// =========================================================================
// ImGui (Hierarchy, Inspector, Gizmo) 処理
// =========================================================================
void Game_00::DrawImGui()
{
    // ---------------------------------------------------------------------
    // 1. Hierarchy ウィンドウ
    // ---------------------------------------------------------------------
    ImGui::Begin("Hierarchy");

    // リスト領域での右クリックメニュー（オブジェクト追加）
    if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::BeginMenu("Create Object"))
        {
            if (ImGui::MenuItem("Neko"))     AddObject(ObjectType::Neko, "Neko");
            if (ImGui::MenuItem("Nekosima")) AddObject(ObjectType::Nekosima, "Nekosima");
            if (ImGui::MenuItem("Hitu"))     AddObject(ObjectType::Hitu, "Hitu");
            if (ImGui::MenuItem("Inu"))      AddObject(ObjectType::Inu, "Inu");
            ImGui::Separator();
            if (ImGui::MenuItem("Kesi1"))    AddObject(ObjectType::Kesi1, "Kesi1");
            if (ImGui::MenuItem("Kesi2"))    AddObject(ObjectType::Kesi2, "Kesi2");
            if (ImGui::MenuItem("Kesi3"))    AddObject(ObjectType::Kesi3, "Kesi3");
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    // オブジェクト一覧描画
    for (int i = 0; i < static_cast<int>(subjects.size()); ++i)
    {
        ImGui::PushID(i);
        bool isSelected = (selectedObjectIndex == i);

        std::string label = subjects[i].name + (subjects[i].active ? "" : " (Disabled)");
        if (ImGui::Selectable(label.c_str(), isSelected))
        {
            selectedObjectIndex = i;
        }

        // 要素個別の右クリックコンテキストメニュー（削除など）
        if (ImGui::BeginPopupContextItem())
        {
            selectedObjectIndex = i;
            if (ImGui::MenuItem("Delete Object"))
            {
                subjects.erase(subjects.begin() + i);
                if (selectedObjectIndex >= static_cast<int>(subjects.size()))
                {
                    selectedObjectIndex = static_cast<int>(subjects.size()) - 1;
                }
                ImGui::EndPopup();
                ImGui::PopID();
                break;
            }
            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImGui::End();

    // ---------------------------------------------------------------------
    // 2. Inspector ウィンドウ
    // ---------------------------------------------------------------------
    ImGui::Begin("Inspector");

    if (selectedObjectIndex >= 0 && selectedObjectIndex < static_cast<int>(subjects.size()))
    {
        Object& obj = subjects[selectedObjectIndex];

        // 名前変更
        char nameBuf[128];
        strncpy_s(nameBuf, obj.name.c_str(), sizeof(nameBuf));
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf)))
        {
            obj.name = nameBuf;
        }

        ImGui::Checkbox("Active", &obj.active);
        ImGui::Separator();

        // --- Transform セクション ---
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Position
            float pos[2] = { obj.position.x, obj.position.y };
            if (ImGui::DragFloat2("Position", pos, 1.0f))
            {
                obj.position.x = pos[0];
                obj.position.y = pos[1];
            }

            // Rotation (度数法で操作して内部ではラジアン保持)
            float deg = obj.rotation * (180.0f / 3.14159265f);
            if (ImGui::DragFloat("Rotation", &deg, 0.5f))
            {
                obj.rotation = deg * (3.14159265f / 180.0f);
            }

            // Size (Scale)
            float sz[2] = { obj.size.x, obj.size.y };
            if (ImGui::DragFloat2("Size", sz, 1.0f, 1.0f, 2000.0f))
            {
                obj.size.x = sz[0];
                obj.size.y = sz[1];
            }
        }

        // 画面上ギズモの描画処理を適用
        DrawGizmo(obj);
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

// -------------------------------------------------------------------------
// 3. 2D ギズモ描画・ドラッグ移動/回転機能
// -------------------------------------------------------------------------
void Game_00::DrawGizmo(Object& obj)
{
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    ImVec2 pos = ImVec2(obj.position.x, obj.position.y);
    ImVec2 mousePos = ImGui::GetMousePos();

    // 選択枠（バウンディングボックス）描画
    float hw = obj.size.x * 0.5f;
    float hh = obj.size.y * 0.5f;
    drawList->AddRect(
        ImVec2(pos.x - hw, pos.y - hh),
        ImVec2(pos.x + hw, pos.y + hh),
        IM_COL32(255, 255, 0, 255), 0.0f, 0, 1.5f
    );

    // 中心ハンドル (ドラッグで移動)
    static bool isDraggingMove = false;
    static ImVec2 dragOffset = ImVec2(0, 0);

    float centerRadius = 10.0f;
    float distToCenter = std::hypot(mousePos.x - pos.x, mousePos.y - pos.y);
    bool isHoverCenter = distToCenter <= centerRadius;

    ImU32 centerColor = isHoverCenter ? IM_COL32(255, 255, 0, 255) : IM_COL32(255, 0, 0, 255);
    drawList->AddCircleFilled(pos, centerRadius, centerColor);

    if (ImGui::IsMouseClicked(0) && isHoverCenter)
    {
        isDraggingMove = true;
        dragOffset = ImVec2(mousePos.x - pos.x, mousePos.y - pos.y);
    }
    if (isDraggingMove)
    {
        if (ImGui::IsMouseDown(0))
        {
            obj.position.x = mousePos.x - dragOffset.x;
            obj.position.y = mousePos.y - dragOffset.y;
        }
        else
        {
            isDraggingMove = false;
        }
    }

    // 回転ハンドル (上の青い円)
    static bool isDraggingRot = false;
    ImVec2 rotHandlePos = ImVec2(pos.x, pos.y - hh - 30.0f);
    drawList->AddLine(ImVec2(pos.x, pos.y - hh), rotHandlePos, IM_COL32(0, 255, 255, 255), 2.0f);

    float distToRot = std::hypot(mousePos.x - rotHandlePos.x, mousePos.y - rotHandlePos.y);
    bool isHoverRot = distToRot <= 8.0f;

    ImU32 rotColor = isHoverRot ? IM_COL32(255, 255, 0, 255) : IM_COL32(0, 200, 255, 255);
    drawList->AddCircleFilled(rotHandlePos, 8.0f, rotColor);

    if (ImGui::IsMouseClicked(0) && isHoverRot)
    {
        isDraggingRot = true;
    }
    if (isDraggingRot)
    {
        if (ImGui::IsMouseDown(0))
        {
            float dx = mousePos.x - pos.x;
            float dy = mousePos.y - pos.y;
            obj.rotation = std::atan2(dy, dx) + (3.14159265f * 0.5f); // 上方向を0度とする調整
        }
        else
        {
            isDraggingRot = false;
        }
    }
}