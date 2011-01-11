include(FindOpenAL)
include(FindMPG123)

add_library (TCAudio
	TCAudioApi.h
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
	TCAudioManager.h
	TCAudioOpenAL.h
	TCAudioOpenALHandler.cpp
	TCAudioOpenALHandler.h
	TCAudioSound.h
	TCAudioSoundCompound.h
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
