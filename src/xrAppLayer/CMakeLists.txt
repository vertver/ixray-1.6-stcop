project(xrAppLayer)
set(XR_APPLAYER_SRC_FILES)

# Files import
file(GLOB_RECURSE XR_APPLAYER_SOURCE_ALL_FILES
    "*.*"
)

file(GLOB_RECURSE XR_APPLAYER_SOURCE_CPP_FILES
    "*.cpp"
    "*.h"
)

source_group("Source Files" FILES ${XR_APPLAYER_SOURCE_CPP_FILES})

# Apply list
list(APPEND XR_APPLAYER_SRC_FILES ${XR_APPLAYER_SOURCE_ALL_FILES})

# xrPlay project
add_library(xrAppLayer SHARED ${XR_APPLAYER_SRC_FILES})
target_include_directories(xrAppLayer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(xrAppLayer PRIVATE "${IXRAY_SDK_INC}")

add_compile_options(/fp:fast)
target_compile_definitions(xrAppLayer PRIVATE "$<$<CONFIG:Debug>:DEBUG>")

target_precompile_headers(xrAppLayer PRIVATE "pch.h")
target_compile_definitions(xrAppLayer PRIVATE APPLAYER_BUILD)

# Linker list 
target_link_libraries(xrAppLayer PUBLIC "d3d11.lib")
target_link_libraries(xrAppLayer PRIVATE "SDL3-static")
target_link_libraries(xrAppLayer PUBLIC "ImGui::imgui")
target_link_libraries(xrAppLayer PUBLIC xrAPI)
target_link_libraries(xrAppLayer PUBLIC xrCore)