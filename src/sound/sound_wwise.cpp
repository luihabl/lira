#ifdef LIRA_SOUND_WWISE

#include "sound.h"
#include "tinysdl.h"
#include "../assets/content.h"
#include "../assets/sound_bank.h"

#include <AK/Plugin/AllPluginsFactories.h> //<--------------- include this!!!
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>
#elif __APPLE__
#include <AK/SoundEngine/Platforms/Mac/AkTypes.h>
#endif
#include <AK/SoundEngine/Common/AkMemoryMgr.h>		// Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>			// Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>		// Streaming Manager
#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>	// Music Engine
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>	// AkStreamMgrModule
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>	// Spatial Audio module
#include <AK/SoundEngine/Common/AkCallback.h>    // Callback
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/IBytes.h>

#include "AkFilePackageLowLevelIOBlocking.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
using fspath = std::filesystem::path;

using namespace Lira;
using namespace TinySDL;

#define N_MAX_QUERY 50


struct WwiseGameObject
{
    AkGameObjectID id;
    std::string name;

    WwiseGameObject() = default;
    WwiseGameObject(const char* _name, AkGameObjectID _id)
    {
        name = std::string(_name);
        id = _id;
    }

    void register_object()
    {
        AK::SoundEngine::RegisterGameObj( id, name.c_str() );
    }

    void unregister_object()
    {
        AK::SoundEngine::UnregisterGameObj(id);
    }
};




namespace
{
    bool initialiazed = false;
    CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
    AkMemSettings memSettings;
    AkStreamMgrSettings stmSettings;
    AkDeviceSettings deviceSettings;
    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

    std::vector<std::string> loaded_banks;
    WwiseGameObject default_obj;
    WwiseGameObject default_listener;

    constexpr AkGameObjectID DEFAULT_GAME_OBJECT = 100;
    constexpr AkGameObjectID DEFAULT_LISTENER = 1000;
}

bool init_sound_engine()
{
    AK::MemoryMgr::GetDefaultSettings(memSettings);
    if ( AK::MemoryMgr::Init( &memSettings ) != AK_Success )
    {
        Log::error("Could not create the memory manager.");
        return false;
    }

    
    AK::StreamMgr::GetDefaultSettings( stmSettings );   
    if ( !AK::StreamMgr::Create( stmSettings ) )

    {
        Log::error("Could not create the Streaming Manager" );
        return false;
    }

    AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

    deviceSettings.bUseStreamCache = true;

    if ( g_lowLevelIO.Init( deviceSettings ) != AK_Success )
    {
        Log::error("Could not create the streaming device and Low-Level I/O system" );
        return false;
    }
    
    AK::SoundEngine::GetDefaultInitSettings( initSettings );
    AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );
	
    if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success )
    {
        Log::error("Could not initialize the Sound Engine." );
        return false;
    }

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings( musicInit );
    if ( AK::MusicEngine::Init( &musicInit ) != AK_Success )
    {
        Log::error("Could not initialize the Music Engine." );
        return false;
    }

    if ( AK::SpatialAudio::Init( settings ) != AK_Success )
    {
        Log::error("Could not initialize the Spatial Audio." );
        return false;
    }

    Log::info("Wwise SDK %d.%d.%d.%d", AK_WWISESDK_VERSION_MAJOR, AK_WWISESDK_VERSION_MINOR, AK_WWISESDK_VERSION_SUBMINOR, AK_WWISESDK_VERSION_BUILD);
    return true;
}

void terminate_sound_engine()
{
    AK::MusicEngine::Term();
    AK::SoundEngine::Term();

    g_lowLevelIO.Term();
    if ( AK::IAkStreamMgr::Get() )
        AK::IAkStreamMgr::Get()->Destroy();

    AK::MemoryMgr::Term();
}

bool load_bank(const std::string& name)
{
    AkBankID bankID;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    AKRESULT eResult = AK::SoundEngine::LoadBank(std::wstring(name.begin(), name.end()).c_str(), bankID);
#elif __APPLE__
    AKRESULT eResult = AK::SoundEngine::LoadBank(name.c_str(), bankID);
#endif
    
    if(eResult != AK_Success) 
    {
        Log::error("Error %d in loading bank %s", eResult, name.c_str());
        return false;
    }
    loaded_banks.push_back(name);
    return true;
}

void set_base_path(const std::string& path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    g_lowLevelIO.SetBasePath(std::wstring(path.begin(), path.end()).c_str());
#elif __APPLE__
    g_lowLevelIO.SetBasePath(path.c_str());
#endif
}

void unload_all_banks()
{
    for(const auto& bank : loaded_banks)
        AK::SoundEngine::UnloadBank(bank.c_str(), NULL);
    loaded_banks.clear();
}

bool is_event_playing(const std::string& event_name, AkGameObjectID object)
{
    AkUInt32 test_event_id = AK::SoundEngine::GetIDFromString(event_name.c_str());
    
    AkUInt32 count = N_MAX_QUERY;
    AkUInt32 playing_ids[N_MAX_QUERY];

    AKRESULT result = AK::SoundEngine::Query::GetPlayingIDsFromGameObject(object, count, playing_ids);

    for (int i = 0; i < count; i++)
    {
        AkUInt32 playing_id = playing_ids[i];
        AkUInt32 event_id = AK::SoundEngine::Query::GetEventIDFromPlayingID(playing_id);

        if (event_id == test_event_id)
            return true;
    }
    return false;
}

void stop_event(const std::string& event_name, AkGameObjectID object)
{
    AkUInt32 test_event_id = AK::SoundEngine::GetIDFromString(event_name.c_str());

    AkUInt32 count = N_MAX_QUERY;
    AkUInt32 playing_ids[N_MAX_QUERY];

    AKRESULT result = AK::SoundEngine::Query::GetPlayingIDsFromGameObject(object, count, playing_ids);

    for (int i = 0; i < count; i++)
    {
        AkUInt32 playing_id = playing_ids[i];
        AkUInt32 event_id = AK::SoundEngine::Query::GetEventIDFromPlayingID(playing_id);

        if (event_id == test_event_id)
            AK::SoundEngine::StopPlayingID(playing_id);
    }
}


void Sound::init()
{
    if(initialiazed)
        return;

    init_sound_engine();

    AK::StreamMgr::SetCurrentLanguage( AKTEXT("English(US)") );

    fspath platform;
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        platform = "Windows";
    #elif __APPLE__
        platform = "Mac";
    #endif
    
    const auto& sound_banks = Content::find_all<SoundBank>();
    for(const auto& [k, bank] : sound_banks)
        if(bank.data.platform == platform && bank.data.name == "Init")
        {
            set_base_path(bank.folder.generic_string());
            load_bank(bank.file_name.generic_string());
            break;
        }
    
    for(const auto& [k, bank] : sound_banks)
        if(bank.data.platform == platform && bank.data.name == "Lira")
            load_bank(bank.file_name.generic_string());

    // For now we use only one Game Object and one Listener.
    // Aftewards we need to create a way to make several objects and change
    // their positions.
    default_obj = WwiseGameObject("DEFAULT_GAME_OBJECT", DEFAULT_GAME_OBJECT);
    default_obj.register_object();

    default_listener = WwiseGameObject("DEFAULT_LISTENER", DEFAULT_LISTENER);
    default_listener.register_object();
    AK::SoundEngine::SetDefaultListeners(&default_listener.id, 1);    
}

void Sound::play(const char* name)
{
    AK::SoundEngine::PostEvent(name, default_obj.id);
}

void Sound::update()
{
    if (AK::SoundEngine::IsInitialized())
        AK::SoundEngine::RenderAudio();
}

void Sound::stop(const char* name)
{
    stop_event(name, default_obj.id);
}

void Sound::stop_all()
{
    AK::SoundEngine::StopAll(default_obj.id);
}

void Sound::terminate()
{
    if(initialiazed)
    {
        stop_all();
        AK::SoundEngine::UnregisterAllGameObj();
        unload_all_banks();
        terminate_sound_engine();
        initialiazed = false;
    }
}



#endif