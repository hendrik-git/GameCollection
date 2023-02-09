# -------------------------------------------------------------------------------------------------
# Building the executable
# -------------------------------------------------------------------------------------------------
add_executable(RemoteControlApp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_compile_features(RemoteControlApp PUBLIC cxx_std_20)
set_target_properties(RemoteControlApp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(RemoteControlApp PRIVATE CLI11::CLI11 ftxui::component SFML::Network)
target_compile_options(RemoteControlApp PRIVATE ${ADDITIONAL_COMPILER_FLAGS})

# copy all necessary files (i.e. dlls) to the directory of the executable this is necessary on
# Windows, since it doesn't support relative paths to libraries there is currently a bug, where a
# statically build target causes an error this happens when the list of dll passed to the copy
# command is empty
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET RemoteControlApp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:RemoteControlApp>
                $<TARGET_FILE_DIR:RemoteControlApp>
        COMMAND_EXPAND_LISTS # expands list to multiple arguments instead of ; seperated
        COMMENT "Copying necessary dll files"
    )
endif()

setfolderinvs(RemoteControlApp Executables)