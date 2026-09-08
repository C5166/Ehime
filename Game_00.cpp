// Game_00.cpp
#include "Game_00.h"
#include "ResourceKeys.h"
#include "ResourceManager.h"
#include "imgui.h"
#include <cstdlib>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct TargetInfo {
    const wchar_t* explanationKey; // ResourceKeys 内の文字列定数を受ける型
    ObjectType targetType;
    const char* jsonFileName;
};

static const TargetInfo g_TargetTable[] = {
    { ResourceKeys::game_setumei_9,  ObjectType::Nekosima, "game_setumei_9.json" },
    { ResourceKeys::game_setumei_10, ObjectType::Neko,     "game_setumei_10.json" },
    { ResourceKeys::game_setumei_11, ObjectType::Hitu,     "game_setumei_11.json" },
    { ResourceKeys::game_setumei_12, ObjectType::Inu,      "game_setumei_12.json" },
    { ResourceKeys::game_setumei_13, ObjectType::Kesi1,    "game_setumei_13.json" },
    { ResourceKeys::game_setumei_14, ObjectType::Kesi2,    "game_setumei_14.json" },
    { ResourceKeys::game_setumei_15, ObjectType::Kesi3,    "game_setumei_15.json" },
};

void Game_00::Init()
{
    CAMERA = RM().GetSound(ResourceKeys::SE_Camera);
    bubu = RM().GetSound(ResourceKeys::SE_bubu);

    good[0] = RM().GetSound(ResourceKeys::SE_GoodVoice1);
    good[1] = RM().GetSound(ResourceKeys::SE_GoodVoice2);
    good[2] = RM().GetSound(ResourceKeys::SE_GoodVoice3);

    perfect = RM().GetSound(ResourceKeys::SE_PerfectVoice);

    setumeivoice = RM().GetSound(ResourceKeys::SE_GameVoice1);

    Reset();
}

bool Game_00::AllTargetsCollected() const
{
    for (const auto& subj : subjects)
    {
        if (subj.active && subj.objType == targetObjectType)
        {
            return false;
        }
    }
    return true;
}

// --- JSON へ保存 ---
void Game_00::SaveToFile(const std::string& filename)
{
    json j = subjects;
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << j.dump(4);
    }
}

// --- JSON から読み込み ---
void Game_00::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        json j;
        file >> j;
        subjects = j.get<std::vector<Object>>();
        selectedObjectIndex = -1; // 選択状態をリセット

        // ロード時に nextUniqueId を更新（IDの重複防止）
        int maxId = 0;
        for (const auto& obj : subjects)
        {
            if (obj.id > maxId) maxId = obj.id;
        }
        nextUniqueId = maxId + 1;
    }
}

void Game_00::Reset()
{
    cameraPos = { screenWidth * 0.5f, screenHeight * 0.5f };
    shutterAnimTimer = 0.0f;
    flashAlpha = 0;
    selectedObjectIndex = -1;
    isCameraLocked = false;
    nextUniqueId = 1;

    subjects.clear();

    // g_TargetTable からランダムに目標を選択
    constexpr int tableSize = sizeof(g_TargetTable) / sizeof(g_TargetTable[0]);
    currentTargetIndex = rand() % tableSize;
    const auto& targetInfo = g_TargetTable[currentTargetIndex];

    // 目標となるオブジェクトタイプと説明画像ポインタをセット
    targetObjectType = targetInfo.targetType;
    currentExplanationSpr = RM().GridAt(targetInfo.explanationKey);

    // バッファを更新して該当の JSON をロード
    strcpy_s(saveFileNameBuf, sizeof(saveFileNameBuf), targetInfo.jsonFileName);
    LoadFromFile(saveFileNameBuf);

    // ロード失敗等でオブジェクトが存在しない場合のフォールバック設定
    if (subjects.empty())
    {
        AddObject(targetObjectType, "Target_Object");
    }
}

void Game_00::AddObject(ObjectType type, const char* defaultName)
{
    Object obj;
    obj.id = nextUniqueId++;
    obj.name = defaultName;
    obj.objType = type;
    obj.position = { screenWidth * 0.5f, screenHeight * 0.5f };
    obj.active = true;

    switch (type)
    {
    case ObjectType::Nekosima:
    case ObjectType::Neko:
    case ObjectType::Hitu:
    case ObjectType::Inu:
        obj.type = BallType::Target;
        obj.size = { 120.0f, 120.0f };
        break;
    case ObjectType::Kesi1:
    case ObjectType::Kesi2:
    case ObjectType::Kesi3:
        obj.type = BallType::Penalty;
        obj.size = { 120.0f, 120.0f };
        break;
    }

    subjects.push_back(obj);
    selectedObjectIndex = static_cast<int>(subjects.size()) - 1;
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
    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
    {
        return;
    }

    using namespace DxPlus::Input;

    int mouseButton = GetButtonDown(PLAYER1);
    static bool prevRightMouse = false;
    bool isRightMousePressed = (DxLib::GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;

    if (isRightMousePressed && !prevRightMouse)
    {
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            isCameraLocked = !isCameraLocked;
        }
    }
    prevRightMouse = isRightMousePressed;

    if (DxLib::CheckHitKey(KEY_INPUT_W)) cameraPos.y -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_S)) cameraPos.y += cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_A)) cameraPos.x -= cameraSpeed;
    if (DxLib::CheckHitKey(KEY_INPUT_D)) cameraPos.x += cameraSpeed;

    if (!isCameraLocked && !ImGui::GetIO().WantCaptureMouse)
    {
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
    }

    if (cameraPos.x < cameraSize.x * 0.5f) cameraPos.x = cameraSize.x * 0.5f;
    if (cameraPos.x > screenWidth - cameraSize.x * 0.5f) cameraPos.x = screenWidth - cameraSize.x * 0.5f;
    if (cameraPos.y < cameraSize.y * 0.5f) cameraPos.y = cameraSize.y * 0.5f;
    if (cameraPos.y > screenHeight - cameraSize.y * 0.5f) cameraPos.y = screenHeight - cameraSize.y * 0.5f;

    bool isEnterPressed = (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1);
    static bool prevEnterState = false;
    bool isEnterTriggered = (isEnterPressed && !prevEnterState);
    prevEnterState = isEnterPressed;

    int button = GetButtonDown(PLAYER1);
    bool isMousePressed = (button & BUTTON_TRIGGER2) != 0;

    if (isEnterTriggered || isMousePressed)
    {
        if (CheckSoundMem(CAMERA) == 0) {
            PlaySoundMem(CAMERA, DX_PLAYTYPE_BACK);
        }
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

            // ワールド座標で判定
            DxPlus::Vec2 worldPos = subj.GetWorldPosition(subjects);
            bool isInCamera = (worldPos.x >= camMinX && worldPos.x <= camMaxX &&
                worldPos.y >= camMinY && worldPos.y <= camMaxY);

            // 指定された targetObjectType を撮影できたか判定
            if (isInCamera && subj.objType == targetObjectType)
            {
                subj.active = false;
                score++;
                hitTarget = true;
                int a = GetRand(2);
                PlaySoundMem(good[a], DX_PLAYTYPE_BACK);
            }
        }

        // ターゲット以外を撮影、または空振りした場合はHPが削れる
        if (!hitTarget)
        {
            hp--;
            PlaySoundMem(bubu, DX_PLAYTYPE_BACK);
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
    for (const auto& subj : subjects)
    {
        if (!subj.active) continue;

        const auto* spr = GetSpriteForType(subj.objType);
        if (spr)
        {
            DxPlus::Vec2 worldPos = subj.GetWorldPosition(subjects);
            float worldRot = subj.GetWorldRotation(subjects);

            DxPlus::Vec2 scale = { subj.size.x / 100.0f, subj.size.y / 100.0f };
            spr->Draw(worldPos, scale, worldRot);
        }
        else
        {
            DxPlus::Vec2 worldPos = subj.GetWorldPosition(subjects);
            DxLib::DrawBox(
                static_cast<int>(worldPos.x - 20),
                static_cast<int>(worldPos.y - 20),
                static_cast<int>(worldPos.x + 20),
                static_cast<int>(worldPos.y + 20),
                DxLib::GetColor(255, 0, 0), TRUE
            );
        }
    }

    const auto* cameraSpr = RM().GridAt(ResourceKeys::game3_camera);
    if (cameraSpr)
    {
        cameraSpr->Draw(cameraPos);
    }

    if (flashAlpha > 0)
    {
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashAlpha);
        DxLib::DrawBox(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight), DxLib::GetColor(255, 255, 255), TRUE);
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
#ifdef _DEBUG
void Game_00::DrawImGui()
{
    ImGui::Begin("Hierarchy");

    if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
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

    if (ImGui::IsWindowFocused())
    {
        if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C) && selectedObjectIndex != -1)
        {
            copiedObjectId = subjects[selectedObjectIndex].id;
        }
        if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V) && copiedObjectId != -1)
        {
            DuplicateObjectRecursive(copiedObjectId, -1);
        }
    }

    for (size_t i = 0; i < subjects.size(); ++i)
    {
        if (subjects[i].parentId == -1)
        {
            DrawHierarchyTree(subjects[i].id);
        }
    }

    ImGui::Separator();

    ImGui::Text("Scene Save / Load");
    ImGui::InputText("File Name", saveFileNameBuf, sizeof(saveFileNameBuf));

    if (ImGui::Button("Save Scene"))
    {
        SaveToFile(saveFileNameBuf);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Scene"))
    {
        LoadFromFile(saveFileNameBuf);
    }

    ImGui::Text("Presets:");
    const char* presets[] = {
        "game_setumei_9.json",  "game_setumei_10.json",
        "game_setumei_11.json", "game_setumei_12.json",
        "game_setumei_13.json", "game_setumei_14.json",
        "game_setumei_15.json"
    };

    for (int i = 0; i < 7; ++i)
    {
        std::string btnLabel = std::to_string(i + 9);
        if (i > 0) ImGui::SameLine();

        if (ImGui::Button(btnLabel.c_str()))
        {
            strcpy_s(saveFileNameBuf, sizeof(saveFileNameBuf), presets[i]);
            LoadFromFile(saveFileNameBuf);
        }
    }

    ImGui::End();

    ImGui::Begin("Inspector");

    if (selectedObjectIndex >= 0 && selectedObjectIndex < static_cast<int>(subjects.size()))
    {
        Object& obj = subjects[selectedObjectIndex];

        char nameBuf[128];
        strcpy_s(nameBuf, sizeof(nameBuf), obj.name.c_str());
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf)))
        {
            obj.name = nameBuf;
        }

        ImGui::Checkbox("Active", &obj.active);
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            float pos[2] = { obj.position.x, obj.position.y };
            if (ImGui::DragFloat2("Position", pos, 1.0f))
            {
                obj.position.x = pos[0];
                obj.position.y = pos[1];
            }

            float deg = obj.rotation * (180.0f / 3.14159265f);
            if (ImGui::DragFloat("Rotation", &deg, 0.5f))
            {
                obj.rotation = deg * (3.14159265f / 180.0f);
            }

            float sz[2] = { obj.size.x, obj.size.y };
            if (ImGui::DragFloat2("Size", sz, 1.0f, 1.0f, 2000.0f))
            {
                obj.size.x = sz[0];
                obj.size.y = sz[1];
            }
        }

        DrawGizmo(obj);
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

void Game_00::DrawGizmo(Object& obj)
{
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    DxPlus::Vec2 worldPos = obj.GetWorldPosition(subjects);
    ImVec2 pos = ImVec2(worldPos.x, worldPos.y);
    ImVec2 mousePos = ImGui::GetMousePos();

    float hw = obj.size.x * 0.5f;
    float hh = obj.size.y * 0.5f;
    drawList->AddRect(
        ImVec2(pos.x - hw, pos.y - hh),
        ImVec2(pos.x + hw, pos.y + hh),
        IM_COL32(255, 255, 0, 255), 0.0f, 0, 1.5f
    );

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
            obj.rotation = std::atan2(dy, dx) + (3.14159265f * 0.5f);
        }
        else
        {
            isDraggingRot = false;
        }
    }
}
#endif
Object* Game_00::FindObjectById(int id)
{
    for (auto& obj : subjects) { if (obj.id == id) return &obj; }
    return nullptr;
}

const Object* Game_00::FindObjectById(int id) const
{
    for (const auto& obj : subjects) { if (obj.id == id) return &obj; }
    return nullptr;
}

void Game_00::SetParent(int childId, int newParentId)
{
    Object* child = FindObjectById(childId);
    if (!child || child->parentId == newParentId) return;

    if (child->parentId != -1)
    {
        Object* oldParent = FindObjectById(child->parentId);
        if (oldParent)
        {
            auto& list = oldParent->childIds;
            list.erase(std::remove(list.begin(), list.end(), childId), list.end());
        }
    }

    child->parentId = newParentId;
    if (newParentId != -1)
    {
        Object* newParent = FindObjectById(newParentId);
        if (newParent)
        {
            newParent->childIds.push_back(childId);
        }
    }
}

void Game_00::DrawHierarchyTree(int objId)
{
    Object* obj = FindObjectById(objId);
    if (!obj) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (selectedObjectIndex != -1 && subjects[selectedObjectIndex].id == objId)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (obj->childIds.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    std::string label = obj->name + (obj->active ? "" : " (Disabled)");

    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)objId, flags, "%s", label.c_str());

    if (ImGui::IsItemClicked())
    {
        for (int i = 0; i < static_cast<int>(subjects.size()); ++i)
        {
            if (subjects[i].id == objId)
            {
                selectedObjectIndex = i;
                break;
            }
        }
    }

    if (ImGui::BeginPopupContextItem("HierarchyObjContext"))
    {
        ImGui::PushID(objId);

        if (ImGui::MenuItem("Copy (Include Children)"))
        {
            copiedObjectId = objId;
        }
        if (ImGui::MenuItem("Paste As Child", nullptr, false, copiedObjectId != -1))
        {
            int createdId = DuplicateObjectRecursive(copiedObjectId, objId);
            SetParent(createdId, objId);
        }
        if (obj->parentId != -1)
        {
            if (ImGui::MenuItem("Unparent"))
            {
                SetParent(objId, -1);
            }
        }
        if (ImGui::MenuItem("Delete"))
        {
            SetParent(objId, -1);
            for (int cId : obj->childIds) { SetParent(cId, -1); }
            subjects.erase(std::remove_if(subjects.begin(), subjects.end(),
                [objId](const Object& o) { return o.id == objId; }), subjects.end());
            selectedObjectIndex = -1;

            ImGui::PopID();
            ImGui::EndPopup();
            if (nodeOpen) ImGui::TreePop();
            return;
        }

        ImGui::PopID();
        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("HIERARCHY_OBJ", &objId, sizeof(int));
        ImGui::Text("Move %s", obj->name.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_OBJ"))
        {
            int draggedId = *(const int*)payload->Data;
            if (draggedId != objId)
            {
                SetParent(draggedId, objId);
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (nodeOpen)
    {
        std::vector<int> children = obj->childIds;
        for (int childId : children)
        {
            DrawHierarchyTree(childId);
        }
        ImGui::TreePop();
    }
}

int Game_00::DuplicateObjectRecursive(int srcId, int newParentId)
{
    const Object* src = FindObjectById(srcId);
    if (!src) return -1;

    Object newObj = *src;
    int newId = nextUniqueId++;
    newObj.id = newId;
    newObj.name += "_Copy";
    newObj.parentId = newParentId;
    newObj.childIds.clear();

    if (newParentId == -1)
    {
        newObj.position.x += 20.0f;
        newObj.position.y += 20.0f;
    }

    subjects.push_back(newObj);

    std::vector<int> srcChildren = src->childIds;
    for (int childId : srcChildren)
    {
        int newChildId = DuplicateObjectRecursive(childId, newId);

        Object* parentObj = FindObjectById(newId);
        if (parentObj && newChildId != -1)
        {
            parentObj->childIds.push_back(newChildId);
        }
    }

    return newId;
}