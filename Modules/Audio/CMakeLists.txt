find_package(OpenAL)
find_Package(MPG123)
find_Package(OGG)
find_Package(VORBIS)
find_Package(VorbisFile)

include_directories (
   inc
   ../Basic/inc
   ../Thread/inc
   ${OPENAL_INCLUDE_DIR}
   ${OGG_INCLUDE_DIR}
   ${VORBIS_INCLUDE_DIR}
   ${VorbisFile_INCLUDE_DIR}
)

set (TC_AUDIO_PUBLIC_HEADER
    inc/TCAudioApi.h
    inc/TCAudioManager.h
    inc/TCAudioSound.h
    inc/TCAudioSoundCompound.h
    inc/TCAudioSoundFormat.h
)

set (TC_AUDIO_SOURCE_FILES
	src/TCAudioChangePanTask.cpp
	src/TCAudioChangePanTask.h
	src/TCAudioChangePitchTask.cpp
	src/TCAudioChangePitchTask.h
	src/TCAudioChangeSoundValueTask.cpp
	src/TCAudioChangeSoundValueTask.h
	src/TCAudioChangeVolumeTask.cpp
	src/TCAudioChangeVolumeTask.h
	src/TCAudioDebugNew.cpp
	src/TCAudioManager.cpp
	src/TCAudioOpenAL.h
	src/TCAudioOpenALHandler.cpp
	src/TCAudioOpenALHandler.h
	src/TCAudioSoundData.h
	src/TCAudioSoundDataCompound.cpp
	src/TCAudioSoundDataCompound.h
	src/TCAudioSoundDataMapper.h
	src/TCAudioSoundDataMp3.h
	src/TCAudioSoundDataOgg.cpp
	src/TCAudioSoundDataOgg.h
	src/TCAudioSoundDataMultitrack.cpp
	src/TCAudioSoundDataMultitrack.h
	src/TCAudioSoundDataSequence.cpp
	src/TCAudioSoundDataSequence.h
	src/TCAudioSoundDataWav.cpp
	src/TCAudioSoundDataWav.h
	src/TCAudioSoundImp.cpp
	src/TCAudioSoundImp.h
	src/TCAudioSoundMultitrackImp.cpp
	src/TCAudioSoundMultitrackImp.h
	src/TCAudioSoundPlayer.h
	src/TCAudioSoundSequenceImp.cpp
	src/TCAudioSoundSequenceImp.h
	src/TCAudioStreamingTask.cpp
	src/TCAudioStreamingTask.h
	src/TCAudioStreamingThread.cpp
	src/TCAudioStreamingThread.h
	src/TCAudioUtil.h
)

if (MPG123_FOUND)
    include_directories (
       ${MPG123_INCLUDE_DIR1}
       ${MPG123_INCLUDE_DIR2}
    )
    set (TC_AUDIO_SOURCE_FILES
        ${TC_AUDIO_SOURCE_FILES}
        src/TCAudioSoundDataMp3.cpp
    )
else()
    set(MPG123_LIBRARIES "")
endif()

add_library (TCAudio
    ${TC_AUDIO_SOURCE_FILES}
    ${TC_AUDIO_PUBLIC_HEADER}
)

target_link_libraries (TCAudio
    TCThread
    TCBasic
    ${OPENAL_LIBRARY}
    ${MPG123_LIBRARIES}
    ${VorbisFile_LIBRARY}
    ${VORBIS_LIBRARY}
    ${OGG_LIBRARY}
)

set_target_properties(TCAudio PROPERTIES FOLDER Modules)
set_target_properties(TCAudio PROPERTIES COMPILE_DEFINITIONS "TCAUDIO_EXPORTS")
set_target_properties(TCAudio PROPERTIES PUBLIC_HEADER "${TC_AUDIO_PUBLIC_HEADER}")

install(TARGETS TCAudio
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   LIBRARY DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   ARCHIVE DESTINATION ${TC_SYSTEM_LIB_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION include
   COMPONENT Audio
)

