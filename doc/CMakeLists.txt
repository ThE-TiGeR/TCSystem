find_package(Doxygen)
#
if(DOXYGEN_EXECUTABLE)
add_custom_target(Documentation
  ${DOXYGEN}
  ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.doxygen
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Build doxygen documentation"
)
endif(DOXYGEN_EXECUTABLE)
