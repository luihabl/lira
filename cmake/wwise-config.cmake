if(NOT DEFINED ENV{WWISESDK})
message(FATAL_ERROR "WWISESDK environment variable must be set to the WWise SDK path")
endif()

string(REPLACE "\\" "/" wwise_path $ENV{WWISESDK})
set(wwise_sample_path "${wwise_path}/samples/SoundEngine")

set(platform_folder "")
set(platform_name "")

if(UNIX AND APPLE)
    message(STATUS "WWise: using UNIX configuration")
    
    set(platform_folder "POSIX")
    
    set(WWISE_LIBRARY_DIRS
    "$ENV{WWISESDK}/Mac/Release/lib"
    )

    set(WWISE_LINKER_FLAGS
    "-framework AudioToolbox -framework CoreAudio -framework AudioUnit -framework Cocoa -framework QuartzCore -framework AppKit"
    )

    set(WWISE_DEFS
    -DUNICODE 
    -DNDEBUG
    -DAK_OPTIMIZED
    )

elseif(WIN32)
    message(STATUS "WWise: Using Win32 configuration")

    set(platform_folder "Win32")

    set(WWISE_LIBRARY_DIRS
    "$ENV{WWISESDK}/x64_vc160/Release/lib"
    )

    add_definitions(
    -DUNICODE 
    -D_UNICODE
    #-D_DEBUG
    -DWIN64
    -DWIN32_LEAN_AND_MEAN
    -DINTDEMO_RENDER_D3D11
    -D_HAS_EXCEPTIONS=0
    -D_WINDOWS
    -D_CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_DEPRECATE
    -D_CRT_NON_CONFORMING_SWPRINTFS
    -DNDEBUG
    -DAK_OPTIMIZED
    /Zc:wchar_t
    )

    #set_target_properties(wwise_integration PROPERTIES COMPILE_FLAGS "/Zc:wchar_t")

endif()

set(WWISE_INCLUDE_DIRS
    "$ENV{WWISESDK}/include"
    "${wwise_sample_path}/Common"
    "${wwise_sample_path}/${platform_folder}"
)

set(WWISE_SRC
    "${wwise_sample_path}/${platform_folder}/AkDefaultIOHookBlocking.cpp"
    "${wwise_sample_path}/${platform_folder}/AkDefaultIOHookDeferred.cpp"
    "${wwise_sample_path}/${platform_folder}/stdafx.cpp"

    # "${wwise_sample_path}/Common/AkFilePackageLowLevelIO.inl"
    # "${wwise_sample_path}/Common/AkDefaultLowLevelIODispatcher.cpp"
    # "${wwise_sample_path}/Common/AkFileLocationBase.cpp"
    # "${wwise_sample_path}/Common/AkFilePackage.cpp"
    # "${wwise_sample_path}/Common/AkFilePackageLUT.cpp"
    # "${wwise_sample_path}/Common/AkMultipleFileLocation.cpp"
    "${wwise_sample_path}/Common/AkDefaultLowLevelIODispatcher.cpp"
    "${wwise_sample_path}/Common/AkFileLocationBase.cpp"
    "${wwise_sample_path}/Common/AkFileLocationBase.h"
    "${wwise_sample_path}/Common/AkFilePackage.cpp"
    "${wwise_sample_path}/Common/AkFilePackage.h"
    "${wwise_sample_path}/Common/AkFilePackageLUT.cpp"
    "${wwise_sample_path}/Common/AkFilePackageLUT.h"
    "${wwise_sample_path}/Common/AkMultipleFileLocation.cpp"
    "${wwise_sample_path}/Common/AkMultipleFileLocation.h"
)



set(WWISE_LIBS
    AkAudioInputSource
    AkAutobahn
    AkCompressorFX
    AkDelayFX
    AkExpanderFX
    AkFlangerFX
    AkGainFX
    AkGuitarDistortionFX
    AkHarmonizerFX
    AkMatrixReverbFX
    AkMemoryMgr
    AkMeterFX
    AkMusicEngine
    AkOpusDecoder
    AkParametricEQFX
    AkPeakLimiterFX
    AkPitchShifterFX
    AkRecorderFX
    AkRoomVerbFX
    AkSilenceSource
    AkSineSource
    AkSoundEngine
    AkSpatialAudio
    AkStereoDelayFX
    AkStreamMgr
    AkSynthOneSource
    AkTimeStretchFX
    AkToneSource
    AkTremoloFX
    AkVorbisDecoder
    # CommunicationCentral
)

if(UNIX AND APPLE)
    set(WWISE_LIBS
        ${WWISE_LIBS}
        AkAACDecoder
    )
elseif(WIN32)
    set(WWISE_LIBS
        ${WWISE_LIBS}
        ws2_32.lib
        dinput8.lib
        Dsound.lib
        Msacm32.lib
        Dbghelp.lib
        Winmm.lib
        d3d11.lib
    )
endif()

