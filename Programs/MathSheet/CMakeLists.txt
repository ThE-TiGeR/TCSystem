
add_executable (TCMathSheet
    MathSheet.cpp
)

target_link_libraries(TCMathSheet
    TCBasic
)
add_dependencies(TCMathSheet
    TCBasic
)

set_target_properties(TCMathSheet PROPERTIES FOLDER Programs)
