# -------------------------------------------------------------------------------------------------
# Setup
# -------------------------------------------------------------------------------------------------
include(Catch)


# -------------------------------------------------------------------------------------------------
# Unit tests for the GameEngine library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Audio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Collision.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/EntityManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/System.cpp
)
target_link_libraries(Test_GameEngine PRIVATE GameEngine GameScenes Catch2::Catch2WithMain)
set_target_properties(Test_GameEngine PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
SetFolderInVS(Test_GameEngine Test)

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET Test_GameEngine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:Test_GameEngine>
                $<TARGET_FILE_DIR:Test_GameEngine>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

#add_test(NAME GameEngine COMMAND Test_GameEngine)
catch_discover_tests(Test_GameEngine WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)

# -------------------------------------------------------------------------------------------------
# Unit tests for the GameMath library
# -------------------------------------------------------------------------------------------------
add_executable(Test_GameMath
    ${CMAKE_CURRENT_SOURCE_DIR}/GameMath/Vec2.cpp
)
target_link_libraries(Test_GameMath PRIVATE GameMath Catch2::Catch2WithMain)
set_target_properties(Test_GameMath PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
SetFolderInVS(Test_GameMath Test)

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET Test_GameMath
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:Test_GameMath>
                $<TARGET_FILE_DIR:Test_GameMath>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

#add_test(NAME GameMath COMMAND Test_GameMath)
catch_discover_tests(Test_GameMath WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)


# -------------------------------------------------------------------------------------------------
# Code coverage
# -------------------------------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable coverage" ON)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND ENABLE_COVERAGE)
    include(code_coverage)
    message(STATUS "Enabled code coverage with clang")

    target_compile_options(Test_GameEngine PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)
    target_link_libraries(Test_GameEngine PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)

    generate_coverage_report_for(Test_GameEngine)
    #generate_coverage_report_for(Test_GameMath)
endif()