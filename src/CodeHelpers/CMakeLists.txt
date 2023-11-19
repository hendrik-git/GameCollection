# -------------------------------------------------------------------------------------------------
# Building the CodeHelpers library
# -------------------------------------------------------------------------------------------------
add_library(
    CodeHelpers STATIC
    # (optional) add header files, so that they may show up in the IDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CodeHelpers/Logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CodeHelpers/Overload.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CodeHelpers/Profiler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/CodeHelpers/ProfilerCategories.hpp
    # necessary source files
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Overload.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Profiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProfilerCategories.cpp
)
target_include_directories(CodeHelpers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(CodeHelpers PUBLIC cxx_std_20)
target_compile_options(CodeHelpers PRIVATE ${ADDITIONAL_COMPILER_FLAGS})
target_link_libraries(CodeHelpers PUBLIC fmt::fmt quill::quill)

if(USE_PROFILING)
    include(NoWarningsLinking)
    target_link_libraries_system(CodeHelpers perfetto::perfetto quill::quill)
    target_compile_definitions(CodeHelpers PUBLIC PROFILING)
endif()