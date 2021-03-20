
list(
        APPEND
        SRC_FILE
        bvhtree.cpp
        renderer.cpp
        scene.cpp
        model.cpp
        material.cpp
        texture.cpp
        renderer.cpp
        image.cpp
)
add_library(rt_core ${SRC_FILE})
target_include_directories(rt_core PUBLIC ${GLM_INCLUDE_DIR} ${TINY_OBJ_LOADER_INCLUDE_DIR})
find_package(OpenMP REQUIRED)
target_link_libraries(rt_core PRIVATE spdlog::spdlog OpenMP::OpenMP_CXX)