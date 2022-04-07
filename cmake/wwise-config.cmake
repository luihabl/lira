if(NOT DEFINED ENV{WWISESDK})
message(FATAL_ERROR "WWISESDK environment variable must be set to the WWise SDK path")
endif()

string(REPLACE "\\" "/" wwise_path $ENV{WWISESDK})
set(wwise_sample_path "${wwise_path}/samples/SoundEngine")

set(platform_folder "")
if(UNIX)
    message(STATUS "WWise: using UNIX configuration")
    set(platform_folder "POSIX")
elseif(WIN32)
    message(STATUS "WWise: Using Win32 configuration")
    set(platform_folder "Win32")
endif()

set(WWISE_INCLUDE_DIRS
    "$ENV{WWISESDK}/include"
    "${wwise_sample_path}/${platform_folder}"
)

set(WWISE_LIBRARY_DIRS
    "$ENV{WWISESDK}/Mac/Release/lib"
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

set(WWISE_LINKER_FLAGS
"-framework AudioToolbox -framework CoreAudio -framework AudioUnit -framework Cocoa -framework QuartzCore -framework AppKit"
)

set(WWISE_LIBS
AkAACDecoder
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

set(WWISE_DEFS
-DUNICODE 
# -D_UNICODE
# -D_DEBUG
-DNDEBUG
-DAK_OPTIMIZED
# -DLUA_USE_MACOSX
# -DWIN64
# -DWIN32_LEAN_AND_MEAN
# -DINTDEMO_RENDER_D3D11
# -D_HAS_EXCEPTIONS=0
# -D_WINDOWS
# -D_CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS
# -D_CRT_SECURE_NO_WARNINGS
# -D_CRT_SECURE_NO_DEPRECATE
# -D_CRT_NON_CONFORMING_SWPRINTFS
)