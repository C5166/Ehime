// =============================
// Resources/ResourceManager.cpp
// =============================
#include "ResourceManager.h"
#include "DxPlus/DxPlus.h"
#include "ResourceKeys.h"

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadAll()
{
    LoadTextureAsSpriteLeftTop(ResourceKeys::Background, L"./Data/Images/game_background.png");

   /* LoadGridCenter(ResourceKeys::Title_background, L"./Data/Images/title_background.png", { 0,0 }, { 10,8 }, { 960, 540 });*/
	LoadTextureAsSpriteLeftTop(ResourceKeys::Title_frame, L"./Data/Images/title_frame.png");
    LoadTextureAsSpriteCenter(ResourceKeys::title_character, L"./Data/Images/title_character.png");
    LoadTextureAsSpriteCenter(ResourceKeys::title_character_2, L"./Data/Images/title_character_2.png");

    LoadGridBottom(ResourceKeys::title_background, L"./Data/Images/title_background.png.png", { 0,0 }, { 10,4 }, { 768, 432 });

    LoadGridCenter(ResourceKeys::number_countdown_b, L"./Data/Images/number_countdown_b.png", { 0,0 }, { 10,1 }, { 180,126 });
    LoadGridCenter(ResourceKeys::number_countdown_321, L"./Data/Images/number_countdown_321.png", { 0,0 }, { 10,18 }, { 180,126 });

    LoadGridLeftTop(ResourceKeys::transition_1, L"./Data/Images/transition_2.png", { 0, 0 }, { 10, 2 }, { 768, 432 });
    LoadGridLeftTop(ResourceKeys::transition_2, L"./Data/Images/transition_1.png", { 0, 0 }, { 10, 2 }, { 768, 432 });

    LoadTextureAsSpriteLeftTop(ResourceKeys::cursor_1, L"./Data/Images/cursor_1.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::cursor_2, L"./Data/Images/cursor_2.png");


	//GameClearの画像
    LoadTextureAsSpriteLeftTop(ResourceKeys::gameclear_background, L"./Data/Images/gameclear_background.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::gameclear_character_1, L"./Data/Images/gameclear_character_1.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::gameclear_character_2, L"./Data/Images/gameclear_character_2.png");
    LoadGridCenter(ResourceKeys::gameclear_logo, L"./Data/Images/gameclear_logo.png", { 0,0 }, { 10,3 }, { 496,165 });

	LoadTextureAsSpriteLeftTop(ResourceKeys::gameover_background, L"./Data/Images/gameover_background.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::gameover_character_1, L"./Data/Images/gameover_character_1.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::gameover_character_2, L"./Data/Images/gameover_character_2.png");
	LoadGridCenter(ResourceKeys::gameover_logo, L"./Data/Images/gameover_logo.png", { 0,0 }, { 10,3 }, { 528,184 });

    LoadGridCenter(ResourceKeys::game_hp_1, L"./Data/Images/game_hp_1.png", { 0,0 }, { 10,2 }, { 70,70 });
    LoadGridCenter(ResourceKeys::game_hp_2, L"./Data/Images/game_hp_2.png", { 0,0 }, { 10,2 }, { 70,70 });
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_1, L"./Data/Images/game_setumei_1.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_2, L"./Data/Images/game_setumei_2.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_3, L"./Data/Images/game_setumei_3.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_4, L"./Data/Images/game_setumei_4.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_5, L"./Data/Images/game_setumei_5.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_6, L"./Data/Images/game_setumei_6.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_7, L"./Data/Images/game_setumei_7.png");
    LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_8, L"./Data/Images/game_setumei_8.png");

    LoadGridCenter(ResourceKeys::game_start123, L"./Data/Images/game_start123.png", { 0, 0 }, { 3, 1 }, { 300, 379 });

    LoadTextureAsSpriteCenter(ResourceKeys::game_start, L"./Data/Images/game_start.png");

	LoadTextureAsSpriteLeftTop(ResourceKeys::game2_back, L"./Data/Images/mihon_3.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::title_frame_2, L"./Data/Images/title_frame_2.png");

	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_9, L"./Data/Images/game_setumei_9.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_10, L"./Data/Images/game_setumei_10.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_11, L"./Data/Images/game_setumei_11.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_12, L"./Data/Images/game_setumei_12.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_13, L"./Data/Images/game_setumei_13.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_14, L"./Data/Images/game_setumei_14.png");
	LoadTextureAsSpriteLeftTop(ResourceKeys::game_setumei_15, L"./Data/Images/game_setumei_15.png");

	LoadTextureAsSpriteCenter(ResourceKeys::game_1_heart_1, L"./Data/Images/game_1_heart_1.png");
    LoadTextureAsSpriteCenter(ResourceKeys::game_1_heart_2, L"./Data/Images/game_1_heart_2.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_1_heart_3, L"./Data/Images/game_1_heart_3.png");


	LoadTextureAsSpriteCenter(ResourceKeys::game3_camera, L"./Data/Images/game3_camera.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_nekosima, L"./Data/Images/game_3_nekosima.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_neko, L"./Data/Images/game_3_neko.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_hitu, L"./Data/Images/game_3_hitu.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_inu, L"./Data/Images/game_3_inu.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_kesi1, L"./Data/Images/game_3_kesi1.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_kesi2, L"./Data/Images/game_3_kesi2.png");
	LoadTextureAsSpriteCenter(ResourceKeys::game_3_kesi3, L"./Data/Images/game_3_kesi3.png");

    LoadMusic(ResourceKeys::BGM_Game,       L"./Data/Sounds/sanjinooyatsu.mp3");
    LoadMusic(ResourceKeys::SE_GameOver,       L"./Data/Sounds/bgm_gameover.mp3");
    LoadSound(ResourceKeys::SE_GameOverVoice, L"./Data/Sounds/ゲームオーバ/voice_gameover.mp3");
    LoadSound(ResourceKeys::SE_GameClearVoice, L"./Data/Sounds/ゲームクリア/voice_gameclear.mp3");

    LoadSound(ResourceKeys::SE_GameVoice1_1, L"./Data/Sounds/説明ボイス/voice_setumei_1.mp3");//ハート拾い
    LoadSound(ResourceKeys::SE_GameVoice1_2, L"./Data/Sounds/説明ボイス/voice_setumei_2.mp3");//ハート拾うな
    LoadSound(ResourceKeys::SE_GameVoice2_1, L"./Data/Sounds/説明ボイス/voice_setumei_3.mp3");//多いほう
    LoadSound(ResourceKeys::SE_GameVoice2_2, L"./Data/Sounds/説明ボイス/voice_setumei_5.mp3");//少ないほう
    LoadSound(ResourceKeys::SE_GameVoice3_1, L"./Data/Sounds/説明ボイス/voice_setumei_6.mp3");//犬探す
    LoadSound(ResourceKeys::SE_GameVoice3_2, L"./Data/Sounds/説明ボイス/voice_setumei_8.mp3");//猫探す
    LoadSound(ResourceKeys::SE_GameVoice3_3, L"./Data/Sounds/説明ボイス/voice_setumei_9.mp3");//消しゴム探す
    LoadSound(ResourceKeys::SE_GameVoice3_4, L"./Data/Sounds/説明ボイス/voice_setumei_4.mp3");//鉛筆探す


    LoadSound(ResourceKeys::SE_Count_1, L"./Data/Sounds/se_count_987654.mp3");
    LoadSound(ResourceKeys::SE_Count_2, L"./Data/Sounds/se_count_321.mp3");

    LoadSound(ResourceKeys::SE_Explosion,   L"./Data/Sounds/Explosion.mp3");

	LoadSound(ResourceKeys::SE_TouchVoice1, L"./Data/Sounds/タイトル_胸を押したときのボイス/voice_mune_baka.mp3");
	LoadSound(ResourceKeys::SE_TouchVoice2, L"./Data/Sounds/タイトル_胸を押したときのボイス/voice_mune_kya.mp3");
	LoadSound(ResourceKeys::SE_TouchVoice3, L"./Data/Sounds/タイトル_胸を押したときのボイス/voice_mune_mou.mp3");
	LoadSound(ResourceKeys::SE_TouchVoice4, L"./Data/Sounds/タイトル_胸を押したときのボイス/voice_mune_nnn.mp3");
	LoadSound(ResourceKeys::SE_TouchVoice5, L"./Data/Sounds/タイトル_胸を押したときのボイス/voice_mune_wa.mp3");
	LoadSound(ResourceKeys::SE_GoodVoice1, L"./Data/Sounds/正解オブジェクトを選択した/voice_seikai_1.mp3");
	LoadSound(ResourceKeys::SE_GoodVoice2, L"./Data/Sounds/正解オブジェクトを選択した/voice_seikai_2.mp3");
	LoadSound(ResourceKeys::SE_GoodVoice3, L"./Data/Sounds/正解オブジェクトを選択した/voice_seikai_3.mp3");
	LoadSound(ResourceKeys::SE_PerfectVoice, L"./Data/Sounds/パーフェクト/voice_perfect.mp3");
	LoadSound(ResourceKeys::SE_poti, L"./Data/Sounds/se_poti.mp3");
	LoadSound(ResourceKeys::SE_kirakira, L"./Data/Sounds/se_kirakira.mp3");

	LoadSound(ResourceKeys::SE_bubu, L"./Data/Sounds/se_bubu.mp3");

	LoadSound(ResourceKeys::SE_StartVoice, L"./Data/Sounds/はじまります！/voice_hazimarimasu.mp3");
	LoadSound(ResourceKeys::SE_GameStart, L"./Data/Sounds/se_start.mp3");
	LoadSound(ResourceKeys::SE_Camera, L"./Data/Sounds/se_camera.mp3");

    LoadFont(ResourceKeys::Font_Title, L"./Data/Fonts/Bitcount/static/Bitcount-Light.ttf");
}

void ResourceManager::UnloadAll()
{
    UnloadGrids();
    UnloadFonts();
}

// ===============================[  GRIDS  ]===================================

const DxPlus::Sprite::SpriteBase* ResourceManager::GridAt(const std::wstring& key, int x, int y)
{
    auto it = grids.find(key);
    if (it == grids.end()) return nullptr;
    const auto& g = it->second;
    if (x < 0 || x >= g.num.x || y < 0 || y >= g.num.y) return nullptr;
    int idx = y * g.num.x + x;
    return &g.frames[idx];
}

void ResourceManager::UnloadGrids()
{
    for (auto& kv : grids) {
        for (auto& f : kv.second.frames) {
            int gid = f.GetID();
            if (gid >= 0) DxPlus::Sprite::Delete(gid);
        }
    }
    grids.clear();
}

int ResourceManager::GetMusic(const std::wstring& key)
{
    auto it = musics.find(key);
    return (it != musics.end()) ? it->second : -1;
}

int ResourceManager::GetSound(const std::wstring& key)
{
    auto it = sounds.find(key);
    return (it != sounds.end()) ? it->second : -1;
}

int ResourceManager::LoadMusic(const std::wstring& key, const std::wstring& path)
{
    int music = DxLib::LoadSoundMem(path.c_str());
    if (music == -1) DxPlus::Utils::FatalError((L"Failed to load music " + path).c_str());
    musics[key] = music;
    return music;
}

int ResourceManager::LoadSound(const std::wstring& key, const std::wstring& path)
{
    int sound = DxLib::LoadSoundMem(path.c_str());
    if (sound == -1) DxPlus::Utils::FatalError((L"Failed to load music " + path).c_str());
    sounds[key] = sound;
    return sound;
}

void ResourceManager::UnloadMusics()
{
    for (auto& m : musics)
    {
        if (m.second >= 0) DxLib::DeleteSoundMem(m.second);
    }
    musics.clear();
}

void ResourceManager::UnloadSounds()
{
    for (auto& s : sounds)
    {
        if (s.second >= 0) DxLib::DeleteSoundMem(s.second);
    }
    sounds.clear();
}

// ===============================[  FONTS  ]===================================

int ResourceManager::GetFont(const std::wstring& fontName)
{
    auto it = fonts.find(fontName);
    if (it == fonts.end())
    {
        DxPlus::Utils::FatalError((L"Font not found: " + fontName).c_str());
    }
    return it->second.handle;
}

int ResourceManager::LoadFont(const std::wstring& fontName, const std::wstring& path)
{
    if (AddFontResourceExW(path.c_str(), FR_PRIVATE, 0) == 0)
    {
        DxPlus::Utils::FatalError((std::wstring(L"Failed to add font: ") + path).c_str());
    }

    int handle = DxPlus::Text::InitializeFont(fontName.c_str(), 40, 2);
    if (handle == -1)
    {
        DxPlus::Utils::FatalError((std::wstring(L"Failed to init font: ") + fontName).c_str());
    }

    fonts[fontName] = { handle, path };

    return handle;
}

void ResourceManager::UnloadFont(const std::wstring& fontName)
{
    auto it = fonts.find(fontName);
    if (it == fonts.end()) return;

    auto& info = it->second;

    if (info.handle != -1)
    {
        DxPlus::Text::DeleteFont(info.handle);
        info.handle = -1;
    }

    if (!info.path.empty())
    {
        RemoveFontResourceExW(info.path.c_str(), FR_PRIVATE, 0);
    }

    fonts.erase(it);
}

void ResourceManager::UnloadFonts()
{
    std::vector<std::wstring> keys;
    keys.reserve(fonts.size());
    for (const auto& pair : fonts)// kv:key-value
    {
        keys.push_back(pair.first);
    }

    for (const auto& name : keys)
    {
        UnloadFont(name);
    }
}
