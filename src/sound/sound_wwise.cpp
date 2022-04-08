#ifdef LIRA_SOUND_WWISE

#include "sound.h"
#include "tinysdl.h"
#include "../assets/content.h"
#include "../assets/sound_bank.h"

#include <AK/Plugin/AllPluginsFactories.h> //<--------------- include this!!!
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
// #include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Platforms/Mac/AkTypes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>		// Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>			// Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>		// Streaming Manager
#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>	// Music Engine
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>	// AkStreamMgrModule
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>	// Spatial Audio module
#include <AK/SoundEngine/Common/AkCallback.h>    // Callback
#include <AK/IBytes.h>

#include "AkFilePackageLowLevelIOBlocking.h"

#include <vector>
#include <filesystem>
using fspath = std::filesystem::path;

using namespace Lira;
using namespace TinySDL;

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

bool load_bank(const char* name)
{
    AkBankID bankID;
    AKRESULT eResult = AK::SoundEngine::LoadBank( name, bankID );
    if(eResult != AK_Success) 
    {
        Log::error("Error %d in loading bank %s", eResult, name);
        return false;
    }
    loaded_banks.push_back(std::string(name));
    return true;
}

void set_base_path(const char* path)
{
    g_lowLevelIO.SetBasePath( path );
}

void unload_all_banks()
{
    for(const auto& bank : loaded_banks)
        AK::SoundEngine::UnloadBank(bank.c_str(), NULL);
}

void Sound::init()
{
    if(initialiazed)
        return;

    init_sound_engine();

    AK::StreamMgr::SetCurrentLanguage( AKTEXT("English(US)") );

    fspath platform;
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        patform = "Windows";
    #elif __APPLE__
        platform = "Mac";
    #endif
    
    const auto& sound_banks = Content::find_all<SoundBank>();
    for(const auto& [k, bank] : sound_banks)
        if(bank.data.platform == platform && bank.data.name == "Init")
        {
            set_base_path(bank.folder.c_str());
            load_bank("Init.bnk");
            break;
        }
    
    for(const auto& [k, bank] : sound_banks)
        if(bank.data.platform == platform && bank.data.name != "Init")
            load_bank(bank.file_name.c_str());
        
}


void Sound::terminate()
{
    unload_all_banks();
    terminate_sound_engine();
}







#endif