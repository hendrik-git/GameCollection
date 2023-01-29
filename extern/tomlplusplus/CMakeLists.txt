# -------------------------------------------------------------------------------------------------
# Header only library toml++
# -------------------------------------------------------------------------------------------------
add_library(tomlplusplus INTERFACE)
target_include_directories(tomlplusplus INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(tomlplusplus INTERFACE cxx_std_20)