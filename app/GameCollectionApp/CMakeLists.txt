# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
add_executable(GameCollectionApp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_compile_features(GameCollectionApp PUBLIC cxx_std_20)
set_target_properties(GameCollectionApp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(GameCollectionApp PRIVATE CLI11::CLI11 GameScenes GameEngine CodeHelpers tomlplusplus quill::quill)
target_compile_options(GameCollectionApp PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET GameCollectionApp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:GameCollectionApp>
                $<TARGET_FILE_DIR:GameCollectionApp>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

setfolderinvs(GameCollectionApp Executables)