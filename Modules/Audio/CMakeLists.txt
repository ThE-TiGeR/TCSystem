include(FindOpenAL)
include(FindMPG123)

set (TC_AUDIO_PUBLIC_HEADER
        TCAudioApi.h
        TCAudioManager.h
        TCAudioSound.h
        TCAudioSoundCompound.h
)

add_library (TCAudio
	TCAudioChangePanTask.cpp
	TCAudioChangePanTask.h
	TCAudioChangePitchTask.cpp
	TCAudioChangePitchTask.h
	TCAudioChangeSoundValueTask.cpp
	TCAudioChangeSoundValueTask.h
	TCAudioChangeVolumeTask.cpp
	TCAudioChangeVolumeTask.h
	TCAudioDebugNew.cpp
	TCAudioManager.cpp
	TCAudioOpenAL.h
	TCAudioOpenALHandler.cpp
	TCAudioOpenALHandler.h
	TCAudioSoundData.h
	TCAudioSoundDataCompound.cpp
	TCAudioSoundDataCompound.h
	TCAudioSoundDataMapper.h
	TCAudioSoundDataMp3.cpp
	TCAudioSoundDataMp3.h
	TCAudioSoundDataMultitrack.cpp
	TCAudioSoundDataMultitrack.h
	TCAudioSoundDataSequence.cpp
	TCAudioSoundDataSequence.h
	TCAudioSoundDataWav.cpp
	TCAudioSoundDataWav.h
	TCAudioSoundFormat.h
	TCAudioSoundImp.cpp
	TCAudioSoundImp.h
	TCAudioSoundMultitrackImp.cpp
	TCAudioSoundMultitrackImp.h
	TCAudioSoundPlayer.h
	TCAudioSoundSequenceImp.cpp
	TCAudioSoundSequenceImp.h
	TCAudioStreamingTask.cpp
	TCAudioStreamingTask.h
	TCAudioStreamingThread.cpp
	TCAudioStreamingThread.h
	TCAudioUtil.h
        ${TC_AUDIO_PUBLIC_HEADER}
)

include_directories (
   "${OPENAL_INCLUDE_DIR}"
   "${MPG123_INCLUDE_DIR1}"
   "${MPG123_INCLUDE_DIR2}"
)

target_link_libraries (TCAudio
    TCThread
    TCBasic
    ${OPENAL_LIBRARY}
    ${MPG123_LIBRARIES}
)

add_dependencies (TCAudio
    TCThread
    TCBasic
)

set_target_properties(TCAudio PROPERTIES FOLDER Modules)
set_target_properties(TCAudio PROPERTIES COMPILE_DEFINITIONS "TCAUDIO_EXPORTS")
set_target_properties(TCAudio PROPERTIES PUBLIC_HEADER "${TC_AUDIO_PUBLIC_HEADER}")

install(TARGETS TCAudio
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION include
   COMPONENT Audio
)

